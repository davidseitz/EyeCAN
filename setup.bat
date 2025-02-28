@echo off
setlocal

REM Update submodules
git pull --recurse-submodules
git submodule update --init --recursive

set SVPWD=%CD%
set CMAKE_PATH="C:\Program Files\JetBrains\CLion 2024.3.1.1\bin\cmake\win\x64\bin\cmake.exe"

REM Build dbcppp
cd libraries\dbcppp
if not exist build mkdir build
cd build
%CMAKE_PATH% -DCMAKE_INSTALL_PREFIX=%USERPROFILE%\.local -DCMAKE_BUILD_TYPE=Release ..
%CMAKE_PATH% --build . --config Release
%CMAKE_PATH% --build . --config Release --target install

REM Build zlib
cd /d %TEMP%
if not exist zlib-1.3.1 (
  echo Getting zlib from https://www.zlib.net/zlib-1.3.1.tar.gz
  powershell -Command "Invoke-WebRequest -Uri https://www.zlib.net/zlib-1.3.1.tar.gz -OutFile zlib-1.3.1.tar.gz"
  powershell -Command "tar -xf zlib-1.3.1.tar.gz"
)
cd zlib-1.3.1
%CMAKE_PATH% -DCMAKE_INSTALL_PREFIX=%USERPROFILE%\.local -DCMAKE_C_FLAGS=-fPIC -DBUILD_SHARED_LIBS=OFF .
%CMAKE_PATH% --build . --config Release
%CMAKE_PATH% --build . --config Release --target install

REM Build mdflib
cd /d %SVPWD%\libraries\mdflib
if not exist build mkdir build
cd build
%CMAKE_PATH% -DCMAKE_INSTALL_PREFIX=%USERPROFILE%\.local -DZLIB_LIBRARY=%USERPROFILE%\.local\lib\zlib.lib -DZLIB_INCLUDE_DIR=%USERPROFILE%\.local\include -DCMAKE_BUILD_TYPE=Release ..
%CMAKE_PATH% --build . --config Release
%CMAKE_PATH% --build . --config Release --target install

cd /d %SVPWD%

echo You should now be able to build the project!

endlocal