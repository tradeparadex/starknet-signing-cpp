# Building from zero everytime.
rm -rf build && mkdir build

cd ./libs/crypto-cpp && rm -rf build
mkdir build && cd build

# Build crypto-cpp
cmake .. && make

# Return to the root
cd ../../../
cd ./libs/curlpp

# Run conan to install libcurl
conan install .
cd ../../build

# Build whole project
cmake .. && make