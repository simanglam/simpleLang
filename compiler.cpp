#include <fstream>
#include "tokenizer.h"
#include "parser.h"
#include "compiler.h"

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

int main() {
    Parser p("test.txt");
    Compiler c;
    
    c.context = new llvm::LLVMContext();
    c.module = new llvm::Module("test.text", *c.context);
    c.builder = new llvm::IRBuilder<>(*c.context);

    llvm::FunctionType* printFT = llvm::FunctionType::get(c.builder->getInt32Ty(), {c.builder->getInt32Ty()}, false);
    llvm::Function* printF = llvm::Function::Create(printFT, llvm::GlobalValue::ExternalLinkage, llvm::Twine("print"), c.module);

    llvm::FunctionType* getFT = llvm::FunctionType::get(c.builder->getInt32Ty(), {}, false);
    llvm::Function* getF = llvm::Function::Create(getFT, llvm::GlobalValue::ExternalLinkage, llvm::Twine("get"), c.module);

    llvm::FunctionType* FT = llvm::FunctionType::get(c.builder->getInt32Ty(), {}, false);
    llvm::Function* F = llvm::Function::Create(FT, llvm::GlobalValue::ExternalLinkage, llvm::Twine("main"), c.module);

    c.builder->SetInsertPoint(llvm::BasicBlock::Create(*c.context, "entry", F));

    ASTNode* a = nullptr;
    while((a = p.parseProgram()) != nullptr){
        a->codeGen(c);
    }
    c.builder->CreateRet(c.builder->getInt32(0));
    c.module->print(llvm::outs(), nullptr);
}