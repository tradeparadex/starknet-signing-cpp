# Check if nlohmann_json_DIR argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <nlohmann_json_DIR>"
    exit 1
fi

nlohmann_json_DIR="$1"

# Building from zero everytime.
rm -rf build && mkdir build

# Run conan to install libcurl
cd ./libs/curlpp
conan install . --build=missing -s build_type=Release

# Build whole project
cd ../../build
cmake -DCMAKE_BUILD_TYPE=Release -Dnlohmann_json_DIR=$nlohmann_json_DIR .. && make