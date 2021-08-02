#pragma once

#include "types.hh"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace plsm {

class Type;
class Value;
class Instruction;

class Engine {
private:
  plsm_size_t index = 0, *jumpIndexPointer = &index;
  std::shared_ptr<Instruction> ip = nullptr;

  std::vector<std::shared_ptr<Type>> types;
  std::vector<std::shared_ptr<Value>> stack;
  std::vector<std::shared_ptr<Value>> argumentStack;
  std::vector<std::shared_ptr<Instruction>> instructions;

  std::map<std::string, std::shared_ptr<Value>> globals;

public:
  Engine(const std::vector<std::shared_ptr<Type>> &types,
         const std::vector<std::shared_ptr<Instruction>> &instructions)
      : types(types), instructions(instructions) {}

  inline void stackPush(const std::shared_ptr<Value> &value) {
    stack.push_back(value);
  }

  inline void stackPop() { stack.pop_back(); }

  inline std::shared_ptr<Value> stackPeek(plsm_size_t back = 0) {
    return stack[stack.size() - 1 - back];
  }

  inline void argumentPush(const std::shared_ptr<Value> &arg,
                           bool isMutable = false) {
    argumentStack.push_back(isMutable ? arg : std::move(arg));
  }

  inline std::shared_ptr<Value> argumentPeek(plsm_size_t back = 0) {
    return argumentStack[argumentStack.size() - 1 - back];
  }

  inline void argumentPop() { argumentStack.pop_back(); }

  inline void jump(plsm_size_t index) { *jumpIndexPointer = index; }

  inline plsm_size_t getJumpIndex() { return *jumpIndexPointer; }

  inline void defineGlobal(const std::string &id,
                           std::shared_ptr<Value> value) {
    globals[id] = value;
  }

  inline std::shared_ptr<Instruction> getInstruction(plsm_size_t index) {
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
