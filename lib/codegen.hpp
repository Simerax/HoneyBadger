#pragma once
#include "visitor.hpp"
#include "ast.hpp"

#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

namespace HoneyBadger
{
class CodeGenerator : public Visitor
{
private:
    llvm::LLVMContext *context;
    llvm::IRBuilder<> *builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value *> named_values;

    llvm::Value *current_value = nullptr;
    llvm::Function *current_function = nullptr;

    llvm::Function *main_function = nullptr;

public:
    CodeGenerator()
    {
        context = new llvm::LLVMContext();
        builder = new llvm::IRBuilder<>(*context);
        module = llvm::make_unique<llvm::Module>("my jiiiiiiiiit reeeeeeeeeee", *context);
    }

    std::unique_ptr<llvm::Module> get_result()
    {
        module->print(llvm::errs(), nullptr);
        return std::move(module);
    }

    void visit(AST::FunctionTable &n) {
        std::vector<AST::Function*> functions = n.get_functions();

        for(unsigned int i = 0; i < functions.size(); ++i)
            functions[i]->accept(*this);
    }

    void visit(AST::FunctionCall &n)
    {
        llvm::Function *called_fn = module->getFunction(n._function_name);

        if (!called_fn)
            throw std::runtime_error("Function '" + n._function_name + "' does not exist!");

        if (called_fn->arg_size() != n._args.size())
            throw std::runtime_error("Invalid Number of Arguments");

        std::vector<llvm::Value *> args_for_llvm;
        for (unsigned int i = 0; i < n._args.size(); i++)
        {
            n._args[i]->accept(*this);
            args_for_llvm.push_back(current_value);
            current_value = nullptr;
        }

        current_value = builder->CreateCall(called_fn, args_for_llvm, "calltmp");
    }

    void visit(AST::Function &n)
    {
        std::string fn_name = n._signature->get_name();
        llvm::Function *fn = module->getFunction(fn_name);

        if (!fn)
        {
            n._signature->accept(*this);
            fn = current_function;
            current_function = nullptr;
        }

        if (!fn->empty())
            throw std::runtime_error("Functions cannot be redefined!");


        llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", fn);
        builder->SetInsertPoint(block);


        named_values.clear();
        for (auto &arg : fn->args())
            named_values[arg.getName()] = &arg;

        current_value = nullptr;
        n._body->accept(*this);
        llvm::Value *ret_val = current_value;
        current_value = nullptr;

        if (!ret_val)
            throw std::runtime_error("Could not generate body for function");
        
        if(fn_name == "main")
            main_function = fn;

        builder->CreateRet(ret_val);
        current_function = fn;
    }

    void visit(AST::FunctionSignature &fs)
    {
        //TODO: types right now we only have doubles so everything is a double
        auto args = fs.get_args();
        std::vector<llvm::Type *> Doubles(args.size(), llvm::Type::getDoubleTy(*context));

        llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(*context), Doubles, false);
        llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, fs.get_name(), module.get());
        current_function = f;

        unsigned int index = 0;
        for (auto &arg : f->args())
        {
            auto name = args[index++];
            arg.setName(name);
        }
    }

    void visit(AST::Number &n)
    {
        current_value = llvm::ConstantFP::get(*context, llvm::APFloat(n.get_number()));
    }

    void visit(AST::Variable &n)
    {
        llvm::Value *var = named_values[n.name()];
        if (!var)
            throw std::runtime_error("Unknown Variable '" + n.name() + "'");

        current_value = var;
    }

    void visit(AST::BinaryExpr &n)
    {
        current_value = nullptr;
        n._left->accept(*this);
        llvm::Value *left = current_value;
        current_value = nullptr;
        n._right->accept(*this);
        llvm::Value *right = current_value;

        if (!left || !right)
            throw std::runtime_error("I think this should not happen");

        switch (n._op)
        {
        case '+':
            current_value = builder->CreateFAdd(left, right, "addtmp");
            break;
        case '-':
            current_value = builder->CreateFSub(left, right, "subtmp");
            break;
        case '*':
            current_value = builder->CreateFMul(left, right, "multmp");
            break;
        case '<':
            current_value = builder->CreateFCmpULT(left, right, "cmptmp");
            current_value = builder->CreateUIToFP(current_value, llvm::Type::getDoubleTy(*context), "booltmp");
            break;
        default:
            throw std::runtime_error("Operator '" + std::to_string(n._op) + "' not supported");
        }
    }

    void visit(AST::Node &n)
    {
        throw std::runtime_error("This should never happen. You forgot to add the AST:: ... Type to the Visitor");
    }
};
} // namespace HoneyBadger