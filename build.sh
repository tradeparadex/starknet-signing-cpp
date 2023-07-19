rm -rf build && mkdir build
cd ./libs/crypto-cpp && rm -rf build
mkdir build && cd build
cmake .. && make
cd ../../../build
cmake .. && make