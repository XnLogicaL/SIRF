/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use crate::lexer::Token;
use bumpalo::Bump;
use sirf_core::diagnostics::Diagnostics;
use sirf_core::module::Module;

pub fn parse_file<'a>(
    arena: &'a mut Bump,
    module: &'a Module,
    diags: &'a mut Diagnostics,
) -> Option<Vec<Token<'a>>> {
    None
}
