# starknet-signing-cpp

# Overview
## What does it do
This C++ library is used to sign requests for authentication and orders sent to Paradex.
The signature is passed with every authertication request and order and proves that whoever sent this it has access to a private key of the account.

## Why is it useful
Trading programs written in C++ provide superior performance vs other languages. For that reason most of low latency traders use C++.
This library will allow them to integrate with Paradex and improve quality of liquidity on Paradex' order books.
This in turn will increase trading volume and transactions on Paradex, StarkNet and Ethereum networks.

## What's inside
C++ library using 3rd party C++ libraries and 3rd party StarkNet Rust library: <https://github.com/xJonathanLEI/starknet-rs>.

## Roadmap
* Extension to Windows OS.
* Replacement of Rust library dependency with native C++ code.
* Implementation of generate_k from rfc6979
* Bug fixes
  
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
