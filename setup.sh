git pull --recurse-submodules
git submodule update --init --recursive


#build dbcppp
#on unix
cd dbcppp
mkdir build/
cd build
cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local -DCMAKE_BUILD_TYPE=Release ..
make -j
make RunTests
make install
ldconfig # on Unix-systems only may require sudo

echo "You can now build the project!"
