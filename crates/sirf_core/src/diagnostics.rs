/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use crate::module::Module;
use std::io::{self, Write};

#[derive(Debug)]
pub struct SourceLoc {
    pub begin: usize,
    pub end: usize,
}

pub trait Error {
    fn is_fatal(&self) -> bool;
    fn to_string(&self) -> String;
}

pub struct Diagnosis {
    pub loc: SourceLoc,
    pub code: Box<dyn Error>,
}

impl Diagnosis {
    pub fn new(loc: SourceLoc, code: Box<dyn Error>) -> Self {
        Self {
            loc: loc,
            code: code,
        }
    }

    pub fn to_string(&self) -> String {
        "".to_string()
    }
}

pub struct Diagnostics<'m> {
    pub module: &'m Module,
    pub diags: Vec<Diagnosis>,
}

impl<'m> Diagnostics<'m> {
    pub fn new(module: &'m Module) -> Self {
        Self {
            module: &module,
            diags: vec![],
        }
    }

    pub fn report(&mut self, diag: Diagnosis) {
        self.diags.push(diag);
    }

    pub fn report_emplace(&mut self, loc: SourceLoc, code: Box<dyn Error>) {
        self.diags.push(Diagnosis::new(loc, code));
    }

    pub fn emit(&self, out: &mut dyn Write) -> io::Result<()> {
        self.diags
            .iter()
            .try_for_each(|diag| writeln!(out, "{}", diag.to_string()))
    }
}
