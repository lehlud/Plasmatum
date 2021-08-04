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
  plsm_size_t _index = 0, *_jump_index_ptr = &_index;
  instruction *_ip = nullptr;

  std::vector<instruction *> _instructions;

  std::vector<Type *> _types;

  std::vector<function *> functions;

  std::vector<value *> _stack;
  std::vector<value *> _argument_stack;
  std::map<std::string, value *> _globals;

public:
  execution_engine(const std::vector<instruction *> &__instructions,
                   const std::vector<Type *> &__types)
      : _instructions(__instructions), _types(__types) {}

  ~execution_engine();

  inline void register_function(function *_function) {
    functions.push_back(_function);
  }

  inline void stack_push(value *value) { _stack.push_back(value); }

  void stack_push_global(const std::string &id);

  inline void stack_pop_no_delete() { _stack.pop_back(); }

  inline void stack_pop() {
    delete _stack.back();
    stack_pop_no_delete();
  }

  inline value *stack_peek(plsm_size_t back = 0) {
    return _stack[_stack.size() - 1 - back];
  }

  inline void argument_push(value *arg) { _argument_stack.push_back(arg->copy()); }

  inline value *argument_peek(plsm_size_t back = 0) {
    return _argument_stack[_argument_stack.size() - 1 - back]->copy();
  }

  inline void argument_pop() {
    delete _argument_stack.back();
    _argument_stack.pop_back();
  }

  inline void jump(plsm_size_t index) { *_jump_index_ptr = index; }

  inline plsm_size_t *jump_index_ptr() { return _jump_index_ptr; }

  inline void set_jump_index_ptr(plsm_size_t *ptr) {
    this->_jump_index_ptr = ptr;
  }

  inline void reset_jump_index_ptr() { _jump_index_ptr = &_index; }

  inline void define_global(const std::string &id, value *value) {
    _globals[id] = value;
  }

  inline instruction *get_instruction(plsm_size_t index) {
    return index >= _instructions.size() ? nullptr : _instructions[index];
  }

  inline plsm_size_t index() { return _index; }
  inline void set_index(plsm_size_t __index) { this->_index = __index; }

  void call(plsm_size_t argc);

  int execute(const std::vector<std::string> &args);
};

} // namespace plsm
