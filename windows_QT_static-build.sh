#!/bin/bash
# usage: ./Windows_QT_staic_build.sh 
# Optional arguments: Install_directory
NPROC=$(nproc)
echo "Seting up MXE to cross-compile QT for Windows-64 bit"
# sets the install directory to /opt by default or the directory that is passed in as an optional argument. 
InstDIR=${1:-/opt}
echo $InstDIR
cd $InstDIR &&
# Grabs the latest version of mxe from git hub
git clone https://github.com/mxe/mxe.git &&
cd ${InstDIR}/mxe &&
# use the mxe build scripts to crosscompile: QT, openssl, zlib, cmake and gnu compilers for windows 64 bit.
make --jobs=$NPROC JOBS=$NPROC MXE_TARGETS='x86_64-w64-mingw32.static' qtbase qtcharts &&
# add the binaries to the local path
PATH=${InstDIR}/mxe/usr/bin:${InstDIR}/mxe/usr/x86_64-w64-mingw32.static/qt5/bin/:$PATH
# test qmake is working correctly
qmake --version &&
echo "Compiling ssh.lib" &&
cd $InstDIR &&
#grab the source for sshlib from there git repo and crosscompile it using the windows 64 bit cmake provided by mxe.
git clone https://git.libssh.org/projects/libssh.git &&
cd libssh &&
git checkout stable-0.8
mkdir build && cd build &&
x86_64-w64-mingw32.static-cmake -DWITH_EXAMPLES=OFF -DBUILD_SHARED_LIBS=OFF -DWITH_STATIC_LIB=ON .. &&
make -j$NPROC &&
echo "build complete"
