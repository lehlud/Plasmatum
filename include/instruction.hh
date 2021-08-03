#pragma once

#include "types.hh"

#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "type.hh"

namespace plsm {

class Type;
class value;
class execution_engine;

typedef plsm_size_t (*inst_function)(execution_engine *, void *);
typedef std::function<plsm_size_t(execution_engine *)> custom_inst_function;

#define _DEF_INST_CONST(name, value)                                           \
  static constexpr plsm_inst_t code_##name = value;

class instruction {
protected:
  static const inst_function *functions;

public:
  const plsm_inst_t code;

  instruction(const plsm_inst_t code) : code(code) {}
  virtual ~instruction() = default;

  virtual instruction *copy() { return new instruction(code); }

  virtual plsm_size_t execute(execution_engine *engine) {
    return functions[code](engine, nullptr);
  }

  _DEF_INST_CONST(return, 10)

  _DEF_INST_CONST(jump, 20)
  _DEF_INST_CONST(jump_cond, 21)

  _DEF_INST_CONST(load_arg, 30)
  _DEF_INST_CONST(load_const, 31)
  _DEF_INST_CONST(load_global, 32)

  _DEF_INST_CONST(cast, 40)

  _DEF_INST_CONST(custom, 50)

  _DEF_INST_CONST(add, 60)
  _DEF_INST_CONST(sub, 61)
  _DEF_INST_CONST(mul, 62)
  _DEF_INST_CONST(div, 63)
  _DEF_INST_CONST(mod, 64)

  _DEF_INST_CONST(eq, 80)
  _DEF_INST_CONST(ne, 81)
  _DEF_INST_CONST(gt, 82)
  _DEF_INST_CONST(ge, 83)
  _DEF_INST_CONST(lt, 84)
  _DEF_INST_CONST(le, 85)

  _DEF_INST_CONST(call, 90)

  _DEF_INST_CONST(func_start, 100)
  _DEF_INST_CONST(func_finish, 101)

  _DEF_INST_CONST(def_global, 110)

  _DEF_INST_CONST(max, 110)
};

} // namespace plsm

#include "value.hh"

