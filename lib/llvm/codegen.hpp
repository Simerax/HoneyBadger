#pragma once
#include <memory>
#include "../visitor.hpp"
#include "../ast.hpp"
#include "../string.hpp"

#include "optimizer.hpp"

#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Verifier.h" // verifyFunction

namespace HoneyBadger
{
class CodeGenerator : public Visitor
{
public:
    struct Config
    {
        bool debug;
        string module_name;
        Config() : debug(false), module_name("") {}
    };

private:
    //TODO: Change to ref
    llvm::LLVMContext *context;
    llvm::IRBuilder<> *builder;
    std::unique_ptr<llvm::Module> module;
    std::map<string, llvm::AllocaInst *> named_values;

    llvm::Value *current_value = nullptr;
    llvm::Function *current_function = nullptr;

    llvm::Function *main_function = nullptr;

    Config config;

public:
    CodeGenerator(Config conf = Config{})
    {
        this->config = conf;
        context = new llvm::LLVMContext();
        builder = new llvm::IRBuilder<>(*context);
        module = llvm::make_unique<llvm::Module>("MyModule", *context);
        define_built_in_functions();
    }

    void define_built_in_functions()
    {
        std::vector<llvm::Type *> arg(1, llvm::Type::getDoubleTy(*context));

        llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(*context), arg, false);
        llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "print", module.get());
    }

    bool main_function_defined()
    {
        return main_function != nullptr;
    }

    llvm::AllocaInst *create_entry_block_alloca(string name, string type = "double", llvm::Function *function = nullptr)
    {
        if (!function)
            function = current_function;

        llvm::IRBuilder<> tmp_builder(&function->getEntryBlock(), function->getEntryBlock().begin());
        return tmp_builder.CreateAlloca(llvm::Type::getDoubleTy(*context), 0, name.c_str());
    }

    std::unique_ptr<llvm::Module> get_result()
    {
        if (llvm::verifyModule(*module, &llvm::errs()))
            throw std::runtime_error("Error in Module: " + module->getName().str());

        if (config.debug)
        {
            llvm::errs() << "Dumping Module: " << module->getName() << "\n";
            module->print(llvm::errs(), nullptr);
            llvm::errs() << "\nDone with dump of Module: " << module->getName() << "\n";
        }

        return std::move(module);
    }

    void visit(AST::If &n)
    {

        // Generate condition
        if (n._condition == nullptr)
            throw std::runtime_error("No condition?");
        current_value = nullptr;
        n._condition->accept(*this);
        auto condition = current_value;

        condition = builder->CreateFCmpONE(condition, llvm::ConstantFP::get(*context, llvm::APFloat(0.0)), "ifcond_double");

        llvm::BasicBlock *then_block = llvm::BasicBlock::Create(*context, "then", current_function);
        llvm::BasicBlock *else_block = llvm::BasicBlock::Create(*context, "else");
        llvm::BasicBlock *merge = llvm::BasicBlock::Create(*context, "merge");

        builder->CreateCondBr(condition, then_block, else_block);
        builder->SetInsertPoint(then_block);

        // Generate then block
        if (n._then == nullptr)
            throw std::runtime_error("No then Block?");
        current_value = nullptr;
        n._then->accept(*this);
        auto then = current_value;

        builder->CreateBr(merge);
        then_block = builder->GetInsertBlock();

        current_function->getBasicBlockList().push_back(else_block);
        builder->SetInsertPoint(else_block);

        // Generate else block
        if (n._else == nullptr)
            throw std::runtime_error("No else Block?");
        current_value = nullptr;
        n._else->accept(*this);
        auto _else = current_value;

        builder->CreateBr(merge);
        else_block = builder->GetInsertBlock();

        current_function->getBasicBlockList().push_back(merge);
        builder->SetInsertPoint(merge);

        llvm::PHINode *pn = builder->CreatePHI(llvm::Type::getDoubleTy(*context), 2, "iftmp");
        pn->addIncoming(then, then_block);
        pn->addIncoming(_else, else_block);

        current_value = pn;
    }

    void visit(AST::FunctionTable &n)
    {
        std::vector<Ref<AST::Function>> functions = n.get_functions();

        for (auto &arg : functions)
            arg->accept(*this);
    }

    void visit(AST::FunctionCall &n)
    {
        llvm::Function *called_fn = module->getFunction(n._function_name);
        if (!called_fn)
        {
            if (n._function_name != "print")
                throw std::runtime_error("Function '" + n._function_name + "' does not exist!");
        }

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
        string fn_name = n._signature->get_name();
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

        current_function = fn; // the signature might already need some information about the function so we gotta set it before

        named_values.clear();
        for (auto &arg : fn->args())
        {
            llvm::AllocaInst *alloca = create_entry_block_alloca(arg.getName());
            builder->CreateStore(&arg, alloca);
            named_values[arg.getName()] = alloca;
        }

        current_value = nullptr;
        n._body->accept(*this);
        llvm::Value *ret_val = current_value;
        current_value = nullptr;

        if (!ret_val)
            throw std::runtime_error("Could not generate body for function");

        if (fn_name == "main")
            main_function = fn;

        builder->CreateRet(ret_val);

        if (llvm::verifyFunction(*fn, &llvm::errs()))
            throw std::runtime_error("Error in Function '" + fn_name + "' (Module: '" + module->getName().str() + "')");

        llvm::verifyFunction(*fn);
    }

    void visit(AST::Block &n)
    {
        for (auto &expr : n.get_expressions())
            expr->accept(*this);
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

        current_value = builder->CreateLoad(var, n.name());
    }

    void visit(AST::VariableDefinition &n)
    {
        string variable_name = n.variable.get()->name();
        auto search = named_values.find(variable_name);

        if (search != named_values.end())
            throw std::runtime_error("Cannot redefine Variable '" + variable_name + "'");

        auto *variable = create_entry_block_alloca(variable_name);
        named_values[variable_name] = variable;

        current_value = nullptr;
        n.value->accept(*this);
        auto value = current_value;

        builder->CreateStore(value, variable);
        current_value = value;
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
        case '>':
            current_value = builder->CreateFCmpULT(right, left, "cmptmp");
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