// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#ifndef SIRF_IRLEXER_HPP
#define SIRF_IRLEXER_HPP

#include <Core/Common.hpp>

namespace SIRF {

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
  AT,

  IDENT,

  LIT_INT,
  LIT_HINT,
  LIT_BINT,
  LIT_FLOAT,
  LIT_STR,

  KW_FUN,
  KW_GLOBAL,
  KW_EXTERN,
};

struct Token {
  size_t loc;
  TokenKind kind;
  std::string lexeme;
};

using TokenHolder = std::vector<Token>;

class IrLexer final {
public:
  explicit IrLexer(const std::string& source, TokenHolder& holder)
    : holder(holder),
      source(source) {}

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

  TokenHolder& holder;
  const std::string& source;
};

} // namespace SIRF

#endif
