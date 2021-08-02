#pragma once

#include "types.hh"

#include <functional>
#include <memory>
#include <string>

namespace plsm {

class Type;
class Engine;
class Constant;

class Instruction {
public:
  virtual ~Instruction() = default;
  virtual plsm_size_t execute(Engine *engine) = 0;

  virtual inline std::string toString() = 0;

  virtual bool isReturn() { return false; }
  virtual bool isFunctionFinish() { return false; }
};

class ReturnInstruction : public Instruction {
public:
  plsm_size_t execute(Engine *) override { return 1; }

  static inline std::shared_ptr<ReturnInstruction> get() {
    return std::make_shared<ReturnInstruction>();
  }

  virtual inline std::string toString() override { return "RETURN"; }

  bool isReturn() override { return true; }
};

class JumpInstruction : public Instruction {
private:
  plsm_size_t destination;

public:
  JumpInstruction(plsm_size_t destination) : destination(destination) {}

  static inline std::shared_ptr<JumpInstruction> get(plsm_size_t destination) {
    return std::make_shared<JumpInstruction>(destination);
  }

  plsm_size_t execute(Engine *engine) override;

  virtual inline std::string toString() override { return "JUMP"; }

  plsm_size_t getDestination() { return destination; };
};

class LoadConstInstruction : public Instruction {
private:
  std::shared_ptr<Constant> value;

public:
  LoadConstInstruction(std::shared_ptr<Constant> value) : value(value) {}

  static inline std::shared_ptr<LoadConstInstruction>
  get(std::shared_ptr<Constant> value) {
    return std::make_shared<LoadConstInstruction>(value);
  }

  virtual inline std::string toString() override { return "LOAD_CONST"; }

  plsm_size_t execute(Engine *engine) override;
};

class LoadGlobalInstruction : public Instruction {
private:
  std::string id;

public:
  LoadGlobalInstruction(const std::string &id) : id(id) {}

  static inline std::shared_ptr<LoadGlobalInstruction>
  get(const std::string &id) {
    return std::make_shared<LoadGlobalInstruction>(id);
  }

  virtual inline std::string toString() override { return "LOAD_GLOBAL"; }

  plsm_size_t execute(Engine *engine) override;
};

class CastInstruction : public Instruction {
private:
  std::shared_ptr<Type> type;

public:
  CastInstruction(std::shared_ptr<Type> type) : type(type) {}

  static inline std::shared_ptr<CastInstruction>
  get(std::shared_ptr<Type> type) {
    return std::make_shared<CastInstruction>(type);
  }

  virtual inline std::string toString() override { return "CAST"; }

  plsm_size_t execute(Engine *engine) override;
};

class CustomInstruction : public Instruction {
private:
  std::function<plsm_size_t(Engine *)> executeFunction;

public:
  CustomInstruction(const std::function<plsm_size_t(Engine *)> &executeFunction)
      : executeFunction(executeFunction) {}

  static inline std::shared_ptr<CustomInstruction>
  get(const std::function<plsm_size_t(Engine *)> &executeFunction) {
    return std::make_shared<CustomInstruction>(executeFunction);
  }

  virtual inline std::string toString() override { return "CUSTOM"; }

  plsm_size_t execute(Engine *engine) override;
};

class AddInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "ADD"; }

  static inline std::shared_ptr<AddInstruction> get() {
    return std::make_shared<AddInstruction>();
  }

  plsm_size_t execute(Engine *engine) override;
};

class SubInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "SUB"; }

  static inline std::shared_ptr<SubInstruction> get() {
    return std::make_shared<SubInstruction>();
  }

  plsm_size_t execute(Engine *engine) override;
};

class MulInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "MUL"; }

  static inline std::shared_ptr<MulInstruction> get() {
    return std::make_shared<MulInstruction>();
  }

  plsm_size_t execute(Engine *engine) override;
};

class DivInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "DIV"; }

  static inline std::shared_ptr<DivInstruction> get() {
    return std::make_shared<DivInstruction>();
  }

  plsm_size_t execute(Engine *engine) override;
};

class ModInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "MOD"; }

  static inline std::shared_ptr<ModInstruction> get() {
    return std::make_shared<ModInstruction>();
  }

  plsm_size_t execute(Engine *engine) override;
};

class CallInstruction : public Instruction {
private:
  plsm_size_t argc;

public:
  CallInstruction(plsm_size_t argc) : argc(argc) {}

  static inline std::shared_ptr<CallInstruction> get(plsm_size_t argc) {
    return std::make_shared<CallInstruction>(argc);
  }

  virtual inline std::string toString() override { return "CALL"; }

  plsm_size_t execute(Engine *engine) override;
};

class FunctionStartInstruction : public Instruction {
private:
  plsm_size_t argc;

public:
  FunctionStartInstruction(plsm_size_t argc) : argc(argc) {}

  static inline std::shared_ptr<FunctionStartInstruction>
  get(plsm_size_t argc) {
    return std::make_shared<FunctionStartInstruction>(argc);
  }

  virtual inline std::string toString() override { return "FUNC_START"; }

  plsm_size_t execute(Engine *engine) override;
};

class FunctionFinishInstruction : public Instruction {
public:
  virtual inline std::string toString() override { return "FUNC_FINISH"; }

  static inline std::shared_ptr<FunctionFinishInstruction> get() {
    return std::make_shared<FunctionFinishInstruction>();
  }

  plsm_size_t execute(Engine *) override { return 1; }

  bool isFunctionFinish() override { return true; }
};

class DefineGlobalInstruction : public Instruction {
private:
  std::string id;

public:
  DefineGlobalInstruction(const std::string &id) : id(id) {}

  static inline std::shared_ptr<DefineGlobalInstruction>
  get(const std::string &id) {
    return std::make_shared<DefineGlobalInstruction>(id);
  }

  virtual inline std::string toString() override { return "DEF_GLOBAL"; }

  plsm_size_t execute(Engine *engine) override;
};

} // namespace plsm
