git pull --recurse-submodules
git submodule update --init --recursive

SVPWD=${PWD}
#build dbcppp
#on unix
cd dbcppp
mkdir build/
cd build
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local -DCMAKE_BUILD_TYPE=Release ..
make -j
make RunTests
make install

#build mdflib
#install zlib made with fPIC option
cd /tmp
wget https://www.zlib.net/zlib-1.3.1.tar.gz
tar xf zlib-1.3.1.tar.gz
cd zlib-1.3.1
CFLAGS="-fPIC" ./configure --prefix=$HOME/.local --static
make -j$(nproc)
make install
#now to mdflib
cd ${SVPWD}/mdflib
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local -DZLIB_LIBRARY=$HOME/.local/lib/libz.a -DZLIB_INCLUDE_DIR=$HOME/.local/include -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
make install

cd ${SVPWD}

echo "You should now be able to build the project!"
