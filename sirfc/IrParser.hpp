// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRFC_IRPARSER_HPP
#define SIRFC_IRPARSER_HPP

#include "IrValues.hpp"
#include "IrTypes.hpp"
#include "IrStmts.hpp"
#include "IrLexer.hpp"
#include "CState.hpp"
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>

namespace sirf {

class IrParserException : public std::exception {
public:
  const size_t line;
  const size_t off;
  const std::string msg;

  explicit IrParserException(size_t line, size_t off, std::string msg)
    : line(line),
      off(off),
      msg(msg) {}

  template<typename... Args>
  explicit IrParserException(size_t line, size_t off, std::format_string<Args...> fmt, Args&&... args)
    : line(line),
      off(off),
      msg(std::format(fmt, std::forward<Args>(args)...)) {}

  const char* what() const throw() {
    return msg.c_str();
  }
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
