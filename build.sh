# Building from zero everytime.
rm -rf build && mkdir build

# Run conan to install libcurl
cd ./libs/curlpp
conan install . --build=missing -s build_type=Release

# Build whole project
cd ../../build
cmake -DCMAKE_BUILD_TYPE=Release .. && make