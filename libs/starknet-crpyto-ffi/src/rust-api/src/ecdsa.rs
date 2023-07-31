use starknet_core::{
    crypto::{ecdsa_sign as ecdsa_sign_rs, ecdsa_verify as ecdsa_verify_rs},
};
use starknet_crypto::{sign as sign_rs, SignError, Signature};

use crate::{
    constants::FELT_LIMBS_LEN,
    error_codes::{
        ERR_ECDSA_SIGN, ERR_INVALID_K, ERR_INVALID_MESSAGE_HASH, ERR_MESSAGE_HASH_LEN, ERR_NULLPTR_K,
        ERR_NULLPTR_MESSAGE_HASH, ERR_NULLPTR_OUTPUT, ERR_NULLPTR_PRIVATE_KEY, ERR_NULLPTR_PUBLIC_KEY, ERR_NULLPTR_R,
        ERR_NULLPTR_S, ERR_PRIVATE_KEY_LEN, ERR_PUBLIC_KEY_LEN, ERR_R_LEN, ERR_S_LEN, ERR_VERIFY_FAILED, SUCCESSFUL,
    },
    utils::{copy_mont_into_raw, raw_pointer_into_felt},
    validate_ptr,
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
    validate_ptr!(private_key, ERR_NULLPTR_PRIVATE_KEY);
    validate_ptr!(message_hash, ERR_NULLPTR_MESSAGE_HASH);
    validate_ptr!(k, ERR_NULLPTR_K);
    // validate_ptr!(output_r, ERR_NULLPTR_OUTPUT);
    // validate_ptr!(output_s, ERR_NULLPTR_OUTPUT);

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
    validate_ptr!(private_key, ERR_NULLPTR_PRIVATE_KEY);
    validate_ptr!(message_hash, ERR_NULLPTR_MESSAGE_HASH);
    // validate_ptr!(output_r, ERR_NULLPTR_OUTPUT);
    // validate_ptr!(output_s, ERR_NULLPTR_OUTPUT);

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

#[no_mangle]
pub extern "C" fn ecdsa_verify(
    public_key: *const u64,
    public_key_len: usize,
    message_hash: *const u64,
    message_hash_len: usize,
    r: *const u64,
    r_len: usize,
    s: *const u64,
    s_len: usize,
    is_valid: *mut bool,
) -> i32 {
    validate_ptr!(public_key, ERR_NULLPTR_PUBLIC_KEY);
    validate_ptr!(message_hash, ERR_NULLPTR_PUBLIC_KEY);
    validate_ptr!(r, ERR_NULLPTR_R);
    validate_ptr!(s, ERR_NULLPTR_S);
    // validate_ptr!(is_valid, ERR_NULLPTR_OUTPUT);

    if public_key_len != FELT_LIMBS_LEN {
        return ERR_PUBLIC_KEY_LEN;
    }
    let public_key = raw_pointer_into_felt(public_key);

    if message_hash_len != FELT_LIMBS_LEN {
        return ERR_MESSAGE_HASH_LEN;
    }
    let message_hash = raw_pointer_into_felt(message_hash);

    if r_len != FELT_LIMBS_LEN {
        return ERR_R_LEN;
    }
    let r = raw_pointer_into_felt(r);

    if s_len != FELT_LIMBS_LEN {
        return ERR_S_LEN;
    }
    let s = raw_pointer_into_felt(s);

    let signature = Signature { r, s };
    match ecdsa_verify_rs(&public_key, &message_hash, &signature) {
        Ok(res) => {
            unsafe {
                *is_valid = res;
            }
            SUCCESSFUL
        }
        Err(_) => ERR_VERIFY_FAILED,
    }
}
