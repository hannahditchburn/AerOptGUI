# The AerOpt GUI

## Compiling statically with Docker
If you are compiling on windows the recommended way of compiling the AerOpt GUI is using the included docker 
image "Dockerfile.mxe". This has been setup to create a single executable with all the QT libraires built
into it. Docker is a tool to run lightweight virtual machines to perform specific tasks. In our case this
has been configured to run a minimal install of ubuntu linux which has been setup with a static build of QT
and the tools nessacary to crosscompile AerOpt GUI for windows. 

This may seem quite a roundabout way of achiving this since is it is possible to build QT staically for windows.
However, this process is far from easy as it fraught with errors and is as such not recomended. If you don't wish 
to (or can't) use Docker you can follow the instructions under **Compiling in QT Creator** to compile a dynamically 
linked executable with QT creator. You will then however, need to perform some extra steps to distribute the executable.
see **Deploying** for details.

### Setup
First you will need to install [git](https://gitforwindows.org/), if you don't already have it.
Go to the [git for Windows home page](https://gitforwindows.org/) and click download.

Next download docker desktop for [windows](https://www.docker.com/get-started) and install it with the default settings.

Windows will ask you to reboot after which docker will automatcally start runnng once you log back in (if you don't want 
this behaviour you can change this in the settings however, you need to remember to start docker whenever you want to re-compile
the code). 

At this point docker may throw an error about windows sub system for linux (WSL2) support. If this occurs you need to 
download the WSL2 kernal from [here](https://docs.microsoft.com/en-gb/windows/wsl/wsl2-kernel) and install it.

At this point there is a tutorial which can go through if you wish (or you can press the skip button in the bottom left corner) 
after which you can close docker.

Next, download the AerOpt GUI repo. Since you have git you should have a program called `Git Bash` which you can use it to clone 
the project. Run `Git Bash` to open the command line interface.
 
If you are familiar with it, use `cd` to navigate to where you want to download the repo. If not, you probably want to place it 
in your home folder (generally this is 'C:\Users\USERNAME'), which is where you start.

Copy the following commands in to the command line and press Enter
```
git clone https://github.com/DrBenEvans/AerOptGUI.git
cd AerOptGUI
```

The final setup setup step is to build the docker container with the following command. 
```
docker build -f Dockerfile.mxe -t qt/mxe:win64 .
```

Warning this command can take several hours to complete depending on your computer and network connection as it has to fetch and 
compile a of lot diferent of tools (fortunatley it is only required to be run once). 

### Compiling the code
Once the setup is complete we can enter the docker container with the following command.   
```
winpty docker run -it -v "$(pwd)":/AerOptGUI qt/mxe:win64
```

Note if you previously closed `Git Bash` will need to use `cd` to move into the git repo. If you downloaded it in your home folder 
just run `cd AerOptGUI` if you downloaded it elsewhere you will need to go there.

Once inside the container we need to run the following to compile the code:
```
	cd /AerOptGUI/
	qmake GeneticGui-docker.pro
	make
```

Once finished we can stop the container with the command 
```
	exit
```

If you need to re-compile the code you can simply re-run the container (you shouldn't need to re-build it).
If you want to permently delete the container simply run the following in `Git Bash`:
```
docker rm -f qt/mxe:win64
```
## Compiling in QT Creator

### Install Compilation Requirements


You can download the IDE on the [QT website](https://www.qt.io/download).
Choose the open source option and download the installer.
In the installer, you will need to create a QT account if you don't already have one.
When selecting components, check the latest version of QT and 64 bit version of MinGW.

You will also need the ssh and ssl libraries.
On Windows, we recommend using [vcpkg](https://github.com/microsoft/vcpkg) for this.
First you will need to install [git](https://gitforwindows.org/), if you don't already have it.
Go to the [git for Windows home page](https://gitforwindows.org/) and click download.

After installing git, you should also have a program called `Git Bash`.
Run it to open a command line interface. If you are familiar with it, use `cd`
to navigate where you want to install vcpkg.
If not, you probably want to install it in your home folder, which is where you start.

Copy the following commands in to the command line and press Enter to execute
```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.sh
```
For later convenience, you may want to execute the following line as well.
Windows will ask for admin access.
```
./vcpkg integrate install
```

Now you can install libraries. You are already in the correct folder, but once you
close the window and open a new one, you will need to run
```
cd vcpkg
```
to get back.
Now install `libssh`, `openssl` and `zlib` with the following command
```
./vcpkg install libssh:x64-windows openssl:x64-windows zlib:x64-windows
```

Next, open `System Properties` in the start menu, and press enter.
Click `Environment Variables`, find `path` in the second panel and double click.
Add the following three lines as separate entries
```
C:\Users\USERNAME\vcpkg\packages\libssh_x64-windows\bin
C:\Users\USERNAME\vcpkg\packages\openssl-windows_x64-windows\bin
C:\Users\USERNAME\vcpkg\packages\zlib_x64-windows\bin
```
replacing `USERNAME` with your actual user name. For this change to take effect, you may need to reboot.

Note that if you installed `vcpkg` in a custom location, you need to change the paths accordingly.

### Setup Project

Next, download the AerOpt GUI. Since you have git, you can use it to clone the project. Open `Git Bash` and run the command
```
git clone https://github.com/DrBenEvans/AerOptGUI.git
```

Open QT Creator, click `Open Project` and open `AerOptGUI\GeneticGui.pro`, which should now be in your home directory.
The first time you do this the IDE will ask you about the configuration.
Choose the latest 64 bit MinGW version and click `configure`.

Next, open `GeneticGui.pro` in the editor and scroll to the bottom.
You will see several lines including the path
```
C:\Users\USERNAME\src\vcpkg...
```
Replace `USERNAME` with your username on each line. If `vcpkg` is in a custom location, use that path instead.

### Setup on the Cluster

The AerOpt GUI depends on a specific version of the AerOpt tool on the cluster.
You can use git to get the newest version.
Log on to the cluster and run
```
mv AerOpt AerOpt-old
git clone https://github.com/DrBenEvans/AerOpt.git
```

Compile the new version of AerOpt
```
module load mpi
cd AerOpt/source
make all
ls -s AerOpt_v3.5 ../AerOpt
```

You should now be able to run jobs on the cluster using the GUI.


## Deploying

I you followed the docker instructions the compiled executable you created can be distributed without needing any additional libraries or tools. 
However, if you compiled the code dynamically using QT creator you will need to do some extra work.

There are two deployed releases in [Jarno's github page](https://github.com/rantahar/AerOpt-gui/releases). All you need to do to run this version
of AerOpt is to donwload AerOptGui.zip, extract the file and run AerOptGui.exe in the new folder. This is much easier for a user than compiling 
QT statically.

To create a new release compile the program using `release` setting in QT Creator.
This is above the play symbol on the bottom left in QT Creator 4.9.
When you compile the program, an executable called `AerOptGui.exe` is created in the same directory the AerOptGui folder is in.

Download the [zip archive](https://github.com/rantahar/AerOpt-gui/releases/download/0.2/AerOptGui.zip) and extract it.
The folder contains everything AerOptGui needs to run.
Replace `AerOptGui.exe` with the newly compiled executable and it should work directly.