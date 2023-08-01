use starknet_crypto::{pedersen_hash as pedersen_hash_rs};

use crate::{
    constants::FELT_LIMBS_LEN,
    error_codes::{ERR_NULLPTR_OUTPUT, ERR_NULLPTR_X, ERR_NULLPTR_Y, ERR_X_LEN, ERR_Y_LEN, SUCCESSFUL},
    utils::{copy_mont_into_raw, raw_pointer_into_felt},
    validate_ptr,
};

#[no_mangle]
pub extern "C" fn pedersen_hash(x: *const u64, x_len: usize, y: *const u64, y_len: usize, output: *mut u64) -> i32 {
    validate_ptr!(x, ERR_NULLPTR_X);
    validate_ptr!(y, ERR_NULLPTR_Y);
    // validate_ptr!(output, ERR_NULLPTR_OUTPUT);

    if x_len != FELT_LIMBS_LEN {
        return ERR_X_LEN;
    }
    let x = raw_pointer_into_felt(x);

    if y_len != FELT_LIMBS_LEN {
        return ERR_Y_LEN;
    }
    let y = raw_pointer_into_felt(y);

    let hash = pedersen_hash_rs(&x, &y);
    copy_mont_into_raw(&hash, output);

    SUCCESSFUL
}
