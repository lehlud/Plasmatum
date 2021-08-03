#pragma once

#include "types.hh"

#include <vector>

namespace plsm {

class instruction;
class execution_engine;

class function {
private:
  plsm_size_t argc;
  std::vector<instruction *> instructions;

public:
  function(plsm_size_t argc, const std::vector<instruction *> &instructions)
      : argc(argc), instructions(instructions) {}

  ~function();

  inline plsm_size_t get_argc() { return argc; }
  inline instruction *get_instruction(plsm_size_t index) {
    return instructions[index];
  }

  void call(execution_engine *engine);
};

} // namespace plsm
