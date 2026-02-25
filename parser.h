#ifndef __Parser_Header__
#define __Parser_Header__

#include "tokenizer.h"
#include "asts.h"

class Parser {
private:
    Tokenizer t;
    ASTNode* parseStatement();
    ASTNode* parseLabel();
    ASTNode* parseIfGoto();
    ASTNode* parseGoto(ASTNode* = nullptr);
    ASTNode* parseDeclare();
    ASTNode* parseIntConstant();
    ASTNode* parseExpression();
    ASTNode* parseBinaryRight(ASTNode*, int);
    ASTNode* parseFactor();
    ASTNode* parseIdentifierExpression();
    ASTNode* parseArgList();
public:
    Parser(string);
    ~Parser();
    ASTNode* parseProgram();
};

#endif