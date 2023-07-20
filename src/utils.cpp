#include "utils.hpp"
#include <numeric>

namespace signer
{

PrimeFieldElement hashElements( const std::initializer_list< PrimeFieldElement >& elements )
{
    constexpr PrimeFieldElement numInitValue = PrimeFieldElement::Zero();

    return std::accumulate( elements.begin(), elements.end(), numInitValue, []( const PrimeFieldElement& a, const PrimeFieldElement& b ) {
        return PedersenHash( a, b );
    } );
}

}
