#pragma once

#include "types.hh"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "value.hh"

namespace plsm {

class Type;
class instruction;

class execution_engine {
private:
  plsm_size_t index = 0, *jumpIndexPointer = &index;
  instruction *ip = nullptr;

  std::vector<instruction *> instructions;

  std::vector<std::shared_ptr<Type>> types;

  std::vector<function *> functions;

  std::vector<value *> stack;
  std::vector<value *> argumentStack;
  std::map<std::string, value *> globals;

public:
  execution_engine(const std::vector<instruction *> &instructions,
                   const std::vector<std::shared_ptr<Type>> &types)
      : instructions(instructions), types(types) {}
  
  inline void registerFunction(function *_function) {
    functions.push_back(_function);
  }

  inline void stackPush(value *value) {
    stack.push_back(value);
  }

  inline void stackPop(bool del = true) {
    if (del) {
      delete stack.back();
    }
    stack.pop_back();
  }

  inline value *stackPeek(plsm_size_t back = 0) {
    return stack[stack.size() - 1 - back];
  }

  inline void agumentPushStack() {
    argumentPush(stackPeek());
    stackPop();
  }

  inline void argumentPush(value *arg) {
    argumentStack.push_back(arg->copy());
  }

  inline value *argumentPeek(plsm_size_t back = 0) {
    return argumentStack[argumentStack.size() - 1 - back]->copy();
  }

  inline void argumentPop() {
    delete argumentStack.back();
    argumentStack.pop_back();
  }

  inline void jump(plsm_size_t index) { *jumpIndexPointer = index; }

  inline plsm_size_t getJumpIndex() { return *jumpIndexPointer; }

  inline void defineGlobal(const std::string &id, value *value) {
    globals[id] = value;
  }

  inline instruction *get_instruction(plsm_size_t index) {
    return index >= instructions.size() ? nullptr : instructions[index];
  }

  inline plsm_size_t *getJumpIndexPointer() { return jumpIndexPointer; }

  inline void setJumpIndexPointer(plsm_size_t *ptr) {
    this->jumpIndexPointer = ptr;
  }

  inline void resetJumpIndexPointer() { jumpIndexPointer = &index; }

  inline plsm_size_t getIndex() { return index; }
  inline void setIndex(plsm_size_t index) { this->index = index; }

  void stackPushGlobal(const std::string &id);

  void call(plsm_size_t argc);

  int execute(const std::vector<std::string> &args);
};

} // namespace plsm
