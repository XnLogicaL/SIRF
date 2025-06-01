// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRPARSER_HPP
#define SIRF_IRPARSER_HPP

#include <cassert>
#include <exception>
#include <spdlog/spdlog.h>
#include <Core/Common.hpp>
#include <IR/IrStmtAssign.hpp>
#include <IR/IrStmtDeclaration.hpp>
#include <IR/IrStmtFunction.hpp>
#include <IR/IrStmtInstruction.hpp>
#include <IR/IrValueRegister.hpp>
#include <IR/IrValueSSA.hpp>
#include <IR/IrValueLiteral.hpp>
#include <IR/IrValueSymbol.hpp>
#include <IR/IrValueLabel.hpp>
#include <IR/IrValuePtr.hpp>
#include <IR/IrTypeSized.hpp>
#include <IR/IrTypePtr.hpp>
#include <IR/IrHolder.hpp>
#include "IrLexer.hpp"

namespace SIRF {

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
  explicit IrParser(const std::string& source, const TokenHolder& tokHolder, IrHolder& irHolder)
    : irHolder(irHolder),
      tokHolder(tokHolder),
      source(source) {}

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

  IrHolder& irHolder;
  const TokenHolder& tokHolder;
  const std::string& source;
};

} // namespace SIRF

#endif
