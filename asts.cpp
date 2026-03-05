#include "asts.h"
#include "compiler.h"

#include <string>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

using namespace std;
using namespace llvm;

LabelNode::LabelNode(string _id): id(_id) { }

Value* LabelNode::codeGen(Compiler& c) {
    c.lables[id] = BasicBlock::Create(*c.context, Twine(id.c_str()), c.module->getFunction("main"), nullptr);
    if (c.pending.count(id) != 0) {
        for (BranchInst* bi : c.pending[id]){
            bi->setSuccessor(0, c.lables[id]);
        }
    }
    c.pending[id].clear();
    c.pending.erase(id);
    c.builder->CreateBr(c.lables[id]);
    c.builder->SetInsertPoint(c.lables[id]);
    return c.lables[id];
}


GotoNode::GotoNode(std::string _targetLabel, ASTNode* _cond): targetLabel(_targetLabel), cond(_cond) { }

Value* GotoNode::codeGen(Compiler& c) {
    BranchInst* ins = nullptr;
    BasicBlock* TargetBlock = c.lables[targetLabel];
    if (!TargetBlock)
        TargetBlock = c.module->getFunction("main")->getEntryBlock().getNextNode();
    if (!cond)
        ins = c.builder->CreateBr(TargetBlock);
    else {
        BasicBlock* elseBlock = BasicBlock::Create(*c.context, targetLabel + "Else", c.module->getFunction("main"));
        // BasicBlock* elseBlock;
        ins = c.builder->CreateCondBr(
            c.builder->CreateICmpNE(cond->codeGen(c), c.builder->getInt32(0), "cmpTemp"),
            TargetBlock,
            elseBlock
        );
        c.builder->SetInsertPoint(elseBlock);
    }
    if (c.lables[targetLabel] == nullptr)
        c.pending[targetLabel].push_back(ins);
    return ins;
}

BinaryExpressionNode::BinaryExpressionNode(std::string _op, ASTNode* _lhs, ASTNode* _rhs): op(_op), lhs(_lhs), rhs(_rhs) { }

llvm::Value* BinaryExpressionNode::codeGen(Compiler& c) {
    if (op == "=") {
        AllocaInst* var = c.vars[((IdExpressionNode*)lhs)->getId()];
        return c.builder->CreateStore(rhs->codeGen(c), var);
    }
    Value* lhsValue = lhs->codeGen(c);
    Value* rhsValue = rhs->codeGen(c);
    if (op == "+")
        return c.builder->CreateAdd(lhsValue, rhsValue, "addTemp");
    else if (op == "-")
        return c.builder->CreateSub(lhsValue, rhsValue, "subTemp");
    else if (op == "*")
        return c.builder->CreateMul(lhsValue, rhsValue, "mulTemp");
    else if (op == "/")
        return c.builder->CreateSDiv(lhsValue, rhsValue, "divTemp");
    
}

DeclareNode::DeclareNode(string _id, ASTNode* _initVal): id(_id), initVal(_initVal) { }

llvm::Value* DeclareNode::codeGen(Compiler& c) {
    llvm::IRBuilder<> tempBuilder(&c.module->getFunction("main")->getEntryBlock(), c.module->getFunction("main")->getEntryBlock().begin());
    AllocaInst* var = tempBuilder.CreateAlloca(tempBuilder.getInt32Ty(), nullptr, id);
    c.vars[id] = var;
    if (initVal)
        c.builder->CreateStore(initVal->codeGen(c), var);
    return var;
}

IntConstantNode::IntConstantNode(int _val): val(_val) { }

llvm::Value* IntConstantNode::codeGen(Compiler& c) {
    return ConstantInt::get(c.builder->getInt32Ty(), val, true);
}

IdExpressionNode::IdExpressionNode(string _id): id(_id) { }

llvm::Value* IdExpressionNode::codeGen(Compiler& c) {
    return c.builder->CreateLoad(c.builder->getInt32Ty(), c.vars[id], id);
}

string IdExpressionNode::getId() const {
    return id;
}

FuntionCallExpressionNode::FuntionCallExpressionNode(string _id, vector<ASTNode*>& _args): id(_id), args(_args) { }

Value* FuntionCallExpressionNode::codeGen(Compiler& c) {
    vector<Value*> argVal;
    for (auto i : args)
        argVal.push_back(i->codeGen(c));
    return c.builder->CreateCall(c.module->getFunction(id), argVal, "callTemp");
}
