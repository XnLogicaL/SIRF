/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use crate::intern_table::InternTable;
use lazy_static::lazy_static;
use std::sync::{Mutex, MutexGuard};

lazy_static! {
    static ref SYMBOL_TABLE: Mutex<InternTable> = Mutex::new(InternTable::new());
}

pub fn get_symbol_table() -> MutexGuard<'static, InternTable> {
    SYMBOL_TABLE.lock().unwrap()
}
