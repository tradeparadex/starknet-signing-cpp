use crate::constants::FELT_LIMBS_LEN;

use crypto_bigint::U256;
use starknet_core::types::FieldElement;

/// Copies montgomery representation
pub(crate) fn copy_mont_into_raw(src: &FieldElement, dst: *mut u64) {
    unsafe {
        std::ptr::copy(src.into_mont().as_ptr(), dst, FELT_LIMBS_LEN);
    }
}

pub(crate) fn copy_u256_into_raw(src: &U256, dst: *mut u64) {
    unsafe {
        std::ptr::copy(src.as_words().as_ptr(), dst, FELT_LIMBS_LEN);
    }
}

pub(crate) fn raw_pointer_into_felt(src: *const u64) -> FieldElement {
    let mut output: [u64; 4] = [0; 4];
    unsafe {
        std::ptr::copy(src, output.as_mut_ptr(), FELT_LIMBS_LEN);
    }

    FieldElement::from_mont(output)
}