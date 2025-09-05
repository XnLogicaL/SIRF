/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
