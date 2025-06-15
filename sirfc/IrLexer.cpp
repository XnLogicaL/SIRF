// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrLexer.hpp"

#define IN_RANGE() pos < state.inputSource.size()
#define NEXT_CHAR()                                                                                                         \
  {                                                                                                                         \
    off++;                                                                                                                  \
    pos++;                                                                                                                  \
  }

namespace sirf {

using enum TokenKind;

static bool isHexChar(char chr) {
  return (chr >= 'A' && chr <= 'F') || (chr >= 'a' && chr <= 'f');
}

char IrLexer::peek(int ahead) {
  if (pos + ahead >= state.inputSource.size()) {
    return '\0';
  }
  return state.inputSource.at(pos + ahead);
}

Token IrLexer::readNumber() {
  TokenKind type = LIT_INT;
  size_t loc = pos;
  char delimiter = '\0';
  std::string value;

  if (peek(0) == '0' && (peek(1) == 'x' || peek(1) == 'b')) {
    NEXT_CHAR();

    if (peek(0) == 'b') {
      type = LIT_BINT;
    }
    else if (peek(0) == 'x') {
      type = LIT_HINT;
    }

    delimiter = peek(0);
    NEXT_CHAR();
  }

  while (IN_RANGE() && (std::isdigit(peek(0)) || (type == LIT_HINT && isHexChar(peek(0))))) {
    value.push_back(peek(0));
    NEXT_CHAR();
  }

  if (IN_RANGE() && peek(0) == '.') {
    value.push_back(peek(0));
    type = LIT_FLOAT;
    NEXT_CHAR();

    while (IN_RANGE() && std::isdigit(peek(0))) {
      value.push_back(peek(0));
      NEXT_CHAR();
    }
  }

  // If it was hex or binary, prefix “0x” or “0b” back onto the string:
  if (type == LIT_HINT || type == LIT_BINT) {
    value = std::format("0{}{}", delimiter, value);
  }

  return {loc, type, value};
}

Token IrLexer::readIdent() {
  static const std::vector<char> allowed_identifier_spec_chars = {'_', '!'};

  TokenKind type = IDENT;
  size_t loc = pos;
  std::string lexeme;

  auto is_allowed = [&](char chr) -> bool {
    bool is_alnum = std::isalnum(static_cast<unsigned char>(chr));
    bool is_special =
      (std::ranges::find(allowed_identifier_spec_chars.begin(), allowed_identifier_spec_chars.end(), chr)
       != allowed_identifier_spec_chars.end());
    return is_alnum || is_special;
  };

  while (IN_RANGE() && is_allowed(peek(0))) {
    lexeme.push_back(peek(0));
    NEXT_CHAR();
  }

  static const std::unordered_map<std::string, TokenKind> kwMap = {
    {"global", KW_GLOBAL},
    {"extern", KW_EXTERN},
    {"data", KW_DATA},
    {"fun", KW_FUN},
  };

  auto it = kwMap.find(lexeme);
  if (it != kwMap.end()) {
    type = it->second;
  }

  return {loc, type, lexeme};
}

Token IrLexer::readString() {
  std::string lexeme;
  size_t loc = pos;

  NEXT_CHAR();

  while (IN_RANGE() && peek(0) != '"') {
    if (peek(0) == '\\') {
      NEXT_CHAR();

      if (IN_RANGE()) {
        char escape_char = peek(0);
        switch (escape_char) {
        case 'n':
          lexeme.push_back('\n');
          break;
        case 't':
          lexeme.push_back('\t');
          break;
        case 'r':
          lexeme.push_back('\r');
          break;
        default:
          lexeme.push_back(escape_char);
          break;
        }

        NEXT_CHAR();
      }
    }
    else {
      lexeme.push_back(peek(0));
      NEXT_CHAR();
    }
  }

  if (IN_RANGE() && peek(0) == '"') {
    NEXT_CHAR();
  }

  return {loc, LIT_STR, lexeme};
}

Token IrLexer::nextToken() {
  while (IN_RANGE()) {
    if (std::isspace(static_cast<unsigned char>(peek(0)))) {
      if (peek(0) == '\n') {
        line++;
        off = 0;
      }
      else
        off++;

      pos++;
      continue;
    }
    break;
  }

  size_t loc = pos;

  if (pos >= state.inputSource.size()) {
    return {loc, EOF_, "<eof>"};
  }

  if (std::isdigit((unsigned char)peek(0))) {
    return readNumber();
  }

  if (peek(0) == '"') {
    return readString();
  }

  if (std::isalpha((unsigned char)peek(0)) || peek(0) == '_') {
    return readIdent();
  }

  char chr = peek(0);
  NEXT_CHAR();

  // clang-format off
  switch (chr) {
#define CHR_CASE(id, type) case id: return {loc, type, std::string(1, id)};
  CHR_CASE('(', PAREN_OPEN)
  CHR_CASE(')', PAREN_CLOSE)
  CHR_CASE('{', BRACE_OPEN)
  CHR_CASE('}', BRACE_CLOSE)
  CHR_CASE('%', PERCENT)
  CHR_CASE('^', CARET)
  CHR_CASE(',', COMMA)
  CHR_CASE('@', AT)
  CHR_CASE('=', EQUALS)
  CHR_CASE(':', COLON)
  CHR_CASE('.', DOT)
#undef CHR_CASE
  default:
    break;
  } // clang-format on

  // If we reach here, it’s an unrecognized character
  return {loc, ILL, std::string(1, chr)};
}

void IrLexer::tokenize() {
  while (true) {
    Token tok = nextToken();
    state.tokHolder.push_back(tok);
    if (tok.kind == EOF_) {
      break;
    }
  }
}

} // namespace sirf
