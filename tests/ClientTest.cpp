#include "Account.hpp"

int main()
{
    using namespace starkware;
    using namespace signer;

    const auto address = 0x1F06D2232E3EB0D8BD4B9294A930D553F610A0D7B24BC52D9472C7BDA478927_Z;
    const BigInt< 4 > chainId = 0x505249564154455F534E5F504F54435F474F45524C49_Z;
    constexpr BigInt< 4 > privateKey = 0x3C831498D18CBD16F45F8A15FA5F08AF4B6B5D4EFDF99F7E4878C227DF86C81_Z;
    constexpr BigInt< 4 > publicKey = 0x1F040087818CE0C7B85C0219232ED47D7EF18DE83B25F287DA54F7A7A1009D8_Z;

    KeyPair keyPair(privateKey, PrimeFieldElement::FromBigInt(publicKey));

    Account account(PrimeFieldElement::FromBigInt(address), chainId, keyPair);

    std::string response = account.getJwtToken();
    std::cout << response << std::endl;

    return 0;
}

