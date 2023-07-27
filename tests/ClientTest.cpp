#include "Account.hpp"

int main()
{
    using namespace starkware;
    using namespace signer;

    std::string url = "https://api.testnet.paradex.trade/v1/auth";
    const std::string chainId = "PRIVATE_SN_POTC_GOERLI";
    const auto address = 0x1F06D2232E3EB0D8BD4B9294A930D553F610A0D7B24BC52D9472C7BDA478927_Z;
    constexpr BigInt< 4 > privateKey = 0x3C831498D18CBD16F45F8A15FA5F08AF4B6B5D4EFDF99F7E4878C227DF86C81_Z;

    KeyPair keyPair( privateKey );
    Account account( PrimeFieldElement::FromBigInt( address ), chainId, keyPair );

    std::string response = account.getJwtToken( url );
    std::cout << response << std::endl;

    return 0;
}

