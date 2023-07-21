#include <iostream>
#include <string>
#include "UtilsImpl.hpp"
#include "StarknetDomain.hpp"

#include "starkware/crypto/ecdsa.h"
#include "starkware/algebra/prime_field_element.h"
#include "starkware/crypto/pedersen_hash.h"

int dummyCryptoCppTest()
{
    std::cout << "dummyCryptoCppTest" << std::endl;

    using namespace starkware;

    const PrimeFieldElement zero = PrimeFieldElement::Zero();
    const PrimeFieldElement val = PrimeFieldElement::FromBigInt( 0x537461726B4E6574204D657373616765_Z );
    const PrimeFieldElement res = PedersenHash( zero, val );

    std::cout << "res: " << res << std::endl;
    std::cout << "my res: " << signer::hashElements( { val } ) << std::endl;

    const BigInt< 4UL > private_key = 0x3c1e9550e66958296d11b60f8e8e7a7ad990d07fa65d5f7652c4a6c87d4e3cc_Z;
    const EcPoint< PrimeFieldElement > public_key(
        PrimeFieldElement::FromBigInt( 0x77a3b314db07c45076d11f62b6f9e748a39790441823307743cf00d6597ea43_Z ),
        PrimeFieldElement::FromBigInt( 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z ) );

    if( public_key == GetPublicKey( private_key ) )
    {
        return 0;
    }
    else
    {
        return 1;
    }

}

int dummyKeccakTest()
{
    std::cout << "dummyKeccakTest" << std::endl;
    using namespace starkware;

    // 98D1932052FC5137543DE5ED85B7A88555A4CD1FF5D5BFEDB62ED9B9A1F0DB
    static constexpr char const* strStarknetDomain = "StarkNetDomain(name:felt,chainId:felt,version:felt)";
    const auto res = signer::getSelectorFromName(strStarknetDomain, strlen(strStarknetDomain));

    constexpr size_t len = std::char_traits<char>::length(strStarknetDomain);
    auto ll = signer::strToBigInt<len>(strStarknetDomain);

    std::cout << signer::getSelectorFromName( ll) << std::endl;
    std::cout << res << std::endl;

    return 0;
}

int domainHashCheck()
{
    std::cout << "domainHashCheck" << std::endl;

    using namespace starkware;

    const BigInt<4> chainId = 0x505249564154455F534E5F504F54435F474F45524C49_Z;
    signer::StarknetDomain domain(chainId);

    PrimeFieldElement res = signer::hashElements( domain.pedersenEncode());
    std::cout << res << std::endl;

    return 0;
}


int main()
{
    return dummyCryptoCppTest() | dummyKeccakTest() | domainHashCheck();
}