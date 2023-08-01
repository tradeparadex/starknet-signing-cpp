# starknet-signing-cpp

# Prerequisites
- CMake 3.5+ ```% cmake -version```
- C++17 toolchain
- Rust
- Python 3.x
- conan

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
    ./build.sh
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
project-root/build/libs/starknet-crpyto-ffi/src/cpp-wrapper/tests/cppWrapperTest
```

# Third party dependencies
- keccak sourced from `https://github.com/AztecProtocol/barretenberg.git`
