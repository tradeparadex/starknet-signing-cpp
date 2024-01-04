@echo off

REM Check if nlohmann_json_DIR argument is provided
if "%1"=="" (
    echo Usage: %0 ^<nlohmann_json_DIR^>
    exit /b 1
)

REM Set the provided nlohmann_json_DIR as a variable
set "nlohmann_json_DIR=%1"

REM Building from scratch every time.
rmdir /s /q build
mkdir build

REM Run conan to install libcurl
cd libs\curlpp
conan install . --build=missing -s build_type=Release
cd ..\..\build

REM Generate Visual Studio project files for the whole project
cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release -Dnlohmann_json_DIR="%nlohmann_json_DIR%" ..
REM Build the whole project using Visual Studio's MSBuild
cmake --build . --target test