namespace plsm {

#define _VALUE_INST_BODY_NO_DESTRUCTOR(T)                                      \
private:                                                                       \
  T *t_value;                                                                  \
                                                                               \
public:                                                                        \
  value_inst(const plsm_size_t code, T *t_value)                               \
      : instruction(code), t_value(t_value) {}                                 \
                                                                               \
  virtual plsm_size_t execute(execution_engine *engine) override {             \
    return functions[code](engine, (void *)t_value);                           \
  }

#define _VALUE_INST_BODY(T)                                                    \
  _VALUE_INST_BODY_NO_DESTRUCTOR(T)                                            \
public:                                                                        \
  ~value_inst() { delete t_value; }

template <typename T> class value_inst : public instruction {
  _VALUE_INST_BODY(T)
public:
  value_inst<T> *copy() override {
    std::cout << "value_inst<T>::copy() not defined properly" << std::endl;
    std::exit(1);
    return nullptr;
  }
};

template <> class value_inst<std::string> : public instruction {
  _VALUE_INST_BODY(std::string)
public:
  value_inst<std::string> *copy() override {
    return new value_inst<std::string>(code, new std::string(*t_value));
  }
};

template <> class value_inst<value> : public instruction {
  _VALUE_INST_BODY(value)
public:
  value_inst<value> *copy() override {
    return new value_inst<value>(code, t_value->copy());
  }
};

template <> class value_inst<plsm_size_t> : public instruction {
  _VALUE_INST_BODY(plsm_size_t)
public:
  value_inst<plsm_size_t> *copy() override {
    plsm_size_t *__tmp = new plsm_size_t;
    *__tmp = *t_value;
    return new value_inst<plsm_size_t>(code, __tmp);
  }
};

template <> class value_inst<Type> : public instruction {
  _VALUE_INST_BODY_NO_DESTRUCTOR(Type)
public:
  virtual ~value_inst<Type>() = default;
  value_inst<Type> *copy() override { return new value_inst(code, t_value); }
};

/*
class instruction {
public:
  virtual ~instruction() = default;

  virtual plsm_size_t execute(execution_engine *engine) = 0;

  virtual instruction *copy() = 0;

  virtual inline std::string to_string() = 0;

  virtual inline bool is_return() { return false; }
  virtual inline bool is_func_finish() { return false; }
};

class return_inst : public instruction {
public:
  return_inst *copy() override;

  inline std::string to_string() override { return "RETURN"; }

  plsm_size_t execute(execution_engine *) override;

  virtual bool is_return() override { return true; }
};

class jump_inst : public instruction {
private:
  plsm_size_t destination;

public:
  jump_inst(plsm_size_t destination) : destination(destination) {}
  jump_inst *copy() override;

  inline std::string to_string() override {
    return "JUMP " + std::to_string(destination);
  }

  plsm_size_t getDestination() { return destination; };

  plsm_size_t execute(execution_engine *engine) override;
};

class jump_cond_inst : public instruction {
private:
  plsm_size_t destination;

public:
  jump_cond_inst(plsm_size_t destination) : destination(destination) {}

  jump_cond_inst *copy() override;

  inline std::string to_string() override {
    return "JUMP_COND " + std::to_string(destination);
  }

  inline plsm_size_t getDestination() { return destination; }

  plsm_size_t execute(execution_engine *engine) override;
};

class load_const_inst : public instruction {
private:
  constant *value;

public:
  load_const_inst(constant *value) : value(value) {}
  ~load_const_inst();

  load_const_inst *copy() override;

  inline std::string to_string() override { return "LOAD_CONST"; }

  plsm_size_t execute(execution_engine *engine) override;
};

class load_arg_inst : public instruction {
private:
  plsm_size_t back;

public:
  load_arg_inst(plsm_size_t back) : back(back) {}

  load_arg_inst *copy() override;

  inline std::string to_string() override { return "LOAD_ARG"; }

  plsm_size_t execute(execution_engine *engine) override;
};

class load_global_inst : public instruction {
private:
  std::string id;

public:
  load_global_inst(const std::string &id) : id(id) {}

  load_global_inst *copy() override;

  inline std::string to_string() override { return "LOAD_GLOBAL"; }

  plsm_size_t execute(execution_engine *engine) override;
};

class cast_inst : public instruction {
private:
  std::shared_ptr<Type> type;

public:
  cast_inst(std::shared_ptr<Type> type) : type(type) {}

  cast_inst *copy() override;

  inline std::string to_string() override { return "CAST"; }

  plsm_size_t execute(execution_engine *engine) override;
};

class custom_inst : public instruction {
private:
  std::function<plsm_size_t(execution_engine *)> executeFunction;

public:
  custom_inst(
      const std::function<plsm_size_t(execution_engine *)> &executeFunction)
      : executeFunction(executeFunction) {}

  custom_inst *copy() override;

  inline std::string to_string() override { return "CUSTOM"; }

  plsm_size_t execute(execution_engine *engine) override;
};

class add_inst : public instruction {
public:
  inline std::string to_string() override { return "ADD"; }
  add_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class sub_inst : public instruction {
public:
  inline std::string to_string() override { return "SUB"; }
  sub_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class mul_inst : public instruction {
public:
  inline std::string to_string() override { return "MUL"; }
  mul_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class div_inst : public instruction {
public:
  inline std::string to_string() override { return "DIV"; }
  div_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class mod_inst : public instruction {
public:
  inline std::string to_string() override { return "MOD"; }
  mod_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class eq_inst : public instruction {
public:
  inline std::string to_string() override { return "EQ"; }
  eq_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class ne_inst : public instruction {
public:
  inline std::string to_string() override { return "NE"; }
  ne_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class gt_inst : public instruction {
public:
  inline std::string to_string() override { return "GT"; }
  gt_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class ge_inst : public instruction {
public:
  inline std::string to_string() override { return "GE"; }
  ge_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class lt_inst : public instruction {
public:
  inline std::string to_string() override { return "LT"; }
  lt_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class le_inst : public instruction {
public:
  inline std::string to_string() override { return "LE"; }
  le_inst *copy() override;

  plsm_size_t execute(execution_engine *engine) override;
};

class call_inst : public instruction {
private:
  plsm_size_t argc;

public:
  call_inst(plsm_size_t argc) : argc(argc) {}

  call_inst *copy() override;

  inline std::string to_string() override { return "CALL"; }

  plsm_size_t execute(execution_engine *engine) override;
};

class func_start_inst : public instruction {
private:
  plsm_size_t argc;

public:
  func_start_inst(plsm_size_t argc) : argc(argc) {}
  func_start_inst *copy() override;

  inline std::string to_string() override { return "FUNC_START"; }

  plsm_size_t execute(execution_engine *engine) override;
};

class func_finish_inst : public instruction {
public:
  inline std::string to_string() override { return "FUNC_FINISH"; }

  func_finish_inst *copy() override;

  plsm_size_t execute(execution_engine *) override;

  virtual bool is_func_finish() override { return true; }
};

class def_global_inst : public instruction {
private:
  std::string id;

public:
  def_global_inst(const std::string &id) : id(id) {}
  def_global_inst *copy() override;

  inline std::string to_string() override { return "DEF_GLOBAL"; }

  plsm_size_t execute(execution_engine *engine) override;
};
*/

} // namespace plsm
