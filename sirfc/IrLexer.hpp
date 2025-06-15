// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRFC_IRLEXER_HPP
#define SIRFC_IRLEXER_HPP

#include "Common.hpp"
#include "CState.hpp"

namespace sirf {

enum class TokenKind {
  ILL,
  EOF_,

  PAREN_OPEN,
  PAREN_CLOSE,
  BRACE_OPEN,
  BRACE_CLOSE,
  PERCENT,
  CARET,
  COMMA,
  EQUALS,
  AT,

  IDENT,

  LIT_INT,
  LIT_HINT,
  LIT_BINT,
  LIT_FLOAT,
  LIT_STR,

  KW_FUN,
  KW_DATA,
  KW_GLOBAL,
  KW_EXTERN,
};

struct Token {
  size_t loc;
  TokenKind kind;
  std::string lexeme;
};

class IrLexer final {
public:
  explicit IrLexer(CState& state)
    : state(state) {}

  void tokenize();

private:
  char peek(int ahead = 1);
  char consume();

  Token readNumber();
  Token readIdent();
  Token readString();

  Token nextToken();

private:
  size_t line = 0;
  size_t off = 0;
  size_t pos = 0;
  CState& state;
};

} // namespace sirf

#endif
