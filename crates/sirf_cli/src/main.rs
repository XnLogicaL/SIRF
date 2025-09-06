/* ================================================== **
**  SIRF - Simple Intermediate Representation Format  **
** -------------------------------------------------- **
**          Copyright (C) XnLogicaL 2025              **
**           Licensed under GNU GPLv3.0               **
** -------------------------------------------------- **
**        https://github.com/XnLogicaL/SIRF           **
** ================================================== */

use std::path::PathBuf;

use bumpalo::Bump;
use clap::Parser;
use clio::*;
use sirf_core::{diagnostics::Diagnostics, module::Module};
use sirf_parser::parser::parse_file;

#[derive(Parser)]
#[clap(name = "sirfc")]
struct Opt {
    /// Input file, use '-' for stdin
    #[clap(value_parser, default_value = "-")]
    input: Input,

    /// Output file '-' for stdout
    #[clap(long, short, value_parser, default_value = "-")]
    output: Output,

    /// Directory to store log files in
    #[clap(long, short, value_parser = clap::value_parser!(ClioPath).exists().is_dir(), default_value = ".")]
    log_dir: ClioPath,
}

fn main() -> Result<()> {
    let opt = Opt::parse();

    println!("input: {} output: {}", opt.input, opt.output);

    let mut arena = Bump::new();
    let path = PathBuf::new();

    let module = Module::new(&path).unwrap();
    let mut diags = Diagnostics::new(&module);

    let ast = parse_file(&mut arena, &module, &mut diags).unwrap();

    Ok(())
}
