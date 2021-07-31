#pragma once

#include <map>
#include <string>
#include <vector>

#include "types.hh"

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
  Engine(const std::vector<Type *> &types, const std::vector<Instruction *> &instructions)
      : types(types), instructions(instructions) {}

  Engine(const std::vector<Type *> &types, const std::vector<Value *> &stack,
         const std::vector<Instruction *> &instructions)
      : types(types), stack(stack), instructions(instructions) {}

  inline void stack_push(Value *value) { stack.push_back(value); }
  inline Value *stack_pop() {
    Value *result = stack_peek();
    stack.pop_back();
    return result;
  }

  inline Value *stack_peek() { return stack.back(); }
  inline Value *stack_peek(plsm_size_t back) {
    return stack[stack.size() - 1 - back];
  }

  inline void stack_push_global(const std::string &id) {
    stack.push_back(globals[id]);
  }

  inline void jump(size_t index) { ip = getInstruction((this->index = index)); }

  inline Instruction *getInstruction(plsm_size_t index) {
    return index >= instructions.size() ? nullptr : instructions[index];
  }

  void call(size_t argc);

  int execute(const std::vector<std::string> &args);
};