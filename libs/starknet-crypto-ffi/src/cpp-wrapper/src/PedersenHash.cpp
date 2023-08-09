#include "PedersenHash.hpp"
#include "ApiException.hpp"

namespace StarkwareCppWrapper
{

starkware::PrimeFieldElement PedersenHash::pedersenHash( const starkware::PrimeFieldElement& x, const starkware::PrimeFieldElement& y )
{
    using namespace starkware;

    // TODO: replace 4 with const
    const std::array< uint64_t, 4 > rawX = x.ToMont().ToLimbs();
    const std::array< uint64_t, 4 > rawY = y.ToMont().ToLimbs();

    std::array< uint64_t, 4 > rawHash = { 0, 0, 0, 0 };
    int code = pedersen_hash( rawX.data(), 4, rawY.data(), 4, rawHash.data() );
    apiCheckResult(code);

    return PrimeFieldElement::FromMont( BigInt( rawHash ) );
}

}