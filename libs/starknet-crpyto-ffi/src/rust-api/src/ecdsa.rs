use starknet_core::{crypto::ecdsa_sign as ecdsa_sign_rs, types::FieldElement};
use starknet_crypto::{sign as sign_rs, SignError};

use crate::{
    constants::FELT_LIMBS_LEN,
    error_codes::{
        ERR_ECDSA_SIGN, ERR_INVALID_K, ERR_INVALID_MESSAGE_HASH, ERR_MESSAGE_HASH_LEN, ERR_PRIVATE_KEY_LEN, SUCCESSFUL,
    },
    utils::{copy_mont_into_raw, raw_pointer_into_felt},
};

#[no_mangle]
pub extern "C" fn sign(
    private_key: *const u64,
    private_key_len: usize,
    message_hash: *const u64,
    message_hash_len: usize,
    k: *const u64,
    k_len: usize,
    output_r: *mut u64,
    output_s: *mut u64,
) -> i32 {
    if private_key_len != FELT_LIMBS_LEN {
        return ERR_PRIVATE_KEY_LEN;
    }
    let private_key = raw_pointer_into_felt(private_key);

    if message_hash_len != FELT_LIMBS_LEN {
        return ERR_MESSAGE_HASH_LEN;
    }
    let message_hash = raw_pointer_into_felt(message_hash);

    if k_len != FELT_LIMBS_LEN {
        return ERR_MESSAGE_HASH_LEN;
    }
    let k = raw_pointer_into_felt(k);

    match sign_rs(&private_key, &message_hash, &k) {
        Ok(signature) => {
            copy_mont_into_raw(&signature.r, output_r);
            copy_mont_into_raw(&signature.s, output_s);
            SUCCESSFUL
        }
        Err(SignError::InvalidMessageHash) => ERR_INVALID_MESSAGE_HASH,
        Err(SignError::InvalidK) => ERR_INVALID_K,
    }
}

#[no_mangle]
pub extern "C" fn ecdsa_sign(
    private_key: *const u64,
    private_key_len: usize,
    message_hash: *const u64,
    message_hash_len: usize,
    output_r: *mut u64,
    output_s: *mut u64,
) -> i32 {
    if private_key_len != FELT_LIMBS_LEN {
        return ERR_PRIVATE_KEY_LEN;
    }
    let private_key = raw_pointer_into_felt(private_key);

    if message_hash_len != FELT_LIMBS_LEN {
        return ERR_MESSAGE_HASH_LEN;
    }
    let message_hash = raw_pointer_into_felt(message_hash);

    match ecdsa_sign_rs(&private_key, &message_hash) {
        Ok(signature) => {
            copy_mont_into_raw(&signature.r, output_r);
            copy_mont_into_raw(&signature.s, output_s);
            SUCCESSFUL
        }
        Err(_) => ERR_ECDSA_SIGN,
    }
}
