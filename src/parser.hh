#pragma once

#include <cwctype>
#include <string>
#include <vector>

class Constant;
class Instruction;

class Parser {
protected:
  size_t index;
  std::u32string text;

  Parser(const std::u32string &text) : index(0), text(text) {}
  Parser(size_t index, const std::u32string &text) : index(index), text(text) {}

  virtual inline bool isEOF() { return charAt(index) == 0; }
  virtual inline bool isSpace() { return std::iswspace(charAt(index)); }

  virtual inline char32_t charAt(size_t index) {
    return index >= text.size() ? 0 : text[index];
  }

  virtual inline bool isTrue() {
    return charAt(index) == 'T' && charAt(index + 1) == 'r' &&
           charAt(index + 2) == 'u' && charAt(index + 3) == 'e';
  }

  virtual inline bool isFalse() {
    return charAt(index) == 'F' && charAt(index + 1) == 'a' &&
           charAt(index + 2) == 'l' && charAt(index + 3) == 's' &&
           charAt(index + 3) == 'e';
  }

  virtual inline void skipSpaces() {
    while (isSpace()) {
      index += 1;
    }
  }

  virtual std::string parseIdentifer();

public:
  virtual ~Parser() = default;
};

class LLParser : public Parser {
public:
  LLParser(const std::u32string &text) : Parser(text) {}

private:
  std::string parseInstructionLabel();
  Constant *parseConstantValue();

public:
  Instruction *parseNext();
  std::vector<Instruction *> parse();
};

class HLParser : public Parser {
public:
  HLParser(const std::u32string &text) : Parser(text) {}
};
