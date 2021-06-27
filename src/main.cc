/*
void setBinOpPrecedence() {
  AST::BinOpPrecedence = {
      // logical operators
      {AST::BinExpr::Type::LOR, 10},
      {AST::BinExpr::Type::LAND, 10},
      {AST::BinExpr::Type::EQ, 10},
      {AST::BinExpr::Type::NE, 10},
      {AST::BinExpr::Type::GT, 10},
      {AST::BinExpr::Type::LT, 10},
      {AST::BinExpr::Type::GE, 10},
      {AST::BinExpr::Type::LE, 10},
      // bitwise operators
      {AST::BinExpr::Type::OR, 20},
      {AST::BinExpr::Type::AND, 20},
      // arithmetic operators
      {AST::BinExpr::Type::ADD, 30},
      {AST::BinExpr::Type::SUB, 30},
      {AST::BinExpr::Type::MUL, 31},
      {AST::BinExpr::Type::DIV, 31},
      {AST::BinExpr::Type::MOD, 31},
      {AST::BinExpr::Type::POW, 32},
  };
}
*/



void initializeModule() {

}

int main(int argc, char **argv) {
  if (argc <= 1) {
    // error here
  }

  initializeModule();

  return 0;
}
