#include <chrono>
#include <vector>

#include "gtest/gtest.h"
#include <starkware/algebra/prime_field_element.h>
#include <starkware/crypto/ecdsa.h>

#include "TestUtils.hpp"
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

#define BENCHMARK_FUNCTION(func, ...)                             \
    auto start_time = std::chrono::high_resolution_clock::now(); \
    auto funcRes = func(__VA_ARGS__);                                        \
    auto end_time = std::chrono::high_resolution_clock::now();   \
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count(); \
    std::cout << "Execution time: " << duration << " microseconds" << std::endl; \

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

    // Draw test parameters.
    const auto privateKey = 0x3c1e9550e66958296d11b60f8e8e7a7ad990d07fa65d5f7652c4a6c87d4e3cc_Z;

    KeyPair keyPair( privateKey );
    StarkCurveSigner signer( keyPair );

    Message message = getOrderMessage();
    const auto h = message.hash();
    const auto k = 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z;

    const Signature res = signer.signMessage( message, k );
    EXPECT_TRUE( signer.verifyEcdsa( h, res ) );
}

TEST(StarkCurveSigner, signSpeedTest)
{
    using namespace starkware;

    const auto privateKey = 0x3c1e9550e66958296d11b60f8e8e7a7ad990d07fa65d5f7652c4a6c87d4e3cc_Z;
    const EcPoint< PrimeFieldElement > publicKey(
        PrimeFieldElement::FromBigInt( 0x77a3b314db07c45076d11f62b6f9e748a39790441823307743cf00d6597ea43_Z ),
        PrimeFieldElement::FromBigInt( 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z ) );

    KeyPair keyPair( privateKey, publicKey );
    StarkCurveSigner signer( keyPair );

    Message message = getOrderMessage();
    const auto k = 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z;

    signer.signMessage( message, k );
    EXPECT_TRUE(true);
}

TEST(Performance, test)
{
    using namespace std::chrono;
    Prng prng;
    const BigInt<2> privateKey = BigInt<2>::RandomBigInt( &prng );
    KeyPair keyPair( privateKey );
    StarknetDomain starknetDomain(std::string("PRIVATE_SN_POTC_GOERLI"));
    StarkCurveSigner signer( keyPair );
    const auto address = 0x1F06D2232E3EB0D8BD4B9294A930D553F610A0D7B24BC52D9472C7BDA478927_Z;
    const PrimeFieldElement accountAddress = PrimeFieldElement::FromBigInt( address );

    auto ts_start = std::chrono::high_resolution_clock::now();
    auto order = signer::Order("ETH-USD-PERP", OrderSide::Buy, OrderType::Limit, 0.1, Uint256(189000000000));
    auto ts_order = std::chrono::high_resolution_clock::now();
    std::cout << "order:" << duration_cast<microseconds>(ts_order - ts_start).count()  << " micros" << std::endl;
    Message message( accountAddress, std::make_shared< StarknetDomain >( starknetDomain ), std::make_shared< Order >( order ) );
    auto ts_msg = std::chrono::high_resolution_clock::now();
    std::cout << "message:" << duration_cast<microseconds>(ts_msg - ts_order).count()  << " micros" << std::endl;
    const auto h = message.hash();
    auto ts_msg_hash = high_resolution_clock::now();
    std::cout << "message hash:" << duration_cast<microseconds>(ts_msg_hash - ts_msg).count()  << " micros" << std::endl;
    
    const auto k = 0x54d7beec5ec728223671c627557efc5c9a6508425dc6c900b7741bf60afec06_Z;
    const Signature res = signer.signMessage( message, k );
    auto ts_signature = high_resolution_clock::now();
    std::cout << "signature:" << duration_cast<microseconds>(ts_signature - ts_msg_hash).count()  << " micros" << std::endl;
    std::cout << "total time:" << duration_cast<microseconds>(ts_signature - ts_start).count()  << " micros" << std::endl;
}

} // namespace tests
} // namespace signer