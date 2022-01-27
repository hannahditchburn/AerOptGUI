# The AerOpt GUI

### Compiling statically with Docker
If you are compiling on windows the new recommended way of compiling the AerOpt GUI is to use Docker to create a Docker
container based on the included DockerFile "DockerFile.mxe". This has been setup to create a single
executable with all the QT libraires built into it. Docker is a tool to run lightweight virtual machines
(Docker call these containers) to perform specific tasks. In our case this has been configured to run a minimal
install of Ubuntu Linux which has been setup with a static build of QT and the tools necessary to crosscompile AerOpt GUI for windows. 

This may seem quite a roundabout way of achiving this, since is it is possible to build QT staically for Windows.
However, this process is far from easy as it fraught with errors. The interested reader can a find decent set of
instructions [here](https://tadeubento.com/2020/qt-static-builds-under-windows/) and [here](https://wiki.qt.io/Building_a_static_Qt_for_Windows_using_MinGW). However, I will warn you now
this is not for the faint hearted.

If you don't wish to (or can't) use Docker you can follow the instructions under **Compiling in QT Creator**
to compile a dynamically linked executable with QT creator. You will then however, need to perform some extra
steps to distribute the executable. See **Deploying** for details.

## Setup

First you will need to install git, if you don't already have it. Go to the [git for Windows home page]
(https://gitforwindows.org/) and click download.

Next download Docker Desktop for [windows](https://www.docker.com/get-started) and install it with the default settings.

Windows will ask you to reboot after which Docker will automatcally start running once you log back in. If you don't want 
this behaviour you can change this in the settings however, you need to remember to start Docker whenever you want to re-compile
the code. 

Note: Docker may throw an error saying WSL2 Installation is incomplete. If this occurs you need to 
download the WSL2 kernal from [here](https://docs.microsoft.com/en-gb/windows/wsl/wsl2-kernel) and install it.

At this point there is a tutorial, which you can go through if you wish (or you can press the skip button in the bottom left corner) 
after which you can close Docker.

Next, we need to download the AerOpt GUI repo. Since you have git we can use microsoft powershell to clone the project.
Open the start menu and search for `Powershell` this will open a blue textbox with white writing, This is called a comand prompt.
 
If you are familiar with it, use `cd` to navigate to where you want to download the repo. If not, you probably want to place it 
in your home folder (generally this is 'C:\Users\YOUR_USERNAME'), which is where you start.

Copy the following commands in to the command prompt and press Enter
```
git clone https://github.com/sa2c/AerOptGUI
cd AerOptGUI
```

The next step is to build the docker image. We can think of this as a template (created using the commands defined by Dockerfile.mxe)
that will be used to create a Linux virtual machine (docker calls this a container).

The image is setup to create a basic Linux installation along with QT and all the tools we need to compile the code.
We can create the image with with the following command. 

```
docker build -f Dockerfile.mxe -t qt/mxe:win64 .
```

Warning this command can take several hours to complete depending on your computer and network connection as it has to fetch and 
compile a of lot diferent of tools (fortunately it is only required to be run once). 

### Compiling the code
Once this is complete we now have a Docker image we can use to create a container with the following command.

```
docker run -it -v ${pwd}:/AerOptGUI --name QT-build qt/mxe:win64
```

This command should place us on a command prompt inside the container we just created. You can tell because the prompt should say:
"root@xyz123/:#" where xyz123 will be a series of random letters and numbers.

From here we simply need to run the following to compile the code.

```
	cd /AerOptGUI/
	unzip docker_libssh.zip
	cd docker_libssh
	cp libssh.a /opt/mxe/usr/x86_64-w64-mingw32.static/lib/libssh.a
	cp -r libssh /opt/mxe/usr/x86_64-w64-mingw32.static/include/libssh
	cd ..
	qmake GeneticGui-docker.pro
	make
```

Once finished we can stop the container with the command 
```
	exit
```
You should now have a static executable AerOptGUI.exe in the release directory. At this point If you ever need to re-compile
the code you can simply run the following in Powershell to restart the container.
```
docker start -i QT-build
```
Then inside the container simply run:

```
cd /AerOptGUI
qmake GeneticGui-docker.pro
make clean
make
```

Finally, if you want to delete the container simply run the following in Powershell:

```
docker rm -f QT-build
```

Or if you want to remove the image you can run:

```
docker image rm -f qt/mxe:win64
```

Note: you will need to re-run the Docker build command if you do this and then want to re-build
again at a later date. Since this, as discussed earlier this can take several hours we advice caution when doing this.

At this point you have everything you need. What follows are the alternative instructions to compile dynamically with QT Creator
should you wish to use it.

## Compiling in QT Creator

### Install Compilation Requirements

You can download the IDE on the [QT website](https://www.qt.io/download).
Choose the open source option and download the installer.
In the installer, you will need to create a QT account if you don't already have one.
When selecting components, check the following options:

[Qt 5.15.2 -> MinGW 8.1.0 x64]
[Qt 5.15.2 -> Qt Charts]
[Developer and Designer Tools -> Qt Creator 6.0.2] (automatically checked)


You will also need the ssh and ssl libraries.
On Windows, we recommend using [vcpkg](https://github.com/microsoft/vcpkg) for this. This requires Visual Studio to be installed on your machine.
You will also need to install [git](https://gitforwindows.org/), if you don't already have it.
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
AerOpt GUI requires four external libraries to run: `libssh`, `openssl`, `zlib` and `pthread`. These can be installed with the following command
```
./vcpkg install libssh:x64-windows openssl:x64-windows zlib:x64-windows pthreads:x64-windows
```

For convenience when building and testing, we recommend adding the library binaries to your system path. Open `System Properties` in the start menu, and press enter.
Click `Environment Variables`, find `path` in the second panel and double click.
Add the following lines as separate entries
```
C:\Users\USERNAME\vcpkg\packages\libssh_x64-windows\bin
C:\Users\USERNAME\vcpkg\packages\openssl-windows_x64-windows\bin
C:\Users\USERNAME\vcpkg\packages\zlib_x64-windows\bin
C:\Users\USERNAME\vcpkg\packages\pthreads_x64-windows\bin
```
replacing `USERNAME` with your actual user name. For this change to take effect, you may need to reboot.

Note that if you installed `vcpkg` in a custom location, you need to change the paths accordingly.

### Setup Project

Next, download the AerOpt GUI. Since you have git, you can use it to clone the project. Open `Git Bash` and run the command
```
git clone https://github.com/DrBenEvans/AerOptGUI.git
```

Open QT Creator, click `Open Project` and open `AerOptGUI\GeneticGui-windows.pro`, which should now be in your home directory.
The first time you do this the IDE will ask you about the configuration.
Choose the latest 64 bit MinGW version and click `configure`.

Next, open `GeneticGui-windows.pro` in the editor and scroll to the bottom.
You will see several lines including the path
```
C:\Users\USERNAME\vcpkg...
```
Replace `USERNAME` with your username on each line. If `vcpkg` is in a custom location, use that path instead.

The AerOpt GUI executable can then be constructed using the `Build` icon in the bottom left of the IDE. If you wish to modify the sources or headers, you can navigate through the project files in a panel in the top left and open them in the QT Creator editor. 

## Setup on the Cluster

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
```
Note that the current version of AerOpt includes executables without execute permissions. As a temporary fix, you can add permissions with `chmod` using the following lines
```
cd ../Executables
chmod u+x *
```
This step will be removed or automated in a future release.

Once permissions are fixed, a symbolic link needs to be created to the AerOpt_v3.5 executable. To do so, use the following lines
```
cd ..
ln -s source/AerOpt_v3.5 ../AerOpt
```

You should now be able to run jobs on the cluster using the GUI. If the job does not run on the cluster, logs can be found in `AerOpt/run_[OptimisationName]/screenlog.0` to help diagnose the problem.


## Deploying

If you followed the Docker instructions the compiled executable you created can be distributed without needing any additional libraries or tools. 
However, if you compiled the code dynamically using QT Creator you will need to do some extra work.

### Deploying for Windows with windeployqt

QT comes with a deployment tool that copies the necessary library binaries to the executable directory called `windeployqt`. This can be used through Powershell with the following commands
```
$PATH = [Environment]::GetEnvironmentVariable("PATH")
[Environment]::SetEnvironmentVariable("PATH","$PATH;C:\Qt\Tools\mingw810_64\bin;C:\Qt\5.15.2\mingw81_64\bin")
cd [AerOptGUI directory]
C:\Qt\5.15.2\mingw81_64\bin\windeployqt AerOptGui.exe
```
The above assumes QT has been installed in C:\Qt - if you have used an alternative installation path, replace the arguments in the second command accordingly.

If you have added the library paths for `openssl`, `libssh`, `zlib` and `pthread`, the application should run from Windows Explorer directly. However, you will need to copy the library files to the AerOptGui.exe directory if you wish to pass on the executable folder to others. In Powershell, this can be done with the following
```
cp "C:\Users\USERNAME\vcpkg\packages\libssh_x64-windows\bin\ssh.dll"
cp "C:\Users\USERNAME\vcpkg\packages\openssl-windows_x64-windows\bin\libcrypto-1_1-x64.dll"
cp "C:\Users\USERNAME\vcpkg\packages\openssl-windows_x64-windows\bin\libssl-1_1-x64.dll"
cp "C:\Users\USERNAME\vcpkg\packages\zlib_x64-windows\bin\zlib1.dll"
cp "C:\Users\USERNAME\vcpkg\packages\pthreads_x64-windows\bin\pthreadVC3.dll"
```
As before, replace USERNAME with your username, and amend the paths if you have vcpkg installed on a custom path.

Please note that AerOptGUI requires a set of executables (copied into `[EXECUTABLE DIRECTORY] /AerOptFiles/Executables`) and example profiles (copied into `[EXECUTABLE DIRECTORY] /AerOptFiles/profiles`) to run properly. This copying is performed on first run of the executable from the `Executables` and `ExampleProfiles` folders included in this GitHub repository. As such, it is recommended to run AerOptGUI.exe at least once on your development machine before passing the folder on to other users. No optimisation or mesh generation is required - simply open and close and the necessary files will automatically be copied.

When all libraries, executables and profiles have been set up, you should be able to simply zip the folder containing AerOptGUI, its executables and libraries and send it to any 64-bit Windows system to run. A PortableApps solution for deployment is also planned for a future release.

### Deploying using GitHub repositories

There are two deployed releases in [Jarno's github page](https://github.com/rantahar/AerOpt-gui/releases). All you need to do to run this version
of AerOpt is to donwload AerOptGui.zip, extract the file and run AerOptGui.exe in the new folder. This is much easier for a user than compiling 
QT statically.

To create a new release compile the program using `release` setting in QT Creator.
This is below the play symbol on the bottom left in QT Creator 6.0.2, but the location may differ for older versions.
When you compile the program, an executable called `AerOptGui.exe` is created in the same directory the AerOptGui folder is in.

Download the [zip archive](https://github.com/rantahar/AerOpt-gui/releases/download/0.2/AerOptGui.zip) and extract it.
The folder contains everything AerOptGui needs to run.
Replace `AerOptGui.exe` with the newly compiled executable and it should work directly.
