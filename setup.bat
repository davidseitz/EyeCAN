@echo off

:: Pull the latest changes and initialize submodules
git pull --recurse-submodules
git submodule update --init --recursive

:: Define the install directory for Windows (change this to your preferred location)
set INSTALL_DIR=C:\Users\robin\local

:: Build dbcppp
cd libraries\dbcppp
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% -DCMAKE_BUILD_TYPE=Release ..
make -j
make RunTests
make install

:: Build zlib for Windows (using MinGW for example)
cd C:\tmp
if not exist "C:\tmp\zlib-1.3.1\" (
    echo Getting zlib from https://www.zlib.net/zlib-1.3.1.tar.gz
    powershell -Command "Invoke-WebRequest https://www.zlib.net/zlib-1.3.1.tar.gz -OutFile zlib-1.3.1.tar.gz"
    tar xf zlib-1.3.1.tar.gz
)
cd zlib-1.3.1
set CFLAGS=-fPIC
./configure --prefix=%INSTALL_DIR% --static
make -j
make install

:: Now build mdflib
cd ..\..\..\libraries\mdflib
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% -DZLIB_LIBRARY=%INSTALL_DIR%\lib\libz.a -DZLIB_INCLUDE_DIR=%INSTALL_DIR%\include -DCMAKE_BUILD_TYPE=Release ..
make -j
make install

cd %SVPWD%

echo You should now be able to build the project!
