use crate::constants::FELT_LIMBS_LEN;

use starknet_core::types::FieldElement;

/// Copies montgomery representation
pub(crate) fn copy_mont_into_raw(src: &FieldElement, dst: *mut u64) {
    unsafe {
        std::ptr::copy(src.into_mont().as_ptr(), dst, FELT_LIMBS_LEN);
    }
}

pub(crate) fn raw_pointer_into_felt(src: *const u64) -> FieldElement {
    let mut output: [u64; 4] = [0; 4];
    unsafe {
        std::ptr::copy(src, output.as_mut_ptr(), FELT_LIMBS_LEN);
    }

    FieldElement::from_mont(output)
}

#[macro_export]
macro_rules! validate_ptr {
    ($ptr: expr, $error_code: expr) => {
        if $ptr == std::ptr::null() {
            return $error_code;
        }
    };
}
