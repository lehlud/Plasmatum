#include "parser.hh"

#include <iostream>

#include "instruction.hh"
#include "utils.hh"
#include "value.hh"

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

plsm_int_t Parser::parseInteger() {
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

    return std::stol(tmp, nullptr, 16);
  }

  while (std::isdigit((c = charAt(index)))) {
    tmp += c;
    index += 1;
  }

  if (!tmp.size()) {
    errorExpected("Int");
  }

  return std::stol(tmp);
}

std::string Parser::parseIdentifer() {
  std::string tmp;

  char32_t c;
  while (isIdChar((c = charAt(index)))) {
    tmp += c;
    index += 1;
  }

  maybeSeparatorError("identifier");

  return tmp;
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

std::shared_ptr<Constant> LLParser::parseConstantValue() {
  std::string tmp;

  if (isTrue()) {
    return BooleanValue::get(true);
  } else if (isFalse()) {
    return BooleanValue::get(false);
  }

  char32_t c = charAt(index);
  if ((c = charAt(index)) == '0' && charAt(index + 1) == 'x') {
    index += 2;
    while (std::isxdigit((c = charAt(index)))) {
      tmp += c;
      index += 1;
    }

    maybeSeparatorError("hex number");

    return IntegerValue::get(std::stol(tmp, nullptr, 16));
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

    return IntegerValue::get(std::stol(tmp));
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

    return FloatValue::get(std::stod(tmp));
  }
}

std::shared_ptr<Instruction> LLParser::parseNext() {
  skipSpaces();

  if (isEOF()) {
    return nullptr;
  }

  std::string label = parseInstructionLabel();

  if (label == "CALL") {
    skipSpaces();
    return CallInstruction::get(parseInteger());
  } else if (label == "JUMP") {
    skipSpaces();
    return JumpInstruction::get(parseInteger());
  } else if (label == "JUMP_COND") {
    skipSpaces();
    return JumpCondInstruction::get(parseInteger());
  } else if (label == "FUNC_START") {
    skipSpaces();
    return FunctionStartInstruction::get(parseInteger());
  } else if (label == "FUNC_FINISH") {
    return FunctionFinishInstruction::get();
  } else if (label == "LOAD_CONST") {
    skipSpaces();
    return LoadConstInstruction::get(parseConstantValue());
  } else if (label == "LOAD_ARG") {
    skipSpaces();
    return LoadArgInstruction::get(parseInteger());
  } else if (label == "LOAD_GLOBAL") {
    skipSpaces();
    return LoadGlobalInstruction::get(parseIdentifer());
  } else if (label == "DEF_GLOBAL") {
    skipSpaces();
    return DefineGlobalInstruction::get(parseIdentifer());
  } else if (label == "ADD") {
    return AddInstruction::get();
  } else if (label == "SUB") {
    return SubInstruction::get();
  } else if (label == "MUL") {
    return MulInstruction::get();
  } else if (label == "DIV") {
    return DivInstruction::get();
  } else if (label == "MOD") {
    return ModInstruction::get();
  } else if (label == "EQ") {
    return EQInstruction::get();
  } else if (label == "NE") {
    return NEInstruction::get();
  } else if (label == "GT") {
    return GTInstruction::get();
  } else if (label == "GE") {
    return GEInstruction::get();
  } else if (label == "LT") {
    return LTInstruction::get();
  } else if (label == "LE") {
    return LEInstruction::get();
  } else if (label == "RETURN") {
    return ReturnInstruction::get();
  } else {
    errorExpected("instruction");
  }

  return nullptr;
}

std::vector<std::shared_ptr<Instruction>> LLParser::parse() {
  std::vector<std::shared_ptr<Instruction>> result;

  std::shared_ptr<Instruction> inst;
  while ((inst = parseNext())) {
    result.push_back(inst);
  }

  return result;
}

} // namespace plsm
