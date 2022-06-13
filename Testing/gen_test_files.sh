#!/bin/bash
cd fake-ssh-home
mkdir FolderO FolderA FolderB FolderAB FolderABC
echo "This file is for testing purposes." >> FolderA/File1.txt
echo "This file is also for testing." >> FolderB/File2.txt
echo "This file is also also for testing." >> FolderABC/File3.txt
cp FolderA/File1.txt FolderAB/.
cp FolderA/File1.txt FolderABC/.
cp FolderB/File2.txt FolderAB/.
cp FolderB/File2.txt FolderABC/.

