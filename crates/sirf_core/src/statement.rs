/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use crate::node::*;

pub mod declaration {
    use crate::expression::Symbol;
    use crate::node::Repr;
    use crate::types::Type;

    #[derive(Debug)]
    pub struct Global {
        pub id: Symbol,
    }

    impl Repr for Global {
        fn to_repr(&self) -> String {
            format!("global {};", self.id.to_repr())
        }
    }

    #[derive(Debug)]
    pub struct Data {
        pub id: Symbol,
        pub bytes: Vec<u8>,
    }

    impl Repr for Data {
        fn to_repr(&self) -> String {
            format!(
                "data {}: {};",
                self.id.to_repr(),
                self.bytes
                    .iter()
                    .map(|b| b.to_string())
                    .collect::<Vec<_>>()
                    .join(",")
            )
        }
    }

    #[derive(Debug)]
    pub struct Function {
        pub id: Symbol,
        pub parms: Vec<Type>,
        pub returns: Type,
    }

    #[derive(Debug)]
    pub enum Declaration {
        Global(Global),
        Data(Data),
        Function(Function),
    }
}

#[derive(Debug)]
pub enum Statement {
    Decl(declaration::Declaration),
    DeclData(declaration::Data),
    DeclGlobal(declaration::Global),
    DeclFunction(declaration::Function),
}
