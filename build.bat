@echo off
REM Building from scratch every time.
rmdir /s /q build
mkdir build

REM Run conan to install libcurl
cd libs\curlpp
conan install . --build=missing --s build_type=Release
cd ..\..\build

REM Generate Visual Studio project files for the whole project
cmake -G "Visual Studio" -DCMAKE_BUILD_TYPE=Release ..
REM Build the whole project using Visual Studio's MSBuild
cmake --build .