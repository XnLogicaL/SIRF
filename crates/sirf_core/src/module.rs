/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use std::fs;
use std::io;
use std::path::{Path, PathBuf};

#[derive(Debug)]
pub struct Module {
    pub path: PathBuf,
    pub name: String,
    pub source: Vec<u8>,
}

#[derive(Debug)]
pub enum ModuleError {
    OsError(io::Error),
    InvalidFileName,
}

impl Module {
    pub fn new(path: &Path) -> Result<Self, ModuleError> {
        let name = path
            .file_stem()
            .ok_or(ModuleError::InvalidFileName)?
            .to_string_lossy()
            .into_owned();

        let data = fs::read(path).map_err(ModuleError::OsError)?;

        Ok(Self {
            path: path.to_path_buf(),
            name: name,
            source: data,
        })
    }
}
