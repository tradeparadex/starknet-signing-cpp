#include <starkware/algebra/prime_field_element.h>

#include "TestUtils.hpp"

namespace signer
{

StarknetDomain getStarknetDomain()
{
    using namespace starkware;

    const BigInt< 4 > chainId = 0x505249564154455F534E5F504F54435F474F45524C49_Z;
    StarknetDomain domain( chainId );

    return domain;
}

Order getOrder( std::chrono::milliseconds timestamp )
{
    using namespace std::chrono;

    const char* strMarket = "ETH-USD-PERP";
    constexpr OrderType orderType = OrderType::Market;
    constexpr OrderSide orderSide = OrderSide::Buy;
    constexpr double size = 0.1;

    Order order( strMarket, orderSide, orderType, size );
    order.setTimestamp( timestamp );

    return order;
}

Auth getAuth( std::chrono::seconds now, std::chrono::seconds expiry )
{
    Auth auth;
    auth.setNow( now );
    auth.setExpiry( expiry );

    return auth;
}

Message getOrderMessage()
{
    using namespace std::chrono;
    using namespace starkware;

    constexpr milliseconds timestamp = 1690187319619ms;
    const auto address = 0x1F06D2232E3EB0D8BD4B9294A930D553F610A0D7B24BC52D9472C7BDA478927_Z;

    const starkware::PrimeFieldElement accountAddress = starkware::PrimeFieldElement::FromBigInt( address );
    Order order = getOrder(timestamp);
    StarknetDomain starknetDomain = getStarknetDomain();

    Message message( accountAddress, std::make_shared< StarknetDomain >( starknetDomain ), std::make_shared< Order >( order ) );
    return message;
}

}