mkdir -p build
cd build
cmake ..
make
make install
cd dist/bin
./OgreApp
cd ../../..