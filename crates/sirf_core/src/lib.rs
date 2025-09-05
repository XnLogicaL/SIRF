/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

pub mod diagnostics;
pub mod module;

mod expr;
mod node;

pub mod ir {
    pub use crate::expr::*;
    pub use crate::node::*;
}
