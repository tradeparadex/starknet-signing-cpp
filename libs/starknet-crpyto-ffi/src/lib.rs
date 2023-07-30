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

/// Copies montgomery representation
fn copy_mont_into_raw(src: &FieldElement, dst: *mut u64) {
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

    if message_hash_len != FELT_LEN {
        return ERR_MESSAGE_HASH_LEN;
    }
    let message_hash = raw_pointer_to_felt(message_hash);

    match ecdsa_sign_rs(&private_key, &message_hash) {
        Ok(signature) => {
            copy_mont_into_raw(&signature.r, r);
            copy_mont_into_raw(&signature.s, s);
            SUCCESSFUL
        },
        Err(_) => ERR_ECDSA_SIGN,
    }
}
