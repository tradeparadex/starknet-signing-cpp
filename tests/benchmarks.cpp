#include <iostream>
#include <starkware/algebra/prime_field_element.h>

#include "StarkCurveSigner.hpp"
#include "Order.hpp"
#include "Message.hpp"

extern "C" {
int32_t ecdsa_sign( const int64_t*, size_t, const int64_t*, size_t, int64_t*, int64_t* );
}

int main()
{
    using namespace signer;
    using namespace starkware;

    Prng prng;
    using ValueType = PrimeFieldElement::ValueType;

    // Draw test parameters.
    const auto privateKey = ValueType::RandomBigInt( &prng );

    KeyPair keyPair( privateKey );
    StarkCurveSigner signer( keyPair );

    Message message = getOrderMessage();
    const auto h = message.hash();
    const auto k = 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z;

    const Signature res = signer.signMessage( message, k );
}
