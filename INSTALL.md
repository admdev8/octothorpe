Requirements
============

C11 compiler, like GCC >= 4.7.

Recommended build
=================

    mkdir build
    cd build
    ../configure
    make
    make check
    [sudo] make install

Thus, the headers files are goes to **/usr/local/include/octothorpe** and library file to **/usr/local/lib**.

Debug build
===========

    mkdir build
    cd build
    ../configure --enable-debug=yes
    make
    make check
    [sudo] make install

MinGW
=====

MinGW installation instructions:

Install MSYS (x86) only
-----------------------
... from [sourceforge](http://sourceforge.net/projects/mingw/files/)

You may also need to install **msys-mktemp**, **msys-bison**, **msys-flex**, **msys-autoconf**, **msys-autogen** and **msys-automake** packages.

Install MinGW (x86)
-------------------
... from [sourceforge](http://sourceforge.net/projects/mingw-w64/files/)

Fix **C:\MinGW\msys\1.0\etc\fstab** file:

    c:/mingw/    /mingw

Fix **C:\MinGW\msys\1.0\home\Administrator\.bash_profile** file:

    export PATH=$PATH:/mingw/bin

Now GCC binaries from **/mingw** are available from MSYS environment.

Install MinGW (x64)
-------------------
... from [sourceforge](http://sourceforge.net/projects/mingw-w64/files/)

x64 version of MinGW can be installed to, let's say, **c:\mingw64**. 
MSYS from x86 version could be copied right into **c:\mingw64\msys**.
**fstab** file there is to be fixed as well, so x64-version of GCC will be invoked automatically.

Windows 2000
============
While compiling for Windows 2000, set **TARGET_IS_WINDOWS_2000**.
