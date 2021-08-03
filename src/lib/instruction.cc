#include "instruction.hh"

#include "engine.hh"
#include "type.hh"
#include "value.hh"

namespace plsm {

#define _DEF_EXECUTE_SKIP(name)                                                \
  plsm_size_t execute_##name(execution_engine *, void *) { return 1; }

#define _DEF_EXECUTE_ARG(name, stmt)                                           \
  plsm_size_t execute_##name(execution_engine *engine, void *arg) {            \
    stmt;                                                                      \
    return 1;                                                                  \
  }

#define _DEF_EXECUTE_BINEXPR(name)                                             \
  plsm_size_t execute_##name(execution_engine *engine, void *) {               \
    engine->stackPeek(1)->type->name(engine);                                  \
    return 1;                                                                  \
  }

_DEF_EXECUTE_SKIP(return )

_DEF_EXECUTE_ARG(jump, engine->jump(*((plsm_size_t *)arg)))

plsm_size_t execute_jump_cond(execution_engine *engine, void *index) {
  plsm_size_t result = 1;
  if (engine->stackPeek()->is_truthy()) {
    engine->jump(*((plsm_size_t *)index));
    result = 0;
  }
  engine->stackPop();
  return result;
}

_DEF_EXECUTE_ARG(load_arg,
                 engine->stackPush(engine->argumentPeek(*((plsm_size_t *)arg))))
_DEF_EXECUTE_ARG(load_const, engine->stackPush((constant *)arg))
_DEF_EXECUTE_ARG(load_global, engine->stackPushGlobal(*((std::string *)arg)))

_DEF_EXECUTE_ARG(cast, ((Type *)arg)->cast(engine, (Type *)arg))

_DEF_EXECUTE_ARG(custom, (*((custom_inst_function *)arg))(engine))

_DEF_EXECUTE_BINEXPR(add)
_DEF_EXECUTE_BINEXPR(sub)
_DEF_EXECUTE_BINEXPR(mul)
_DEF_EXECUTE_BINEXPR(div)
_DEF_EXECUTE_BINEXPR(mod)

_DEF_EXECUTE_BINEXPR(eq)
_DEF_EXECUTE_BINEXPR(ne)
_DEF_EXECUTE_BINEXPR(gt)
_DEF_EXECUTE_BINEXPR(ge)
_DEF_EXECUTE_BINEXPR(lt)
_DEF_EXECUTE_BINEXPR(le)

_DEF_EXECUTE_ARG(call, engine->call(*((plsm_size_t *)arg)))

plsm_size_t execute_func_start(execution_engine *engine, void *argc) {
  std::vector<instruction *> body;

  plsm_size_t index = engine->getIndex() + 1;

  instruction *ip;
  while ((ip = engine->get_instruction(index++))->code !=
         instruction::code_func_finish) {
    body.push_back(ip);
  }

  function *func = new function(*((plsm_size_t *)argc), body);

  engine->registerFunction(func);
  engine->stackPush(new function_pointer(func));

  return 1 + body.size();
}

_DEF_EXECUTE_SKIP(func_finish)

_DEF_EXECUTE_ARG(def_global, {
  engine->defineGlobal(*((std::string *)arg), engine->stackPeek());
  engine->stackPop(false);
})

