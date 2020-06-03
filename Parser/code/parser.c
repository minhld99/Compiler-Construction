/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}
// 1
void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}
// 2, 3
void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } 
  else compileBlock2();
  assert("Block parsed!");
}
// 4, 5
void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } 
  else compileBlock3();
}
// 6, 7
void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } 
  else compileBlock4();
}
// 8
void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}
// 9
void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}
// ================================
// void compileDeclarations(void) {
//   switch (lookAhead->tokenType)
//   {
//   case KW_CONST:
//     compileConstDecls();
//     compileDeclarations();
//     break;
//   case KW_VAR:
//     compileVarDecls();
//     compileDeclarations();
//     break;
//   case KW_TYPE:
//     compileTypeDecls();
//     compileDeclarations();
//     break;
//   default:
//     compileSubDecls();
//     compileDeclarations();
//     break;
//   }
// }
// ================================
// void compileVariable(void) {
//   eat(TK_IDENT);
//   while (lookAhead->tokenType==SB_LSEL) {
//     eat(SB_LSEL);
//     compileExpression();
//     eat(SB_RSEL);
//   }
// }
// ================================

// 10, 11
void compileConstDecls(void) {
while (lookAhead->tokenType==TK_IDENT) 
compileConstDecl(); 
}
// 12
void compileConstDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
}
// 13, 14
void compileTypeDecls(void) {
  // TODO
  while (lookAhead->tokenType==TK_IDENT)
    compileTypeDecl();
}
// 15
void compileTypeDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
}
// 16, 17
void compileVarDecls(void) {
  // TODO
  while (lookAhead->tokenType==TK_IDENT)
    compileVarDecl();
}
// 18
void compileVarDecl(void) {
  // TODO
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}
// ================================
// void compileParamlist(void) {
//   if (lookAhead->tokenType==SB_LPAR)
//     eat(SB_LPAR);
//   if (lookAhead->tokenType==KW_VAR)
//     eat(KW_VAR);
//   eat(TK_IDENT);
//   eat(SB_COLON);
//   compileBasicType();
//   if (lookAhead->tokenType==SB_SEMICOLON)
//     compileParamlist();
//   eat(SB_RPAR);
// } 
// ================================
// 19, 20, 21
void compileSubDecls(void) {
  assert("Parsing subtoutines ....");
  // TODO
  if (lookAhead->tokenType==KW_FUNCTION) {
    compileFuncDecl();
    compileSubDecls();
  }
  if (lookAhead->tokenType==KW_PROCEDURE) {
    compileProcDecl();
    compileSubDecls();
  }
  assert("Subtoutines parsed ....");
}
// 22
void compileFuncDecl(void) {
  assert("Parsing a function ....");
  // TODO
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}
// 23
void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  // TODO
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}
// 36, 37, 38
void compileUnsignedConstant(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 40, 41, 42, 43
void compileConstant(void) {
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    compileConstant2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileConstant2();
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    compileConstant2();
    break;
  }
}
// 44, 45
void compileConstant2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 30, 31, 32, 33
void compileType(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case KW_ARRAY:
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);
    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
    break;
  default:
    error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 34, 35
