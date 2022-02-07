#!/bin/bash
echo "Moving to home directory..."
cd %userprofile%
echo "Downloading vcpkg for library installation..."
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
cd vcpkg
echo "Installing libssh, openssl, zlib and pthread libraries for Windows with vcpkg..."
./vcpkg install libssh:x64-windows openssl:x64-windows zlib:x64-windows pthread:x64-windows