git pull --recurse-submodules
git submodule update --init --recursive

SVPWD=${PWD}
#build dbcppp
#on unix
cd libraries/dbcppp
mkdir build/
cd build
cmake -Dbuild_tools=OFF -Dbuild_examples=OFF -Dbuild_tests=OFF -Dbuild_kcd=OFF -DCMAKE_INSTALL_PREFIX=$HOME/.local -DCMAKE_BUILD_TYPE=Release .. 
make -j
make RunTests
make install

#build mdflib
#install zlib made with fPIC option
cd /tmp

if [ "$(uname)" != "Darwin" ]; then
  # Non-macOS (Linux/Unix) installation logic
  if [ ! -d "/tmp/zlib-1.3.1/" ]; then
    echo "Getting zlib from https://www.zlib.net/zlib-1.3.1.tar.gz"
    wget https://www.zlib.net/zlib-1.3.1.tar.gz
    tar xf zlib-1.3.1.tar.gz
  fi
  cd zlib-1.3.1
  CFLAGS="-fPIC" ./configure --prefix=$HOME/.local --static
  make -j$(nproc)
  make install
else
  # macOS installation logic using Homebrew
  echo "Detected macOS. Installing zlib via Homebrew..."
  brew install zlib
  brew install expat
  export LDFLAGS="-L/opt/homebrew/opt/zlib/lib"
  export CPPFLAGS="-I/opt/homebrew/opt/zlib/include"
  export LDFLAGS="-L/opt/homebrew/opt/expat/lib"
  export CPPFLAGS="-I/opt/homebrew/opt/expat/include"

fi
#now to mdflib
cd ${SVPWD}/libraries/mdflib
mkdir build
cd build

if [ "$(uname)" = "Linux" ]; then
  cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local -DZLIB_LIBRARY=$HOME/.local/lib/libz.a -DZLIB_INCLUDE_DIR=$HOME/.local/include -DCMAKE_BUILD_TYPE=Release ..
elif [ "$(uname)" = "Darwin" ]; then
  # macOS message
  cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local -DEXPAT_LIBRARY=/opt/homebrew/opt/expat/lib/libexpat.a -DEXPAT_INCLUDE_DIR=/opt/homebrew/opt/expat/include -DZLIB_LIBRARY=/opt/homebrew/opt/zlib/lib/libz.a -DZLIB_INCLUDE_DIR=/opt/homebrew/opt/zlib/include -DCMAKE_BUILD_TYPE=Release ..
else
  # Other operating systems
  echo "Unsupported operating system: $(uname)"
fi

make -j$(nproc)
make install

cd ${SVPWD}

echo "You should now be able to build the project!"

mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=$HOME/.local -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
make install
make RunTests


