#include <iostream>

#include "starkware/crypto/ecdsa.h"
#include "starkware/algebra/prime_field_element.h"
#include "starkware/crypto/elliptic_curve_constants.h"

int main()
{
    using namespace starkware;
    const BigInt<4UL> private_key = 0x3c1e9550e66958296d11b60f8e8e7a7ad990d07fa65d5f7652c4a6c87d4e3cc_Z;
    const EcPoint<PrimeFieldElement> public_key(
        PrimeFieldElement::FromBigInt(
            0x77a3b314db07c45076d11f62b6f9e748a39790441823307743cf00d6597ea43_Z),
        PrimeFieldElement::FromBigInt(
            0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z));

    if (public_key == GetPublicKey(private_key))
    {
        std::cout << "SUCCESS" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "FAILED" << std::endl;
        return 1;
    }
}