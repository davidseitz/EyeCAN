@echo off
setlocal

REM Update submodules
git pull --recurse-submodules
git submodule update --init --recursive

set SVPWD=%CD%
set CMAKE_PATH="C:\Program Files\JetBrains\CLion 2024.3.1.1\bin\cmake\win\x64\bin\cmake.exe"
set MINGW_PATH=C:\MinGW\bin

REM Add MinGW to PATH
set PATH=%MINGW_PATH%;%PATH%

REM Build dbcppp
cd libraries\dbcppp
if exist build (
    cd build
    del /q CMakeCache.txt
    rmdir /s /q CMakeFiles
) else (
    mkdir build
    cd build
)
%CMAKE_PATH% -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=%USERPROFILE%\.local -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
mingw32-make install

REM Build zlib
cd /d %TEMP%
if not exist zlib-1.3.1 (
  echo Getting zlib from https://www.zlib.net/zlib-1.3.1.tar.gz
  powershell -Command "Invoke-WebRequest -Uri https://www.zlib.net/zlib-1.3.1.tar.gz -OutFile zlib-1.3.1.tar.gz"
  powershell -Command "tar -xf zlib-1.3.1.tar.gz"
)
cd zlib-1.3.1
if exist build (
    cd build
    del /q CMakeCache.txt
    rmdir /s /q CMakeFiles
) else (
    mkdir build
    cd build
)
%CMAKE_PATH% -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=%USERPROFILE%\.local -DCMAKE_C_FLAGS=-fPIC -DBUILD_SHARED_LIBS=OFF ..
mingw32-make
mingw32-make install

REM Build mdflib
cd /d %SVPWD%\libraries\mdflib
if exist build (
    cd build
    del /q CMakeCache.txt
    rmdir /s /q CMakeFiles
) else (
    mkdir build
    cd build
)
%CMAKE_PATH% -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=%USERPROFILE%\.local -DZLIB_LIBRARY=%USERPROFILE%\.local\lib\libz.a -DZLIB_INCLUDE_DIR=%USERPROFILE%\.local\include -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
mingw32-make install

cd /d %SVPWD%

echo You should now be able to build the project!

endlocal