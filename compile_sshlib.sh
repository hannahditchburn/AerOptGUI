#!/bin/bash
echo "Compiling ssh.lib"
cd /opt
git clone https://git.libssh.org/projects/libssh.git &&
cd libssh &&
git checkout stable-0.9 &&
mkdir build && cd build &&
x86_64-w64-mingw32.static-cmake -DWITH_EXAMPLES=OFF -DBUILD_SHARED_LIBS=OFF -DWITH_STATIC_LIB=ON .. &&
make
make install
echo "build complete"
#PATH="${PATH}:/opt/mxe/usr/bin:/opt/mxe/usr/x86_64-w64-mingw32.static/qt5/bin/"
