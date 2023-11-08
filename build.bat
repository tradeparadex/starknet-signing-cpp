@echo off
REM Building from scratch every time.
rmdir /s /q build
mkdir build

cd libs\crypto-cpp
rmdir /s /q build
mkdir build
cd build

REM Generate Visual Studio project files for crypto-cpp
cmake -G "Visual Studio" -DCMAKE_BUILD_TYPE=Release ..
REM Build the crypto-cpp project using Visual Studio's MSBuild
cmake --build .
REM Return to the root
cd ..\..\

cd libs\curlpp

REM Run conan to install libcurl
@REM C:\Users\edwin\AppData\Local\Programs\Python\Python39\Scripts\conan.exe install . --build=missing
conan install . --build=missing


cd ..\..\build

REM Generate Visual Studio project files for the whole project
cmake -G "Visual Studio" -DCMAKE_BUILD_TYPE=Release ..
REM Build the whole project using Visual Studio's MSBuild
cmake --build .