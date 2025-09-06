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
use std::fmt;

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
    source: &'a [u8],
    cursor: usize,
}

impl<'a> Lexer<'a> {
    pub fn new(arena: &'a mut Bump, module: &'a Module) -> Self {
        let source = module.source.as_ref();
        Self {
            arena,
            source,
            cursor: 0,
        }
    }

    pub fn tokenize(&mut self) -> Vec<Token<'a>> {
        vec![]
    }

    fn peek(&self, ahead: usize) -> u8 {
        self.source.get(self.cursor + ahead).copied().unwrap_or(0)
    }

    fn advance(&mut self, ahead: usize) -> u8 {
        let byte = self.peek(0);
        self.cursor += ahead.max(1);
        byte
    }
}
