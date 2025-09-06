/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use std::fmt::format;

use crate::node::*;

#[derive(Debug, Clone, Copy)]
pub enum Constant {
    Byte(u8),
    Int8(i8),
    Int16(i16),
    Int32(i32),
    Int64(i64),
    Int128(i128),
    Uint8(u8),
    Uint16(u16),
    Uint32(u32),
    Uint64(u64),
    Uint128(u128),
    Float32(f32),
    Float64(f64),
}

impl Repr for Constant {
    fn to_repr(&self) -> String {
        match self {
            Self::Byte(data) => format!("byte({data})"),
            Self::Int8(data) => format!("int8({data})"),
            Self::Int16(data) => format!("int16({data})"),
            Self::Int32(data) => format!("int32({data})"),
            Self::Int64(data) => format!("int64({data})"),
            Self::Int128(data) => format!("int128({data})"),
            Self::Uint8(data) => format!("uint8({data})"),
            Self::Uint16(data) => format!("uint16({data})"),
            Self::Uint32(data) => format!("uint32({data})"),
            Self::Uint64(data) => format!("uint64({data})"),
            Self::Uint128(data) => format!("uint128({data})"),
            Self::Float32(data) => format!("float32({data})"),
            Self::Float64(data) => format!("float64({data})"),
        }
    }
}

#[derive(Debug)]
pub struct Symbol {
    pub id: &'static str,
}

impl Repr for Symbol {
    fn to_repr(&self) -> String {
        format!("@{}", self.id)
    }
}

#[derive(Debug)]
pub struct Label {
    pub id: &'static str,
}

impl Repr for Label {
    fn to_repr(&self) -> String {
        format!(".{}", self.id)
    }
}

#[derive(Debug)]
pub enum Expression {
    Constant(Constant),
    Symbol(Symbol),
    Label(Label),
}

impl Repr for Expression {
    fn to_repr(&self) -> String {
        match self {
            Expression::Constant(c) => c.to_repr(),
            Expression::Symbol(s) => s.to_repr(),
            Expression::Label(l) => l.to_repr(),
        }
    }
}
