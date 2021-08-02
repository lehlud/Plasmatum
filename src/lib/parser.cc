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

    while (charAt(index) != 0 && (charAt(index) != '\\' || charAt(index + 1) != '\\')) {
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
    std::cout << fname << ": ";
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

Constant *LLParser::parseConstantValue() {
  std::string tmp;

  if (isTrue()) {
    return new BooleanValue(true);
  } else if (isFalse()) {
    return new BooleanValue(false);
  }

  char32_t c = charAt(index);
  if ((c = charAt(index)) == '0' && charAt(index + 1) == 'x') {
    index += 2;
    while (std::isxdigit((c = charAt(index)))) {
      tmp += c;
      index += 1;
    }

    maybeSeparatorError("hex number");

    return new IntegerValue(std::stol(tmp, nullptr, 16));
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

    return new IntegerValue(std::stol(tmp));
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

    return new FloatValue(std::stod(tmp));
  }
}

Instruction *LLParser::parseNext() {
  skipSpaces();

  if (isEOF()) {
    return nullptr;
  }

  std::string label = parseInstructionLabel();

  if (label == "CALL") {
    skipSpaces();
    return new CallInstruction(parseInteger());
  } else if (label == "FUNC_START") {
    skipSpaces();
    return new FunctionStartInstruction(parseInteger());
  } else if (label == "FUNC_FINISH") {
    return new FunctionFinishInstruction();
  } else if (label == "LOAD_CONST") {
    skipSpaces();
    return new LoadConstInstruction(parseConstantValue());
  } else if (label == "LOAD_GLOBAL") {
    skipSpaces();
    return new LoadGlobalInstruction(parseIdentifer());
  } else if (label == "DEF_GLOBAL") {
    skipSpaces();
    return new DefineGlobalInstruction(parseIdentifer());
  } else if (label == "ADD") {
    return new AddInstruction();
  } else if (label == "SUB") {
    return new SubInstruction();
  } else if (label == "MUL") {
    return new MulInstruction();
  } else if (label == "DIV") {
    return new DivInstruction();
  } else if (label == "MOD") {
    return new ModInstruction();
  } else if (label == "RETURN") {
    return new ReturnInstruction();
  } else {
    errorExpected("instruction");
  }

  return nullptr;
}

std::vector<Instruction *> LLParser::parse() {
  std::vector<Instruction *> result;

  Instruction *inst;
  while ((inst = parseNext())) {
    result.push_back(inst);
  }

  return result;
}

}
