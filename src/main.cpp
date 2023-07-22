#include <iostream>
#include <string>
#include "UtilsImpl.hpp"
#include "StarknetDomain.hpp"
#include "Order.hpp"

#include "starkware/crypto/ecdsa.h"
#include "starkware/algebra/prime_field_element.h"
#include "starkware/crypto/pedersen_hash.h"

int dummyKeccakTest()
{
    std::cout << "dummyKeccakTest" << std::endl;
    using namespace starkware;

    // 98D1932052FC5137543DE5ED85B7A88555A4CD1FF5D5BFEDB62ED9B9A1F0DB
    static constexpr char const* strStarknetDomain = "StarkNetDomain(name:felt,chainId:felt,version:felt)";
    const auto res = signer::getSelectorFromName(strStarknetDomain, strlen(strStarknetDomain));
    std::cout << res << std::endl;

    return 0;
}

int domainHashCheck()
{
    std::cout << "domainHashCheck" << std::endl;

    using namespace starkware;

    const BigInt< 4 > chainId = 0x505249564154455F534E5F504F54435F474F45524C49_Z;
    signer::StarknetDomain domain( chainId );

    // 0x6A9F6F5D2B1C9AD4528A1EB8F357FC7E51F5C01AD09120D46075023164BD1C4
    PrimeFieldElement res = signer::hashElements( domain.pedersenEncode() );
    std::cout << res << std::endl;

    return 0;
}

int orderHashCheck()
{
    std::cout << "orderHashCheck" << std::endl;

    using namespace starkware;
    using namespace signer;
    using namespace std::chrono;

    const char* strMarket = "ETH-USD-PERP";
    constexpr OrderType orderType = OrderType::Market;
    constexpr OrderSide orderSide = OrderSide::Buy;
    constexpr double size = 0.1;

    Order order(strMarket, orderSide, orderType, size);
    order.setTimestamp(1690026783853ms);
    const auto encoded = order.pedersenEncode();

    // 0x059b1e79c5b2a6fc4df453515ebeceb51532ed20104c67ed3414b6671f1769a7
    const PrimeFieldElement hash = hashElements(encoded);
    std::cout << "order hash: " << hash << std::endl;

    return 0;
}

int main()
{
    return dummyKeccakTest() | domainHashCheck() | orderHashCheck();
}