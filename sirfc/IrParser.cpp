// This file is a part of the SIRF (Simple Intermediate Representation Format)
// project
// Copyright (C) 2025 XnLogical - Licensed under GNU GPL v3.0

#include "IrParser.hpp"

namespace sirf {

using enum TokenKind;

static std::pair<size_t, size_t> translateOffset(const std::string& source, size_t offset) {
  size_t line = 1, col = 0;
  size_t i = 0;
  size_t end = std::min(offset, source.size());

  for (; i < end; ++i) {
    if (source[i] == '\n') {
      ++line;
      col = 0;
    }
    else {
      ++col;
    }
  }
  return {line, col};
}

template<typename... Args>
[[noreturn]] static void throwUnexpectedToken(
  std::string src, size_t loc, std::format_string<Args...>&& fmt, Args&&... args
) {
  auto [line, col] = translateOffset(src, loc);
  throw IrParserException(line, col, std::format(fmt, std::forward<Args>(args)...));
}

Token IrParser::peek(int ahead) {
  if (pos + ahead >= state.tokHolder.size()) {
    size_t loc = state.tokHolder.empty() ? 0u : state.tokHolder.back().loc;
    throwUnexpectedToken(state.inputSource, loc, "unexpected end of file");
  }
  return state.tokHolder[pos + ahead];
}

Token IrParser::consume() {
  if (pos >= state.tokHolder.size()) {
    size_t loc = state.tokHolder.empty() ? 0u : state.tokHolder.back().loc;
    throwUnexpectedToken(state.inputSource, loc, "unexpected end of file");
  }
  return state.tokHolder[pos++];
}

Token IrParser::assertConsume(TokenKind expected) {
  Token token = consume();
  if (token.kind != expected) {
    throwUnexpectedToken(
      state.inputSource, token.loc, "unexpected token '{}' (expected {})", token.lexeme, magic_enum::enum_name(expected)
    );
  }
  return token;
}

IrScope IrParser::parseScope() {
  IrScope scope;
  assertConsume(BRACE_OPEN);

  while (peek(0).kind != BRACE_CLOSE) {
    scope.push_back(parseStmt());
  }

  assertConsume(BRACE_CLOSE);
  return scope;
}

IrValue IrParser::parseValue() {
  Token token = peek(0);

  switch (token.kind) {
  case AT: {
    // @symbol
    consume();
    Token nameTok = consume();
    return IrValueSymbol::newValue(nameTok.lexeme);
  }

  case PERCENT: {
    // %ssaName (implicit i32 type if no type is provided later)
    consume();
    Token nameTok = consume();
    if (nameTok.kind == LIT_INT)
      return IrValueSSA::newValue("", std::stoull(nameTok.lexeme));
    else
      return IrValueSSA::newValue(nameTok.lexeme, 0);
  }

  case IDENT: {
    // Could be: <type> <integer literal>  OR  register name like "r11".
    // We first try to parse a type. If the next token is an integer literal, we emit a literal.
    // Otherwise, fallback to register pattern: "r<digits>".

    // Save position in case it’s not actually a type
    size_t savePos = pos;
    Token typeTok = consume(); // This is IDENT; might represent a sized type like "i32" or "u64"

    // Attempt to parse sized type (iN / uN).
    bool isSigned = false;
    uint64_t bitWidth = 0;
    do {
      const std::string& lex = typeTok.lexeme;
      if ((lex[0] == 'i' || lex[0] == 'u') && lex.size() > 1) {
        try {
          isSigned = (lex[0] == 'i');
          bitWidth = std::stoull(lex.substr(1));
        }
        catch (...) {
          // Fall through; invalid sized‐type syntax -> fallback to register parsing.
          bitWidth = 0;
        }
      }
    } while (0);

    if (bitWidth != 0) {
      uint64_t undef; // used for undefined initialization
      IrType type = IrTypeSized::newType(isSigned, bitWidth);

      if (peek(0).kind == LIT_INT) {
        // We have a proper "iN" or "uN" followed by a literal token.
        Token litTok = consume();
        uint64_t intValue = 0;
        try {
          intValue = std::stoull(litTok.lexeme);
        }
        catch (...) {
          throwUnexpectedToken(state.inputSource, litTok.loc, "invalid integer literal '{}'", litTok.lexeme);
        }
        return IrValueLiteral::newValue(type, intValue);
      }

      return IrValueLiteral::newValue(type, undef);
    }

    // Not a literal after iN/uN. Reset position to parse as register.
    pos = savePos;

    // Now check if IDENT is a register like "r11".
    Token regTok = consume();
    const std::string& regLex = regTok.lexeme;
    if (regLex.size() >= 2 && regLex[0] == 'r' && std::all_of(regLex.begin() + 1, regLex.end(), ::isdigit)) {
      uint32_t regId = 0;
      try {
        regId = static_cast<uint32_t>(std::stoul(regLex.substr(1)));
      }
      catch (...) {
        throwUnexpectedToken(state.inputSource, regTok.loc, "invalid register id '{}'", regLex);
      }
      return IrValueRegister::newValue(regId, IrRegisterKind::QWORD);
    }

    // If we reach here, it's neither a sized‐literal nor a register.
    throwUnexpectedToken(state.inputSource, regTok.loc, "unexpected identifier '{}' in value position", regLex);
  }

  default:
    throwUnexpectedToken(state.inputSource, token.loc, "unexpected token '{}' while parsing value expression", token.lexeme);
  }
}

IrType IrParser::parseType() {
  Token token = consume(); // Must be IDENT or CARET

  switch (token.kind) {
  case IDENT: {
    // Sized type like "i32" or "u64"
    const std::string& lex = token.lexeme;
    if ((lex[0] == 'i' || lex[0] == 'u') && lex.size() > 1) {
      bool isSigned = (lex[0] == 'i');
      uint64_t bitWidth = 0;
      try {
        bitWidth = std::stoull(lex.substr(1));
      }
      catch (...) {
        throwUnexpectedToken(state.inputSource, token.loc, "invalid sized type '{}'", lex);
      }
      return IrTypeSized::newType(isSigned, bitWidth);
    }
    break;
  }

  case CARET: {
    // ^<type>
    IrType pointee = parseType();
    return IrTypePtr::newType(std::move(pointee));
  }

  default:
    break;
  }

  throwUnexpectedToken(state.inputSource, token.loc, "unexpected token '{}' while parsing type expression", token.lexeme);
}

IrStmt IrParser::parseStmt() {
  Token token = peek(0);

  switch (token.kind) {
  case KW_GLOBAL:
    consume();
    return IrStmtDeclaration::newStmt(IrDeclKind::GLOBAL, parseValue());
  case KW_EXTERN:
    consume();
    return IrStmtDeclaration::newStmt(IrDeclKind::EXTERN, parseValue());
  case PERCENT: {
    IrValue lval = parseValue();
    assertConsume(EQUALS);
    IrValue rval = parseValue();
    return IrStmtAssign::newStmt(lval, rval);
  }
  case KW_FUN: {
    consume();

    // fun <retType> @<symbol> ( params ) <scope>
    IrType returnType = parseType();

    IrValue maybeSym = parseValue();
    IrValueSymbol* symbol = dynamic_cast<IrValueSymbol*>(maybeSym.get());
    if (!symbol) {
      auto [line, col] = translateOffset(state.inputSource, peek(0).loc);
      throw IrParserException(line, col, "Expected function name symbol");
    }

    assertConsume(PAREN_OPEN);

    std::vector<IrParameter> params;
    while (peek(0).kind != PAREN_CLOSE) {
      IrValue paramName = parseValue();
      IrValueSSA* ssaName = dynamic_cast<IrValueSSA*>(paramName.get());
      if (!ssaName) {
        auto [line, col] = translateOffset(state.inputSource, peek(0).loc);
        throw IrParserException(line, col, "Expected SSA name in parameter list");
      }

      IrType paramType = parseType();
      params.emplace_back(std::move(*ssaName), std::move(paramType));

      if (peek(0).kind == COMMA) {
        consume(); // skip comma
      }
    }
    assertConsume(PAREN_CLOSE);

    IrScope bodyScope = parseScope();
    return IrStmtFunction::newStmt(
      std::move(returnType), *symbol, std::move(params),
      /*localVars=*/{}, std::move(bodyScope)
    );
  }

  case IDENT: {
    consume();

    // Raw instruction: <OPCODE> <operand> (',' <operand>)*
    std::string opcodeLex = token.lexeme;
    std::transform(opcodeLex.begin(), opcodeLex.end(), opcodeLex.begin(), ::toupper);

    auto maybeOp = magic_enum::enum_cast<IrOpCode>(opcodeLex);
    if (!maybeOp.has_value())
      throwUnexpectedToken(state.inputSource, token.loc, "unrecognized opcode '{}'", token.lexeme);
    IrOpCode op = *maybeOp;

    std::vector<IrValue> operands;
    do {
      operands.push_back(parseValue());
      if (peek(0).kind == COMMA)
        consume();
      else
        break;
    } while (true);

    return IrStmtInstruction::newStmt(op, std::move(operands));
  }

  default: {
    throwUnexpectedToken(state.inputSource, token.loc, "unexpected token '{}' while parsing statement", token.lexeme);
  }
  }
}

void IrParser::parse() {
  try {
    while (true) {
      if (peek(0).kind == EOF_) {
        break;
      }
      state.irHolder.data.push_back(parseStmt());
    }
  }
  catch (const IrParserException& err) {
    state.exitCode = 1;
    spdlog::error("{}:{}:{}: {}", state.inputPath, err.line, err.off, err.msg);
  }
}

} // namespace sirf
