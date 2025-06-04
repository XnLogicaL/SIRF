// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrParser.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <algorithm>

namespace sirf {

using enum TokenKind;

static std::pair<size_t, size_t> translateOffset(const std::string& source, size_t off) {
  size_t line = 1;
  size_t col = 0;
  size_t i = 0;
  size_t end = std::min(off, source.size());

  while (i < end) {
    if (source[i] == '\n') {
      ++line;
      col = 0;
    }
    else {
      ++col;
    }
    ++i;
  }

  return {line, col};
}

Token IrParser::peek(int ahead) {
  if (pos + ahead >= state.tokHolder.size()) {
    size_t loc = state.tokHolder.empty() ? 0 : state.tokHolder.back().loc;
    auto [line, col] = translateOffset(state.fileSource, loc);
    throw IrParserException(line, col, "Unexpected end of file");
  }

  return state.tokHolder.at(pos + ahead);
}

Token IrParser::consume() {
  if (pos >= state.tokHolder.size()) {
    size_t loc = state.tokHolder.empty() ? 0 : state.tokHolder.back().loc;
    auto [line, col] = translateOffset(state.fileSource, loc);
    throw IrParserException(line, col, "Unexpected end of file");
  }

  return state.tokHolder.at(pos++);
}

Token IrParser::assertConsume(TokenKind kind) {
  Token tok = consume();
  if (tok.kind != kind) {
    auto [line, col] = translateOffset(state.fileSource, tok.loc);
    throw IrParserException(line, col, "Unexpected token");
  }

  return tok;
}

IrScope IrParser::parseScope() {
  IrScope scope;
  assertConsume(BRACE_OPEN);

  while (true) {
    if (peek(0).kind == BRACE_CLOSE) {
      break;
    }

    IrStmt stmt = parseStmt();
    scope.push_back(std::move(stmt));
  }

  assertConsume(BRACE_CLOSE);
  return scope;
}

IrValue IrParser::parseValue() {
  Token tok = consume();

  switch (tok.kind) {
  case AT:
    return IrValueSymbol::newValue(consume().lexeme);
  case PERCENT:
    return IrValueSSA::newValue(consume().lexeme, 0);
  case LIT_INT: {
    try {
      IrType type = parseType();
      return IrValueLiteral::newValue(type, std::stoull(tok.lexeme));
    }
    catch (const std::exception& err) {
      auto [line, off] = translateOffset(state.fileSource, tok.loc);
      throw IrParserException(line, off, "Invalid integer literal");
    }
  }
  default:
    break;
  }

  assert(false);
  SIRF_UNREACHABLE();
}

IrType IrParser::parseType() {
  Token tok = consume();

  switch (tok.kind) {
  case IDENT: {
    try {
      char pref = tok.lexeme[0];
      if (pref == 'i' || pref == 'u') {
        const auto size = tok.lexeme.substr(1, tok.lexeme.length() - 1);
        return IrTypeSized::newType(pref == 'i', std::stoull(size));
      }
    }
    catch (const std::exception& err) {
      auto [line, off] = translateOffset(state.fileSource, tok.loc);
      throw IrParserException(line, off, "Invalid size");
    }
    break;
  }
  case CARET:
    return IrTypePtr::newType(parseType());
  default:
    break;
  }

  assert(false);
  SIRF_UNREACHABLE();
}

IrStmt IrParser::parseStmt() {
  Token tok = consume();

  switch (tok.kind) {
  case KW_GLOBAL:
    return IrStmtDeclaration::newStmt(IrDeclKind::GLOBAL, parseValue());
  case KW_EXTERN:
    return IrStmtDeclaration::newStmt(IrDeclKind::EXTERN, parseValue());
  case KW_FUN: {
    IrType ret = parseType();
    IrValue id = parseValue();
    IrValueSymbol* sym = dynamic_cast<IrValueSymbol*>(id.get());
    std::vector<IrParameter> params;

    if (!sym) {
      auto [line, col] = translateOffset(state.fileSource, peek(0).loc);
      throw IrParserException(line, col, "Expected symbol");
    }

    assertConsume(PAREN_OPEN);

    while (true) {
      if (peek(0).kind == PAREN_CLOSE)
        break;

      IrValue paramId = parseValue();
      IrType paramType = parseType();
      IrValueSSA* ssa = dynamic_cast<IrValueSSA*>(paramId.get());

      if (!ssa) {
        auto [line, col] = translateOffset(state.fileSource, peek(0).loc);
        throw IrParserException(line, col, "Expected SSA");
      }

      params.emplace_back(std::move(*ssa), std::move(paramType));

      if (peek(0).kind != PAREN_CLOSE) {
        assertConsume(COMMA); // Assuming COMMA-separated args
      }
    }

    assertConsume(PAREN_CLOSE);

    return IrStmtFunction::newStmt(std::move(ret), std::move(*sym), std::move(params), {}, parseScope());
  }
  case IDENT: {
    IrOpCode op;
    // clang-format off
    if (tok.lexeme == "ret") op = IrOpCode::RET;
    // clang-format on

    std::vector<IrValue> ops;
    while (true) {
      ops.push_back(parseValue());
      if (peek(0).kind == COMMA)
        consume();
      else
        break;
    }

    return IrStmtInstruction::newStmt(op, std::move(ops));
  }
  default:
    break;
  }

  assert(false);
  SIRF_UNREACHABLE();
}

void IrParser::parse() {
  try {
    while (true) {
      if (peek(0).kind == EOF_)
        break;
      state.irHolder.data.push_back(parseStmt());
    }
  }
  catch (const IrParserException& err) {
    state.exitCode = 1;
    spdlog::error("{}:{}:{}: {}", state.filePath, err.line, err.off, err.msg);
  }
}

} // namespace sirf
