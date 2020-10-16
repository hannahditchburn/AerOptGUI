#!/bin/bash
echo "Compiling ssh.lib"
git clone https://git.libssh.org/projects/libssh.git &&
cd libssh &&
git checkout libssh-0.9.4 &&
mkdir -p build && cd build &&
x86_64-w64-mingw32.static-cmake -DWITH_EXAMPLES=OFF -DBUILD_SHARED_LIBS=OFF -DWITH_STATIC_LIB=ON -DOPENSSL_ROOT_DIR=opt/mxe/usr/x86_64-w64-mingw32.static/lib .. &&
make clean
make && make install
echo "complete"
#PATH="${PATH}:/opt/mxe/usr/bin:/opt/mxe/usr/x86_64-w64-mingw32.static/qt5/bin/"
