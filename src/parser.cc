#include "parser.hh"

#include <iostream>

#include "value.hh"
#include "utils.hh"
#include "instruction.hh"

std::string Parser::parseIdentifer() {
  std::string tmp;

  char32_t c;
  while (isIdChar((c = charAt(index)))) {
    tmp += c;
    index += 1;
  }

  if (!isSpace()) {
    std::cout << "expected whitespace after identifier" << std::endl;
    std::exit(1);
  }

  return tmp;
}

std::string LLParser::parseInstructionLabel() {
  std::string tmp;

  char32_t c;
  while (isUpperAscii((c = charAt(index)))) {
    tmp += c;
    index += 1;
  }

  if (!isSpace()) {
    std::cout << "expected whitespace after instruction label" << std::endl;
    std::exit(1);
  }

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

    if (!isSpace()) {
      std::cout << "expected whitespace after hex number" << std::endl;
      std::exit(1);
    }

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
    std::cout << "expected value" << std::endl;
    std::exit(1);
  }

  if (charAt(index) != '.') {
    if (!isSpace()) {
      std::cout << "expected whitespace after integer" << std::endl;
      std::exit(1);
    }

    return new IntegerValue(std::stol(tmp));
  } else {
    index += 1;
    while (std::isdigit((c = charAt(index)))) {
      tmp += c;
      index += 1;
    }

    if (!isSpace()) {
      std::cout << "expected whitespace after float" << std::endl;
      std::exit(1);
    }

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
    Constant *c = parseConstantValue();

    if (!c->isInteger()) {
      std::cout << "expected integer constant after 'CALL'" << std::endl;
      std::exit(1);
    }

    return new CallInstruction(c->asInteger());
  } else if (label == "FUNC_START") {
    skipSpaces();
    Constant *c = parseConstantValue();

    if (!c->isInteger()) {
      std::cout << "expected integer constant after 'FUNC_START'" << std::endl;
      std::exit(1);
    }

    return new FunctionStartInstruction(c->asInteger());
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
    std::cout << "expected instruction" << std::endl;
    std::exit(1);
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
