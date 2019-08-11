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

    // Why do we need to keep track of this function in a special kind of way?
    // the "top level" function basically wraps all "free floating" expressions and the other function definitions
    // this means that there can be a function defined within our (top level) function (which is forbidden otherwise)
    // if we would not seperate this function from the rest then the llvm::IRBuilder would insert top level code in its last known function
    //
    //  lets say we create a function A().
    //  we would first reach the __TOP_LEVEL__ Function (which is provided by the parser!) to handle free floating expressions
    //  then we find the Function A(). 
    //  We start generating its signature. At this point our "current_function" is A()
    //  After that we generate the functions body (IRBuilder is inserting code into that function)
    //  Now, after the body is complete, we generate its return value (last expression from the body)
    //  Next we "leave" the function. We are back in our __TOP_LEVEL__ Function.
    //  Now the IRBuilder is still in the context of A() and will insert any future code into that function
    //  This of course is wrong since we are back in our __TOP_LEVEL__ Function.
    //  Thats why we now explicitly set our insert_point back to top_level_function_insert_point
    //  
    llvm::Function *top_level_function = nullptr;
    llvm::BasicBlock *top_level_function_insert_point = nullptr;

public:
    CodeGenerator()
    {
        context = new llvm::LLVMContext();
        builder = new llvm::IRBuilder<>(*context);
        module = llvm::make_unique<llvm::Module>("my jiiiiiiiiit reeeeeeeeeee", *context);
    }

    llvm::Function *get_result()
    {
        module->print(llvm::errs(), nullptr);
        return top_level_function;
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
        bool is_top_level_function = false;
        std::string fn_name = n._signature->get_name();

        if (fn_name == "__TOP_LEVEL__")
            is_top_level_function = true;

        llvm::Function *fn = module->getFunction(fn_name);

        if (!fn)
        {
            n._signature->accept(*this);
            fn = current_function;
            current_function = nullptr;
        }

        if (!fn->empty())
            throw std::runtime_error("Functions cannot be redefined!");

        if (is_top_level_function)
            top_level_function = fn;

        llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", fn);
        builder->SetInsertPoint(block);

        // normally we don't allow a function inside a function but our top level (outside any function)
        // is basically wrapped in a anonymous function. 
        // To not lose track about our insert point for "free floating" expressions we need to make sure we keep this insert point around
        if(is_top_level_function)
            top_level_function_insert_point = block;

        named_values.clear();
        for (auto &arg : fn->args())
            named_values[arg.getName()] = &arg;

        current_value = nullptr;
        n._body->accept(*this);
        llvm::Value *ret_val = current_value;
        current_value = nullptr;

        if (is_top_level_function) {
            builder->SetInsertPoint(top_level_function_insert_point);
            builder->CreateRet(nullptr); // top level function always returns void - at least for now
        }
        else
        {
            if (!ret_val)
                throw std::runtime_error("Could not generate body for function");
            builder->CreateRet(ret_val);
        }
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
        throw std::runtime_error("This should never happend");
    }
};
} // namespace HoneyBadger