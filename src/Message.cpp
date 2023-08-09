#include <cstring>

#include "UtilsImpl.hpp"
#include "Message.hpp"

namespace signer
{

Message::Message( PrimeFieldElement theAccountAddress, HashableIface::Pointer theDomain, HashableIface::Pointer thePrimaryType )
    : primaryType( thePrimaryType )
    , domain( theDomain )
    , accountAddress( theAccountAddress )
{
}

std::vector< starkware::PrimeFieldElement > Message::pedersenEncode() const
{
    using namespace starkware;

    const char* strStarknetMessage = "StarkNet Message";

    const PrimeFieldElement starknetMessage = strToFelt( strStarknetMessage, std::strlen( strStarknetMessage ) );
    const PrimeFieldElement domainHash = domain->hash();
    const PrimeFieldElement primaryTypeHash = primaryType->hash();

    return { starknetMessage, domainHash, accountAddress, primaryTypeHash };
}

} // namespace signer
