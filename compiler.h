#ifndef __Compiler_Header__
#define __Compiler_Header__

#include <map>
#include <string>
#include <vector>

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

class Compiler {
public:
    llvm::LLVMContext* context;
    llvm::Module* module;
    llvm::IRBuilder<>* builder;
    std::map<std::string, llvm::AllocaInst*> vars;
    std::map<std::string, std::vector<llvm::BranchInst*>> pending;
    std::map<std::string, llvm::BasicBlock*> lables;
};

#endif