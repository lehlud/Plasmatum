#pragma once

#include "types.hh"

#include <algorithm>
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
  plsm_size_t index = 0;
  std::shared_ptr<Instruction> ip = nullptr;

  std::vector<std::shared_ptr<Type>> types;
  std::vector<std::shared_ptr<Value>> stack;
  std::vector<std::shared_ptr<Instruction>> instructions;

  std::map<std::string, std::shared_ptr<Value>> globals;

public:
  Engine(const std::vector<std::shared_ptr<Type>> &types,
         const std::vector<std::shared_ptr<Instruction>> &instructions)
      : types(types), instructions(instructions) {}

  Engine(const std::vector<std::shared_ptr<Type>> &types,
         const std::vector<std::shared_ptr<Value>> &stack,
         const std::vector<std::shared_ptr<Instruction>> &instructions)
      : types(types), stack(stack), instructions(instructions) {}

  inline void stackPush(std::shared_ptr<Value> value) { stack.push_back(value); }

  inline void stackPopVoid() { stack.pop_back(); }

  inline void stackPopVoid(plsm_size_t count) {
    for (plsm_size_t i = 0; i < count; i++) {
      stack.pop_back();
    }
  }

  inline std::shared_ptr<Value> stackPop() {
    std::shared_ptr<Value> result = stackPeek();
    stack.pop_back();
    return result;
  }

  inline std::vector<std::shared_ptr<Value>> stackPop(plsm_size_t count) {
    std::vector<std::shared_ptr<Value> > result;
    for (plsm_size_t i = 0; i < count; i++) {
      result.push_back(stackPop());
    }
    std::reverse(result.begin(), result.end());
    return result;
  }

  inline std::shared_ptr<Value> stackPeek() { return stack.back(); }
  inline std::shared_ptr<Value> stackPeek(plsm_size_t back) {
    return stack[stack.size() - 1 - back];
  }

  inline void jump(size_t index) { ip = getInstruction((this->index = index)); }

  inline void defineGlobal(const std::string &id, std::shared_ptr<Value> value) {
    globals[id] = value;
  }

  inline std::shared_ptr<Instruction> getInstruction(plsm_size_t index) {
    return index >= instructions.size() ? nullptr : instructions[index];
  }

  inline plsm_size_t getIndex() { return index; }
  inline void setIndex(plsm_size_t index) { this->index = index; }

  void stackPushGlobal(const std::string &id);

  void call(size_t argc);

  int execute(const std::vector<std::string> &args);
};

} // namespace plsm
