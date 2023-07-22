#include "StarknetDomain.hpp"
#include "UtilsImpl.hpp"

namespace signer
{
using namespace starkware;

StarknetDomain::StarknetDomain( const PrimeFieldElement& theChainId )
{
    chainId = theChainId.ToStandardForm();
}

StarknetDomain::StarknetDomain( const Uint256& theChainId )
{
    chainId = theChainId;
}

PrimeFieldElement StarknetDomain::hash() const
{
    // TODO
    return PrimeFieldElement::Zero();
}

std::vector< starkware::PrimeFieldElement > StarknetDomain::pedersenEncode() const
{
    static constexpr char const* strStarknetDomain = "StarkNetDomain(name:felt,chainId:felt,version:felt)";
    static constexpr char const* strName = "Paradex";

    constexpr size_t numNameLen = std::char_traits< char >::length( strName );
    const auto tmpName = signer::strToBigInt< numNameLen >( strName );

    const BigInt< 4 > name = signer::bigIntToUint256( tmpName );
    const BigInt< 4 > typeNameHash = signer::getSelectorFromName( strStarknetDomain, strlen( strStarknetDomain ) );

    return std::vector< starkware::PrimeFieldElement >( { PrimeFieldElement::FromBigInt( typeNameHash ),
        PrimeFieldElement::FromBigInt( name ), PrimeFieldElement::FromBigInt( chainId ), PrimeFieldElement::FromBigInt( numVersionId ) } );
}

}
