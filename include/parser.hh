#pragma once

#include "types.hh"

#include <cwctype>
#include <memory>
#include <string>
#include <vector>

#include "utils.hh"

namespace plsm {

inline bool isUpperAscii(char32_t c) {
  return (c >= 'A' && c <= 'Z') || (c == '_');
}

inline bool isIdChar(char32_t c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_');
}

class Constant;
class Instruction;

class Parser {
protected:
  plsm_size_t index;
  std::string fname;
  std::u32string text;

  Parser(const std::u32string &text, const std::string &fname = "")
      : index(0), fname(fname), text(text) {}
  Parser(plsm_size_t index, const std::u32string &text,
         const std::string &fname = "")
      : index(index), fname(fname), text(text) {}

  virtual inline bool isEOF() { return charAt(index) == 0; }
  virtual inline bool isSpace() { return std::iswspace(charAt(index)); }
  virtual inline bool isSeparator() { return isSpace() || isEOF(); }

  virtual inline char32_t charAt(plsm_size_t index) {
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

  virtual void skipSpaces();

  virtual std::string getPosition(plsm_size_t index);
  virtual inline std::string getPosition() { return getPosition(index); }

  virtual void errorExpected(const std::string &exp);
  virtual inline void maybeSeparatorError(const std::string &after) {
    if (!isSeparator()) {
      errorExpected("separator after " + after);
    }
  }

  virtual plsm_int_t parseInteger();
  virtual std::string parseIdentifer();

public:
  virtual ~Parser() = default;
};

class LLParser : public Parser {
public:
  LLParser(const std::u32string &text, const std::string &fname = "") : Parser(text, fname) {}

  static inline std::shared_ptr<LLParser> get(const std::u32string &text) {
    return std::make_shared<LLParser>(text);
  }

  static inline std::shared_ptr<LLParser> fromFile(const std::string &fname) {
    return std::make_shared<LLParser>(readFile(fname), fname);
  }

private:
  std::string parseInstructionLabel();
  std::shared_ptr<Constant> parseConstantValue();

public:
  std::shared_ptr<Instruction> parseNext();
  std::vector<std::shared_ptr<Instruction>> parse();
};

class HLParser : public Parser {
public:
  HLParser(const std::u32string &text, const std::string &fname = "") : Parser(text, fname) {}

  static inline std::shared_ptr<HLParser> get(const std::u32string &text) {
    return std::make_shared<HLParser>(text);
  }

  static inline std::shared_ptr<HLParser> fromFile(const std::string &fname) {
    return std::make_shared<HLParser>(readFile(fname), fname);
  }
};

} // namespace plsm
