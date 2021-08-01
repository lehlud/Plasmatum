#pragma once

#include "types.hh"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

class Type;
class Value;
class Instruction;

class Engine {
private:
  plsm_size_t index = 0;
  Instruction *ip = nullptr;

  std::vector<Type *> types;
  std::vector<Value *> stack;
  std::vector<Instruction *> instructions;

  std::map<std::string, Value *> globals;

public:
  Engine(const std::vector<Type *> &types,
         const std::vector<Instruction *> &instructions)
      : types(types), instructions(instructions) {}

  Engine(const std::vector<Type *> &types, const std::vector<Value *> &stack,
         const std::vector<Instruction *> &instructions)
      : types(types), stack(stack), instructions(instructions) {}

  inline void stackPush(Value *value) { stack.push_back(value); }

  inline void stackPopVoid() { stack.pop_back(); }

  inline void stackPopVoid(plsm_size_t count) {
    for (plsm_size_t i = 0; i < count; i++) {
      stack.pop_back();
    }
  }

  inline Value *stackPop() {
    Value *result = stackPeek();
    stack.pop_back();
    return result;
  }

  inline std::vector<Value *> stackPop(plsm_size_t count) {
    std::vector<Value *> result;
    for (plsm_size_t i = 0; i < count; i++) {
      result.push_back(stackPop());
    }
    std::reverse(result.begin(), result.end());
    return result;
  }

  inline Value *stackPeek() { return stack.back(); }
  inline Value *stackPeek(plsm_size_t back) {
    return stack[stack.size() - 1 - back];
  }

  inline void stackPushGlobal(const std::string &id) {
    stack.push_back(globals[id]);
  }

  inline void jump(size_t index) { ip = getInstruction((this->index = index)); }

  inline void defineGlobal(const std::string &id, Value *value) {
    globals[id] = value;
  }

  inline Instruction *getInstruction(plsm_size_t index) {
    return index >= instructions.size() ? nullptr : instructions[index];
  }

  inline plsm_size_t getIndex() { return index; }
  inline void setIndex(plsm_size_t index) { this->index = index; }

  void call(size_t argc);

  int execute(const std::vector<std::string> &args);
};