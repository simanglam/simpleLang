
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

    ASTNode* a = nullptr;
    while ((a = p.parseProgram()));
    
}