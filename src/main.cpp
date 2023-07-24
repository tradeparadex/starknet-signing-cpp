//#include <iostream>
//#include <string>
//#include <chrono>
//#include "UtilsImpl.hpp"
//#include "StarknetDomain.hpp"
//#include "Order.hpp"
//#include "Message.hpp"
//#include "StarkCurveSigner.hpp"
//#include "Auth.hpp"
//
//#include "starkware/crypto/ecdsa.h"
//#include "starkware/algebra/prime_field_element.h"
//#include "starkware/algebra/big_int.h"
//#include "starkware/crypto/pedersen_hash.h"
//#include "starkware/crypto//elliptic_curve_constants.h"
//
//signer::StarknetDomain getDomain()
//{
//    using namespace starkware;
//
//    const BigInt< 4 > chainId = 0x505249564154455F534E5F504F54435F474F45524C49_Z;
//    signer::StarknetDomain domain( chainId );
//
//    return domain;
//}
//
//int domainHashCheck()
//{
//    std::cout << "domainHashCheck" << std::endl;
//
//    using namespace starkware;
//
//    signer::StarknetDomain domain = getDomain();
//
//    // 0x6A9F6F5D2B1C9AD4528A1EB8F357FC7E51F5C01AD09120D46075023164BD1C4
//    PrimeFieldElement res = signer::hashElements( domain.pedersenEncode() );
//    std::cout << res << std::endl;
//
//    return 0;
//}
//
//signer::Order getOrder()
//{
//    using namespace signer;
//    using namespace std::chrono;
//
//    const char* strMarket = "ETH-USD-PERP";
//    constexpr OrderType orderType = OrderType::Market;
//    constexpr OrderSide orderSide = OrderSide::Buy;
//    constexpr double size = 0.1;
//
//    Order order(strMarket, orderSide, orderType, size);
//    order.setTimestamp(1690176595470ms);
//
//    return order;
//}
//
//
//int orderHashCheck()
//{
//    std::cout << "orderHashCheck" << std::endl;
//
//    using namespace starkware;
//    using namespace signer;
//
//    Order order = getOrder();
//    const auto encoded = order.pedersenEncode();
//
//    // 0x059b1e79c5b2a6fc4df453515ebeceb51532ed20104c67ed3414b6671f1769a7
//    const PrimeFieldElement hash = hashElements( encoded );
//    std::cout << "order hash: " << hash << std::endl;
//
//    return 0;
//}
//
//signer::Message getMessage()
//{
//    using namespace starkware;
//    using namespace signer;
//
//    StarknetDomain starknetDomain = getDomain();
//    Order order = getOrder();
//    const auto address = 0x1F06D2232E3EB0D8BD4B9294A930D553F610A0D7B24BC52D9472C7BDA478927_Z;
//    const PrimeFieldElement accountAddress = PrimeFieldElement::FromBigInt( address );
//
//    return Message( accountAddress, std::make_shared< StarknetDomain >( starknetDomain ), std::make_shared< Order >( order ) );
//}
//
//int messageHashCheck()
//{
//    std::cout << "messageHashCheck" << std::endl;
//
//    using namespace starkware;
//    using namespace signer;
//
//    Message message = getMessage();
//    // With current fixed timestamp 1690093693471ms
//    // and address 0x1F06D2232E3EB0D8BD4B9294A930D553F610A0D7B24BC52D9472C7BDA478927
//    // 0x07e6f70ab4abb4cd996472c715a665dd4099fd20f4e3a0da522195df672742ed
//    auto h = message.hash();
//    std::cout << h << std::endl;
//
//    return 0;
//}
//
//signer::Auth getAuth()
//{
//    using namespace std::chrono;
//    using namespace signer;
//    Auth auth;
//
//    const seconds now = 1690124303s;
//    const seconds expiry = 1690210703s;
//
//    auth.setNow(now);
//    auth.setExpiry(expiry);
//
//    return auth;
//}
//
//signer::Message getAuthMessage()
//{
//    using namespace starkware;
//    using namespace signer;
//
//    StarknetDomain starknetDomain = getDomain();
//    Auth auth = getAuth();
//    const auto address = 0x1F06D2232E3EB0D8BD4B9294A930D553F610A0D7B24BC52D9472C7BDA478927_Z;
//    const PrimeFieldElement accountAddress = PrimeFieldElement::FromBigInt( address );
//
//    return Message( accountAddress, std::make_shared< StarknetDomain >( starknetDomain ), std::make_shared< Auth >( auth ) );
//}
//
////int signAuthMessageCheck()
////{
////    std::cout << "signAuthMessageCheck" << std::endl;
////
////    using namespace starkware;
////    using namespace signer;
////
////    constexpr BigInt< 4 > privateKey = 0x3C831498D18CBD16F45F8A15FA5F08AF4B6B5D4EFDF99F7E4878C227DF86C81_Z;
////    constexpr BigInt< 4 > publicKey = 0x1F040087818CE0C7B85C0219232ED47D7EF18DE83B25F287DA54F7A7A1009D8_Z;
////
////    Message message = getAuthMessage();
////    auto h = message.hash();
////    std::cout << h << std::endl;
////
////    KeyPair keyPair( PrimeFieldElement::FromBigInt( privateKey ), PrimeFieldElement::FromBigInt( publicKey ) );
////    StarkCurveSigner starkSigner( keyPair );
////
////    const auto sig = starkSigner.signMessage( message, 0x7d2e89427a362516977d40be70c3890d849c5290bf71e76276350fcfac05532_Z );
////
////    std::cout << "sig: " << sig.first << " " << sig.second << std::endl;
////
////    return 0;
////}
//
////int signMessageCheck()
////{
////    std::cout << "signMessageCheck" << std::endl;
////    using namespace starkware;
////    using namespace signer;
////
////    constexpr BigInt< 4 > privateKey = 0x3C831498D18CBD16F45F8A15FA5F08AF4B6B5D4EFDF99F7E4878C227DF86C81_Z;
////    constexpr BigInt< 4 > publicKey = 0x1F040087818CE0C7B85C0219232ED47D7EF18DE83B25F287DA54F7A7A1009D8_Z;
////
////    Message message = getMessage();
////    const auto h = message.hash();
////    std::cout << "h: " << message.hash() << std::endl;
////    KeyPair keyPair( PrimeFieldElement::FromBigInt( privateKey ), PrimeFieldElement::FromBigInt( publicKey ) );
////    StarkCurveSigner starkSigner( keyPair );
////    auto sig = starkSigner.signMessage( message, 0x4e15da4575026159a4ea01ebe78356342ae05f0aff615d217991c3562da7b7e_Z );
////
////    std::cout << "sig: " << sig.first << " " << sig.second << std::endl;
////    std::cout << VerifyEcdsa( GetPublicKey( privateKey ), h, sig ) << std::endl;
////    std::cout << sig.second.ToStandardForm().InvModPrime(GetEcConstants().k_order);
////    //sig.second = PrimeFieldElement::FromBigInt( 0x50A393A9CEA6AB144FE260E2304511E8374B979CE307F3E810E5002013B431_Z );
////    //std::cout << VerifyEcdsa( GetPublicKey( privateKey ), h, sig ) << std::endl;
////
////    return 0;
////}
//
////int main()
////{
////    return signMessageCheck(); // | dummyKeccakTest() | domainHashCheck() | orderHashCheck();
////}