#pragma once

#include "types.hh"

#include <string>

class Type;
class Engine;
class Constant;

class Instruction {
public:
  virtual ~Instruction() = default;
  virtual fast_size_t execute(Engine *engine) = 0;

  virtual bool isReturn() { return false; }
};

class ReturnInstruction : public Instruction {
public:
  fast_size_t execute(Engine *engine) override;

  bool isReturn() override { return true; }
};

class JumpInstruction : public Instruction {
private:
  fast_size_t destination;

public:
  JumpInstruction(fast_size_t destination) : destination(destination) {}

  fast_size_t execute(Engine *engine) override;

  fast_size_t getDestination() { return destination; };
};

class LoadConstInstruction : public Instruction {
private:
  Constant *value;

public:
  LoadConstInstruction(Constant *value) : value(value) {}
  
  fast_size_t execute(Engine *engine) override;
};

class LoadGlobalInstruction : public Instruction {
private:
  std::string id;

public:
  LoadGlobalInstruction(const std::string &id) : id(id) {}
  
  fast_size_t execute(Engine *engine) override;
};

class CastInstruction : public Instruction {
private:
  Type *type;

public:
  CastInstruction(Type *type) : type(type) {}

  fast_size_t execute(Engine *engine) override;
};

class AddInstruction : public Instruction {
public:
  fast_size_t execute(Engine *engine) override;
};

class SubInstruction : public Instruction {
public:
  fast_size_t execute(Engine *engine) override;
};

class MulInstruction : public Instruction {
public:
  fast_size_t execute(Engine *engine) override;
};

class DivInstruction : public Instruction {
public:
  fast_size_t execute(Engine *engine) override;
};

class ModInstruction : public Instruction {
public:
  fast_size_t execute(Engine *engine) override;
};
