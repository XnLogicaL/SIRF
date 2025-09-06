/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use std::collections::HashMap;

pub struct InternTable {
    map: HashMap<String, Box<str>>,
}

impl InternTable {
    pub fn new() -> Self {
        Self {
            map: HashMap::new(),
        }
    }

    pub fn intern(&mut self, s: &str) -> &str {
        let entry = self
            .map
            .entry(s.to_string())
            .or_insert_with(|| s.to_string().into_boxed_str());
        entry
    }
}
