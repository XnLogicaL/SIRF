/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use crate::node::Node;

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

impl Node for Constant {
    fn to_repr(&self) -> String {
        match self {
            Constant::Byte(data) => format!("byte({data})"),
            Constant::Int8(data) => format!("int8({data})"),
            Constant::Int16(data) => format!("int16({data})"),
            Constant::Int32(data) => format!("int32({data})"),
            Constant::Int64(data) => format!("int64({data})"),
            Constant::Int128(data) => format!("int128({data})"),
            Constant::Uint8(data) => format!("uint8({data})"),
            Constant::Uint16(data) => format!("uint16({data})"),
            Constant::Uint32(data) => format!("uint32({data})"),
            Constant::Uint64(data) => format!("uint64({data})"),
            Constant::Uint128(data) => format!("uint128({data})"),
            Constant::Float32(data) => format!("float32({data})"),
            Constant::Float64(data) => format!("float64({data})"),
        }
    }
}

#[derive(Debug, Clone, Copy)]
pub enum Expr {
    Constant(Constant),
}
