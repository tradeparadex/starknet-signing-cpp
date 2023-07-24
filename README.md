# starknet-signing-cpp

# Prerequisites
- CMake 3.5+ 
- C++17 toolchain
- Python 3.x
- conan

# Conan setup
In root run 
```
pip install -r requirments.txt
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

# Third party dependencies
- keccak sourced from `https://github.com/AztecProtocol/barretenberg.git`