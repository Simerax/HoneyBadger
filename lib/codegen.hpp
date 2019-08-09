#pragma once
#include"visitor.hpp"
#include"ast.hpp"

#include"llvm/IR/Value.h"
#include"llvm/IR/Type.h"
#include"llvm/IR/LLVMContext.h"
#include"llvm/IR/IRBuilder.h"
#include"llvm/IR/Module.h"
#include"llvm/IR/Constants.h"

namespace HoneyBadger {

    class CodeGenerator : public Visitor{
        private:
            std::unique_ptr<llvm::LLVMContext> context;
            std::unique_ptr<llvm::IRBuilder<>> builder;
            std::unique_ptr<llvm::Module> module;
            std::map<std::string, llvm::Value*> named_values;
        public:

        CodeGenerator() {
            context = llvm::make_unique<llvm::LLVMContext>();
            builder = llvm::make_unique<llvm::IRBuilder<>>(context);
        }

        void visit(AST::FunctionSignature &fs) {
            //TODO: types right now we only have doubles so everything is a double
            auto args = fs.get_args();
            std::vector<llvm::Type*> Doubles(args.size(), llvm::Type::getDoubleTy(*context));

            llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(*context), Doubles, false);
            llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, fs.get_name(), module.get());
        }

        void visit(AST::Number &n) {
            llvm::Value* x = llvm::ConstantFP::get(*context, llvm::APFloat(n.get_number()));
        }

        virtual void visit(AST::Node &n) = 0;
        virtual void visit(AST::Variable &n) = 0;
        virtual void visit(AST::Number &n) = 0;
        virtual void visit(AST::BinaryExpr &n) = 0;
        virtual void visit(AST::FunctionCall &n) = 0;
        virtual void visit(AST::FunctionSignature &n) = 0;
        virtual void visit(AST::Function &n) = 0;
    };
}