#define _SET_INST(name)                                                        \
  inst_functions[instruction::code_##name] = execute_##name;

inst_function *get_inst_functions() {
  typedef inst_function inst_f;
  inst_f *inst_functions =
      (inst_f *)std::malloc(instruction::code_max * sizeof(inst_f));

  _SET_INST(return )

  _SET_INST(jump)
  _SET_INST(jump_cond)

  _SET_INST(load_arg)
  _SET_INST(load_const)
  _SET_INST(load_global)

  _SET_INST(cast)

  _SET_INST(custom)

  _SET_INST(add)
  _SET_INST(sub)
  _SET_INST(mul)
  _SET_INST(div)
  _SET_INST(mod)

  _SET_INST(eq)
  _SET_INST(ne)
  _SET_INST(gt)
  _SET_INST(ge)
  _SET_INST(lt)
  _SET_INST(le)

  _SET_INST(call)

  _SET_INST(func_start)
  _SET_INST(func_finish)

  _SET_INST(def_global)

  return inst_functions;
};

const inst_function *instruction::functions = get_inst_functions();

/*
return_inst *return_inst::copy() { return new return_inst(); }
plsm_size_t return_inst::execute(execution_engine *) { return 1; }

jump_inst *jump_inst::copy() { return new jump_inst(destination); }
plsm_size_t jump_inst::execute(execution_engine *engine) {
  engine->jump(destination);
  return 0;
}

jump_cond_inst *jump_cond_inst::copy() {
  return new jump_cond_inst(destination);
}

plsm_size_t jump_cond_inst::execute(execution_engine *engine) {
  plsm_size_t result = 1;
  if (engine->stackPeek()->is_truthy()) {
    engine->jump(destination);
    result = 0;
  }

  engine->stackPop();
  return result;
}

load_const_inst::~load_const_inst() { delete value; }
load_const_inst *load_const_inst::copy() { return new load_const_inst(value); }

plsm_size_t load_const_inst::execute(execution_engine *engine) {
  engine->stackPush(value);
  return 1;
}

load_arg_inst *load_arg_inst::copy() { return new load_arg_inst(back); }
plsm_size_t load_arg_inst::execute(execution_engine *engine) {
  engine->stackPush(engine->argumentPeek(back));
  return 1;
}

load_global_inst *load_global_inst::copy() { return new load_global_inst(id); }
plsm_size_t load_global_inst::execute(execution_engine *engine) {
  engine->stackPushGlobal(id);
  return 1;
}

cast_inst *cast_inst::copy() { return new cast_inst(type); }
plsm_size_t cast_inst::execute(execution_engine *engine) {
  type->cast(engine, engine->stackPeek()->type);
  return 1;
}

custom_inst *custom_inst::copy() { return new custom_inst(executeFunction); }
plsm_size_t custom_inst::execute(execution_engine *engine) {
  return executeFunction(engine);
}

add_inst *add_inst::copy() { return new add_inst(); }
plsm_size_t add_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->add(engine);
  return 1;
}

sub_inst *sub_inst::copy() { return new sub_inst(); }
plsm_size_t sub_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->sub(engine);
  return 1;
}

mul_inst *mul_inst::copy() { return new mul_inst(); }
plsm_size_t mul_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->mul(engine);
  return 1;
}

div_inst *div_inst::copy() { return new div_inst(); }
plsm_size_t div_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->div(engine);
  return 1;
}

mod_inst *mod_inst::copy() { return new mod_inst(); }
plsm_size_t mod_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->mod(engine);
  return 1;
}

eq_inst *eq_inst::copy() { return new eq_inst(); }
plsm_size_t eq_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->eq(engine);
  return 1;
}

ne_inst *ne_inst::copy() { return new ne_inst(); }
plsm_size_t ne_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->ne(engine);
  return 1;
}

gt_inst *gt_inst::copy() { return new gt_inst(); }
plsm_size_t gt_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->gt(engine);
  return 1;
}

ge_inst *ge_inst::copy() { return new ge_inst(); }
plsm_size_t ge_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->ge(engine);
  return 1;
}

lt_inst *lt_inst::copy() { return new lt_inst(); }
plsm_size_t lt_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->lt(engine);
  return 1;
}

le_inst *le_inst::copy() { return new le_inst(); }
plsm_size_t le_inst::execute(execution_engine *engine) {
  engine->stackPeek(1)->type->le(engine);
  return 1;
}

call_inst *call_inst::copy() { return new call_inst(argc); }
plsm_size_t call_inst::execute(execution_engine *engine) {
  engine->call(argc);
  return 1;
}

func_start_inst *func_start_inst::copy() { return new func_start_inst(argc); }
plsm_size_t func_start_inst::execute(execution_engine *engine) {
  std::vector<instruction *> body;

  plsm_size_t index = engine->getIndex() + 1;

  instruction *ip;
  while (!(ip = engine->get_instruction(index++))->is_func_finish()) {
    body.push_back(ip);
  }

  engine->stackPush(new function(argc, body));

  return 1 + body.size();
}

func_finish_inst *func_finish_inst::copy() { return new func_finish_inst(); }
plsm_size_t func_finish_inst::execute(execution_engine *) { return 1; }

def_global_inst *def_global_inst::copy() { return new def_global_inst(id); }
plsm_size_t def_global_inst::execute(execution_engine *engine) {
  engine->defineGlobal(id, engine->stackPeek());
  engine->stackPop(false);
  return 1;
}
*/

} // namespace plsm
