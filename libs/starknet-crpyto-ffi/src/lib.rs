use starknet_core::{
    crypto::{ecdsa_sign as ecdsa_sign_rs, Signature},
    types::FieldElement,
};
use starknet_crypto::sign;
use ark_ff::{
    fields::{Field, Fp256, PrimeField},
    BigInteger, BigInteger256,
};
use ark_ff::BigInt;
use crypto_bigint::{U256};

// Try to get from fp;

const FELT_LEN: usize = 4;

const SUCCESSFUL: i32 = 0;
const ERR_PRIVATE_KEY_LEN: i32 = 1;
const ERR_MESSAGE_HASH_LEN: i32 = 2;
const ERR_ECDSA_SIGN: i32 = 3;

fn raw_pointer_to_felt(src: *const u64) -> FieldElement {
    let mut output: [u64; 4] = [0; 4];
    unsafe {
        std::ptr::copy(src, output.as_mut_ptr(), FELT_LEN);
    }

    FieldElement::from_mont(output)
}

fn copy_felt_into_raw(src: &FieldElement, dst: *mut u64) {
    unsafe {
        std::ptr::copy(src.into_mont().as_ptr(), dst, FELT_LEN);
    }
}

fn copy_u256_into_raw(src: &U256, dst: *mut u64) {
    unsafe {
        std::ptr::copy(src.as_words().as_ptr(), dst, FELT_LEN);
    }
}

// TODO: Passing strings will work
#[no_mangle]
pub extern "C" fn ecdsa_sign(
    private_key: *const u64,
    private_key_len: usize,
    message_hash: *const u64,
    message_hash_len: usize,
    r: *mut u64,
    s: *mut u64,
) -> i32 {
    if private_key_len != FELT_LEN {
        return ERR_PRIVATE_KEY_LEN;
    }
    let private_key = raw_pointer_to_felt(private_key);
    println!("private_key rs: {:x}", private_key);
    let private_key = FieldElement::from_hex_be("0x3c1e9550e66958296d11b60f8e8e7a7ad990d07fa65d5f7652c4a6c87d4e3cc").unwrap();

    if message_hash_len != FELT_LEN {
        return ERR_MESSAGE_HASH_LEN;
    }
    let message_hash = raw_pointer_to_felt(message_hash);
    let message_hash = FieldElement::from_hex_be("0x02ff954a62a6191411aa051588c65a4ac37690ce781bb345bb886ac947630e5b").unwrap();

    let k: FieldElement = FieldElement::from_hex_be("0x2305FBC10A7C0CDBE46A3E67DAF0B5DB99E366739B65D123C53443E78065826").unwrap();
    let fromK: U256 = From::from(&k);
    let ogK = FieldElement::from_mont(*fromK.as_words());

    let kBig: BigInteger256 = BigInt!("990095092402820057987715554077723085421189601992678519703957714492874709030");
    let asd = FieldElement::from_mont(kBig.0);
    let fromAsd: U256 = From::from(&asd);
    println!("from kBig: {} {}", asd,  fromAsd);

    let res = sign(&private_key, &message_hash, &k).unwrap();
    let resR: U256 = From::from(&res.r);
    let resS: U256 = From::from(&res.s);

    let resR1 = res.r.into_mont();
    let resS1 = res.s.into_mont();
    // match ecdsa_sign_rs(&private_key, &message_hash) {
    //     Ok(signature) => {
    //         copy_felt_into_raw(&signature.r, r);
    //         copy_felt_into_raw(&signature.s, s);
    //         SUCCESSFUL
    //     },
    //     Err(_) => ERR_ECDSA_SIGN,
    // }

    match ecdsa_sign_rs(&private_key, &message_hash) {
        Ok(signature) => {
            copy_u256_into_raw(&From::from(&signature.r), r);
            copy_u256_into_raw(&From::from(&signature.s), s);
            SUCCESSFUL
        },
        Err(_) => ERR_ECDSA_SIGN,
    }
}
