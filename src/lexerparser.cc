#include "error.h"
#include "lexerparser.h"

using namespace Plasmatum;

AST::Expr* Parser::parseExpr(bool topLevel) {
  if (topLevel) {
    if (token.type == TokenType::ID) {

      bool def = false;
      bool lambda = false;

      std::string defId;

      if (token.val == "def") {
        next();

        if (token.type != TokenType::ID)
          Error::parserExpected("identifier", token.val);

        defId = token.val;

        next();

        def = true;
      } else if (token.val == "lambda") {
        next();
      }

      if (def || lambda) {
        if (token.type != TokenType::PO)
          Error::parserExpected("'('", token.val);

        next();

        std::vector<const std::string&> args;
        while (token.type != TokenType::PC) {
          if (token.type != TokenType::ID)
            Error::parserExpected("identifer", token.val);

          args.push_back(token.val);

          next();
        }

        auto body = parseExpr(false);

        next();

        auto base = new AST::Lambda(args, body);

        if (lambda) return base;
        else return new AST::Function(base, defId);
      }
    }
  } else {

  }
}
