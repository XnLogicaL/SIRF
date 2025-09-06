/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use crate::lexer::{Lexer, Token};
use bumpalo::Bump;
use sirf_core::diagnostics::Diagnostics;
use sirf_core::expression::Expression;
use sirf_core::module::Module;
use sirf_core::statement::Statement;

pub struct Parser<'a> {
    arena: &'a mut Bump,
    diags: &'a mut Diagnostics<'a>,
    tokens: &'a Vec<Token<'a>>,
}

impl<'a> Parser<'a> {}

pub fn parse_file<'a>(
    arena: &'a mut Bump,
    module: &'a Module,
    diags: &'a mut Diagnostics,
) -> Option<Vec<Statement>> {
    let mut lexer = Lexer::new(arena, module);
    let tokens = lexer.tokenize();

    None
}
