#include <chrono>
#include <vector>
#include <iostream>
#include <string>

#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto/ecdsa.h>

#include "Utils.hpp"
// #include "UtilsImpl.hpp"
// #include "StarknetDomain.hpp"
// #include "Auth.hpp"
// #include "Order.hpp"
// #include "Message.hpp"
// #include "StarkCurveSigner.hpp"

int main()
{
    using namespace starkware;
    using namespace signer;
    std::string privateKey, address;

    std::cout << "Please enter SN private key:";
    std::getline(std::cin, privateKey);
    std::cout << "Please enter SN address:";
    std::getline(std::cin, address);
    const auto pk_int = strToBigInt( privateKey.c_str() );
    const auto ad_int = strToBigInt( address.c_str() );
    std::cout << "private key int:" << pk_int << std::endl;
    std::cout << "address int:" << ad_int << std::endl;

    return 0;
}

