/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use bumpalo::Bump;
use sirf_core::diagnostics::SourceLoc;
use sirf_core::module::Module;
use std::io::{Cursor, Read};

#[derive(Debug, Clone, Copy)]
pub enum TokenKind {}

#[derive(Debug)]
pub struct Token<'a> {
    pub kind: TokenKind,
    pub loc: SourceLoc,
    pub lexeme: &'a str,
}

impl<'a> Token<'a> {
    pub fn to_string(&self) -> String {
        self.lexeme.to_string()
    }
}

#[derive(Debug)]
pub struct Lexer<'a> {
    arena: &'a mut Bump,
    module: &'a Module,
    cursor: Cursor<u8>,
}
