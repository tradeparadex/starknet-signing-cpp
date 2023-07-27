#include <chrono>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto/ecdsa.h>

#include "UtilsImpl.hpp"
#include "StarknetDomain.hpp"
#include "Auth.hpp"
#include "Order.hpp"
#include "Message.hpp"
#include "StarkCurveSigner.hpp"

namespace signer
{

namespace tests
{

TEST( Utils, getSelectorFromName )
{
    using namespace starkware;

    {
        static constexpr char const* strStarknetDomain = "StarkNetDomain(name:felt,chainId:felt,version:felt)";

        const auto res = getSelectorFromName( strStarknetDomain, strlen( strStarknetDomain ) );
        const auto expected = 0x98D1932052FC5137543DE5ED85B7A88555A4CD1FF5D5BFEDB62ED9B9A1F0DB_Z;

        EXPECT_EQ( expected, res );
    }

    {
        const auto res = strToFelt("PRIVATE_SN_POTC_GOERLI", strlen("PRIVATE_SN_POTC_GOERLI"));
        std::cout << res << std::endl;
    }

    {
        static constexpr char const* str = "3";
        const auto res = getSelectorFromName( str, strlen( str ) );
        const auto expected = 0x280e1ef1d7842f27f2e6be0972bb708b9a135c38860dbe73c27c3486c34f4de_Z;

        EXPECT_EQ( expected, res );
    }
}

signer::StarknetDomain getStarknetDomain()
{
    using namespace starkware;

    const BigInt< 4 > chainId = 0x505249564154455F534E5F504F54435F474F45524C49_Z;
    StarknetDomain domain( chainId );

    return domain;
}

TEST( StarknetDomain, pedersenEncode )
{
    using namespace starkware;

    const std::vector< BigInt< 4 > > expected = { 0x0098d1932052fc5137543de5ed85b7a88555a4cd1ff5d5bfedb62ed9b9a1f0db_Z, 0x50617261646578_Z,
        0x505249564154455f534e5f504f54435f474f45524c49_Z, 0x1_Z };

    StarknetDomain domain = getStarknetDomain();
    const auto res = domain.pedersenEncode();
    EXPECT_EQ( res.size(), expected.size() );

    for( int i = 0; i < res.size(); i++ )
    {
        EXPECT_EQ( res[ i ].ToStandardForm(), expected[ i ] );
    }
}

TEST( StarknetDomain, hash )
{
    using namespace starkware;

    StarknetDomain domain = getStarknetDomain();
    PrimeFieldElement res = domain.hash();

    EXPECT_EQ( res.ToStandardForm(), 0x6A9F6F5D2B1C9AD4528A1EB8F357FC7E51F5C01AD09120D46075023164BD1C4_Z );
}

Order getOrder(std::chrono::milliseconds timestamp)
{
    using namespace std::chrono;

    const char* strMarket = "ETH-USD-PERP";
    constexpr OrderType orderType = OrderType::Market;
    constexpr OrderSide orderSide = OrderSide::Buy;
    constexpr double size = 0.1;

    Order order(strMarket, orderSide, orderType, size);
    order.setTimestamp(timestamp);

    return order;
}

TEST( Order, hash )
{
    using namespace std::chrono;
    using namespace starkware;

    constexpr milliseconds timestamp = 1690186431033ms;

    Order order = getOrder(timestamp);
    const auto res = order.hash();
    const auto expected = 0x067a4beeeff01460f2fab511a313b2971514b0e433981cdca8ad146f7cb7191e_Z;

    EXPECT_EQ(res.ToStandardForm(), expected);
}

Auth getAuth(std::chrono::seconds now, std::chrono::seconds expiry)
{
    Auth auth;
    auth.setNow(now);
    auth.setExpiry(expiry);

    return auth;
}

TEST(Auth, hash)
{
    using namespace std::chrono;

    const seconds now = 1690186758s;
    const seconds expiry = 1690273158s;

    Auth auth = getAuth(now, expiry);
    const auto res = auth.hash();
    const auto expected = 0x0069cb5c6ea69984c70005d0a56c2198c693aade9e7611686d517e6e71ecaad3_Z;

    EXPECT_EQ(res.ToStandardForm(), expected);
}

Message getOrderMessage()
{
    using namespace std::chrono;

    constexpr milliseconds timestamp = 1690187319619ms;
    const auto address = 0x1F06D2232E3EB0D8BD4B9294A930D553F610A0D7B24BC52D9472C7BDA478927_Z;

    const PrimeFieldElement accountAddress = PrimeFieldElement::FromBigInt( address );
    Order order = getOrder(timestamp);
    StarknetDomain starknetDomain = getStarknetDomain();

    Message message( accountAddress, std::make_shared< StarknetDomain >( starknetDomain ), std::make_shared< Order >( order ) );
    return message;
}


TEST(OrderMessage, hash)
{
    Message message = getOrderMessage();
    const auto res = message.hash();
    const auto expected = 0x02ff954a62a6191411aa051588c65a4ac37690ce781bb345bb886ac947630e5b_Z;

    EXPECT_EQ(res.ToStandardForm(), expected);
}

TEST(StarkCurveSigner, signAndVerify)
{
    using namespace starkware;

    Prng prng;
    using ValueType = PrimeFieldElement::ValueType;

    // Draw test parameters.
    const auto privateKey = ValueType::RandomBigInt( &prng );

    KeyPair keyPair( privateKey );
    StarkCurveSigner signer( keyPair );

    Message message = getOrderMessage();
    const auto h = message.hash();
    const auto k = 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z;

    const Signature res = signer.signMessage( message, k );
    EXPECT_TRUE( signer.verifyEcdsa( h, res ) );
}


} // namespace tests
} // namespace signer