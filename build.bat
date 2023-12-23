@echo off
REM Building from scratch every time.
rmdir /s /q build
mkdir build

REM Run conan to install libcurl
cd libs\curlpp
conan install . --build=missing -s compiler="Visual Studio" -s build_type=Release -s compiler.version=17
cd ..\..\build

REM Generate Visual Studio project files for the whole project
cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release ..
REM Build the whole project using Visual Studio's MSBuild
cmake --build . --target test