// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRFC_IRPARSER_HPP
#define SIRFC_IRPARSER_HPP

#include <IR/IrValues.hpp>
#include <IR/IrTypes.hpp>
#include <IR/IrStmts.hpp>
#include "IrLexer.hpp"
#include "CState.hpp"

namespace sirf {

class IrParserException : public std::exception {
public:
  size_t line;
  size_t off;
  const char* msg;

  IrParserException(size_t line, size_t off, const char* msg)
    : line(line),
      off(off),
      msg(msg) {}
};

class IrParser final {
public:
  explicit IrParser(CState& state)
    : state(state) {}

  void parse();

private:
  Token peek(int ahead = 1);
  Token consume();
  Token assertConsume(TokenKind kind);

  IrScope parseScope();

  IrValue parseValue();
  IrType parseType();
  IrStmt parseStmt();

private:
  size_t pos = 0;
  CState& state;
};

} // namespace sirf

#endif