void compileBasicType(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  default:
    error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 24, 25
void compileParams(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_LPAR:
    eat(SB_LPAR);
    compileParam();
    compileParams2();
    eat(SB_RPAR);
    break;
  case SB_COLON:
  case SB_SEMICOLON:
    break;
  default:
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 26, 27
void compileParams2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_SEMICOLON:
    eat(SB_SEMICOLON);
    compileParam();
    compileParams2();
    break;
  case SB_RPAR:
    break;
  default:
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 28, 29
void compileParam(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  case KW_VAR:
    eat(KW_VAR);
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  default:
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 46
void compileStatements(void) {
  // TODO
  compileStatement();
  compileStatements2();
}
// 47, 48
void compileStatements2(void) {
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_SEMICOLON:
    eat(SB_SEMICOLON);
    compileStatement();
    compileStatements2();
    break;
  case KW_END:
    break;
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 49 - 55
void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 56, 57
void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  // TODO
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LSEL) compileIndexes();
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}
// 58
void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // TODO
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  assert("Call statement parsed ....");
}
// 59
void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // TODO
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}
// 60
void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}
// 61, 62
void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}
// 63
void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // TODO
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement pased ....");
}
// 64
void compileForSt(void) {
  assert("Parsing a for statement ....");
  // TODO
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}
// 65, 66
void compileArguments(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_LPAR:
    eat(SB_LPAR);
    compileExpression();
    compileArguments2();
    eat(SB_RPAR);
    break;
  // Follow - same as call statement as statement:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  // Follow - term2
  case SB_TIMES:
  case SB_SLASH:
  // Follow - expression3
  // Follow (For statement)
  case KW_TO:
  case KW_DO:
  // Follow (arguments2)
  case SB_COMMA:
  // Follow (condition2)
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  // Follow (factor)
  case SB_RPAR:
  // Follow (indexes)
  case SB_RSEL:
  // Follow (if statement)
  case KW_THEN:
    break;
  // Error
  default:
    error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 67, 68
void compileArguments2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_COMMA:
    eat(SB_COMMA);
    compileExpression();
    compileArguments2();
    break;
  // Follow
  case SB_RPAR:
    break;
  // Error:
  default:
    error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 68
void compileCondition(void) {
  // TODO
  compileExpression();
  compileCondition2();
}
// 69-74
void compileCondition2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_EQ:
    eat(SB_EQ);
    compileExpression();
    break;
  case SB_NEQ:
    eat(SB_NEQ);
    compileExpression();
    break;
  case SB_LE:
    eat(SB_LE);
    compileExpression();
    break;
  case SB_LT:
    eat(SB_LT);
    compileExpression();
    break;
  case SB_GE:
    eat(SB_GE);
    compileExpression();
    break;
  case SB_GT:
    eat(SB_GT);
    compileExpression();
    break;
  default:
    error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 75 - 77
void compileExpression(void) {
  assert("Parsing an expression");
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    compileExpression2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileExpression2();
  default:
    compileExpression2();
    break;
  }
  assert("Expression parsed");
}
// 78
void compileExpression2(void) {
  // TODO
  compileTerm();
  compileExpression3();
}
// 79 - 81
void compileExpression3(void) {
  // TODO
  switch(lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    compileTerm();
    compileExpression3();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileTerm();
    compileExpression3();
    break;
  // Follow (statement)
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  // Follow (For statement)
  case KW_TO:
  case KW_DO:
  // Follow (arguments2)
  case SB_COMMA:
  // Follow (condition2)
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  // Follow (factor)
  case SB_RPAR:
  // Follow (indexes)
  case SB_RSEL:
  // Follow (if statement)
  case KW_THEN:
    break;
  // Error
  default:
    error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 82
void compileTerm(void) {
  // TODO
  compileFactor();
  compileTerm2();
}
// 83 - 85
void compileTerm2(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case SB_TIMES:
    eat(SB_TIMES);
    compileFactor();
    compileTerm2();
    break;
  case SB_SLASH:
    eat(SB_SLASH);
    compileFactor();
    compileTerm2();
    break;
  // Follow - same as expression3
  case SB_PLUS:
  case SB_MINUS:
  // Follow (statement)
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  // Follow (For statement)
  case KW_TO:
  case KW_DO:
  // Follow (arguments2)
  case SB_COMMA:
  // Follow (condition2)
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  // // Follow (factor)
  case SB_RPAR:
  // Follow (indexes)
  case SB_RSEL:
  // Follow (if statement)
  case KW_THEN:
    break;
  default:
    error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 86 - 89
void compileFactor(void) {
  // TODO
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
  case TK_CHAR:
    compileUnsignedConstant();
    break;
  case SB_LPAR:
    eat(SB_LPAR);
    compileExpression();
    eat(SB_RPAR);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    switch(lookAhead->tokenType) {
    case SB_LSEL:
      compileIndexes();
      break;
    case SB_LPAR:
      compileArguments();
      break;
    default:
      break;
    }
    break;
  default:
    error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
// 92, 93
void compileIndexes(void) {
  // TODO
  if (lookAhead->tokenType == SB_LSEL) {
    eat(SB_LSEL);
    compileExpression();
    eat(SB_RSEL);
    compileIndexes();
  }
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}
