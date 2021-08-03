#include "parser.hh"

#include <iostream>

#include "instruction.hh"
#include "utils.hh"
#include "value.hh"

#define _PRIM_PTR_VAL(name, type, value)                                       \
  type *name = new type;                                                       \
  *name = value;

namespace plsm {

void Parser::skipSpaces() {
  while (isSpace()) {
    index += 1;
  }

  if (charAt(index) == '-' && charAt(index + 1) == '-') {
    index += 2;

    char32_t c;
    while ((c = charAt(index)) != 0 && c != '\n' && c != '\r') {
      index += 1;
    }

    skipSpaces();
  } else if (charAt(index) == '\\' && charAt(index + 1) == '\\') {
    index += 2;

    while (charAt(index) != 0 &&
           (charAt(index) != '\\' || charAt(index + 1) != '\\')) {
      index += 1;
    }

    if (charAt(index) == 0) {
      errorExpected("comment delimiter before EOF");
    } else {
      index += 2;
    }

    skipSpaces();
  }
}

std::string Parser::getPosition(plsm_size_t index) {
  plsm_size_t lineno = 1, column = 1, tmpIndex = 0;

  char32_t c;
  while ((c = charAt(tmpIndex)) != 0 && tmpIndex <= index) {
    if (c == '\n' || c == '\r') {
      lineno += 1;
      column = 1;
    } else {
      column += 1;
    }
    tmpIndex += 1;
  }

  return std::to_string(lineno) + ":" + std::to_string(column);
}

void Parser::errorExpected(const std::string &exp) {
  if (fname.size()) {
    std::cout << fname << ":";
  }
  std::cout << getPosition() << " : expected " << exp << std::endl;
  std::exit(1);
}

plsm_size_t *Parser::parseSize() {
  std::string tmp;

  char32_t c;
  if ((c = charAt(index)) == '0' && (c = charAt(index)) == 'x') {
    index += 2;
    while (std::isxdigit((c = charAt(index)))) {
      tmp += c;
      index += 1;
    }

    if (!tmp.size()) {
      errorExpected("at least 1 digit after '0x'");
    }

    _PRIM_PTR_VAL(res, plsm_size_t, std::stol(tmp, nullptr, 16))
    return res;
  }

  while (std::isdigit((c = charAt(index)))) {
    tmp += c;
    index += 1;
  }

  if (!tmp.size()) {
    errorExpected("Int");
  }

  _PRIM_PTR_VAL(res, plsm_size_t, std::stol(tmp))
  return res;
}

std::string *Parser::parseIdentifer() {
  std::string tmp;

  char32_t c;
  while (isIdChar((c = charAt(index)))) {
    tmp += c;
    index += 1;
  }

  maybeSeparatorError("identifier");

  return new std::string(tmp);
}

std::string LLParser::parseInstructionLabel() {
  std::string tmp;

  char32_t c;
  while (isUpperAscii((c = charAt(index)))) {
    tmp += c;
    index += 1;
  }

  maybeSeparatorError("instruction label");

  return tmp;
}

constant *LLParser::parseConstantValue() {
  std::string tmp;

  if (isTrue()) {
    return new boolean(true);
  } else if (isFalse()) {
    return new boolean(false);
  }

  char32_t c = charAt(index);
  if ((c = charAt(index)) == '0' && charAt(index + 1) == 'x') {
    index += 2;
    while (std::isxdigit((c = charAt(index)))) {
      tmp += c;
      index += 1;
    }

    maybeSeparatorError("hex number");

    return new integer(std::stol(tmp, nullptr, 16));
  }

  if (c == '-') {
    index += 1;
    tmp += c;
  }

  while (std::isdigit((c = charAt(index)))) {
    tmp += c;
    index += 1;
  }

  if (!tmp.size()) {
    errorExpected("value");
  }

  if (charAt(index) != '.') {
    maybeSeparatorError("Int");

    return new integer(std::stol(tmp));
  } else {
    if (!tmp.size()) {
      tmp += '0';
    }

    tmp += '.';

    index += 1;
    while (std::isdigit((c = charAt(index)))) {
      tmp += c;
      index += 1;
    }

    maybeSeparatorError("Float");

    return new floating_point(std::stod(tmp));
  }
}

instruction *LLParser::parseNext() {
  skipSpaces();

  if (isEOF()) {
    return nullptr;
  }

  std::string label = parseInstructionLabel();

  if (label == "CALL") {
    skipSpaces();
    return new value_inst<plsm_size_t>(instruction::code_call, parseSize());
  } else if (label == "JUMP") {
    skipSpaces();
    return new value_inst<plsm_size_t>(instruction::code_jump, parseSize());
  } else if (label == "JUMP_COND") {
    skipSpaces();
    return new value_inst<plsm_size_t>(instruction::code_jump_cond, parseSize());
  } else if (label == "FUNC_START") {
    skipSpaces();
    return new value_inst<plsm_size_t>(instruction::code_func_start, parseSize());
  } else if (label == "FUNC_FINISH") {
    return new instruction(instruction::code_func_finish);
  } else if (label == "LOAD_CONST") {
    skipSpaces();
    return new value_inst<value>(instruction::code_load_const, parseConstantValue());
  } else if (label == "LOAD_ARG") {
    skipSpaces();
    return new value_inst<plsm_size_t>(instruction::code_load_arg, parseSize());
  } else if (label == "LOAD_GLOBAL") {
    skipSpaces();
    return new value_inst<std::string>(instruction::code_load_global, parseIdentifer());
  } else if (label == "DEF_GLOBAL") {
    skipSpaces();
    return new value_inst<std::string>(instruction::code_def_global, parseIdentifer());
  } else if (label == "ADD") {
    return new instruction(instruction::code_add);
  } else if (label == "SUB") {
    return new instruction(instruction::code_sub);
  } else if (label == "MUL") {
    return new instruction(instruction::code_mul);
  } else if (label == "DIV") {
    return new instruction(instruction::code_div);
  } else if (label == "MOD") {
    return new instruction(instruction::code_mod);
  } else if (label == "EQ") {
    return new instruction(instruction::code_eq);
  } else if (label == "NE") {
    return new instruction(instruction::code_ne);
  } else if (label == "GT") {
    return new instruction(instruction::code_gt);
  } else if (label == "GE") {
    return new instruction(instruction::code_ge);
  } else if (label == "LT") {
    return new instruction(instruction::code_lt);
  } else if (label == "LE") {
    return new instruction(instruction::code_le);
  } else if (label == "RETURN") {
    return new instruction(instruction::code_return);
  } else {
    errorExpected("instruction");
  }

  return nullptr;
}

std::vector<instruction *> LLParser::parse() {
  std::vector<instruction *> result;

  instruction *inst;
  while ((inst = parseNext())) {
    result.push_back(inst);
  }

  return result;
}

} // namespace plsm
