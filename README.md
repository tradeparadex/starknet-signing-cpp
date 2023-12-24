# starknet-signing-cpp

# Prerequisites
- CMake 3.5+ ```% cmake -version```
- C++17 toolchain
- Rust
- Python 3.x
- conan
- Installed [nlohmann_json](https://github.com/nlohmann/json/tree/develop#cmake) library

# Conan setup
In root run 
```
% sudo pip3 install -r requirements.txt
```

# Compilation
1)  Init submodule
    ```
    git submodule update --init
    ```
2)  Run
    ```
    ./build.sh <path>/install/share/cmake/nlohmann_json
    ```
3) The compiled library resides in
   ```
   project-root/build/src/libsigner.a
   ```    
# Running tests

After successful compilation executables shall be located in: `project-root/build/tests`.

- To run gtests:
```
project-root/build/tests/test
```

- To run test of client authentication request:
```
project-root/build/tests/clientTest
```

- To run cpp wrapper tests
```
project-root/build/libs/starknet-crypto-ffi/src/cpp-wrapper/tests/cppWrapperTest
```

# Third party dependencies
- keccak sourced from `https://github.com/AztecProtocol/barretenberg.git`
