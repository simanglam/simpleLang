#ifndef __ASTS_Header__
#define __ASTS_Header__
#include "compiler.h"

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <string>


class ASTNode {
public:
    ASTNode() { };
    friend std::ofstream& operator<<(std::ostream&, ASTNode*);
    virtual llvm::Value* codeGen(Compiler&) = 0;
    virtual void print(std::ostream&) = 0;
};

class LabelNode : public ASTNode{
private:
    std::string id;
public:
    LabelNode(std::string);
    llvm::Value* codeGen(Compiler&) override;
    void print(std::ostream&) override;
};

class GotoNode : public ASTNode{
private:
    std::string targetLabel;
    ASTNode* cond;
public:
    GotoNode(std::string, ASTNode* = nullptr);
    llvm::Value* codeGen(Compiler&) override;
    void print(std::ostream&) override;
};

class BinaryExpressionNode : public ASTNode {
private:
    std::string op;
    ASTNode* lhs, *rhs;
public:
    BinaryExpressionNode(std::string, ASTNode*, ASTNode*);
    llvm::Value* codeGen(Compiler&) override;
    void print(std::ostream&) override;
};

class DeclareNode : public ASTNode{
private:
    std::string id;
    ASTNode* initVal;
public:
    DeclareNode(std::string, ASTNode* = nullptr);
    llvm::Value* codeGen(Compiler&) override;
    void print(std::ostream&) override;
};

class IntConstantNode : public ASTNode{
private:
    int val;
public:
    IntConstantNode(int);
    llvm::Value* codeGen(Compiler&) override;
    void print(std::ostream&) override;
};
class IdExpressionNode : public ASTNode {
private:
    std::string id;
public:
    IdExpressionNode(std::string);
    llvm::Value* codeGen(Compiler&) override;
    std::string getId() const;
    void print(std::ostream&) override;
};

class FuntionCallExpressionNode : public ASTNode {
private:
    std::string id;
    std::vector<ASTNode*> args;
public:
    FuntionCallExpressionNode(std::string, std::vector<ASTNode*>&);
    llvm::Value* codeGen(Compiler&) override;
    void print(std::ostream&) override;
};



#endif