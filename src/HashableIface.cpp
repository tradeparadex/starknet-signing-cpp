#include "HashableIface.hpp"
#include "UtilsImpl.hpp"

namespace signer
{

starkware::PrimeFieldElement HashableIface::hash() const
{
    const std::vector<starkware::PrimeFieldElement> elements = this->pedersenEncode();
    return hashElements(elements);
}

}
