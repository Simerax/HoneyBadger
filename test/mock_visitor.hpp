
#pragma once
#include"../lib/visitor.hpp"
#include"../lib/ast.hpp"
#include"../lib/ref.hpp"


namespace HoneyBadger
{
class MockVisitor : public Visitor
{
public:

    void on_function_table(AST::FunctionTable &n){}
    void visit(AST::FunctionTable &n) {
        on_function_table(n);
        std::vector<Ref<AST::Function>> functions = n.get_functions();
        for(unsigned int i = 0; i < functions.size(); ++i)
            functions[i]->accept(*this);
    }

    void on_function_call(AST::FunctionCall &n) {}
    void visit(AST::FunctionCall &n)
    {
        on_function_call(n);
        for(auto &e : n._args)
            e->accept(*this);
    }

    virtual void on_function(AST::Function &n) = 0;
    void visit(AST::Function &n)
    {
        on_function(n);
        n._signature->accept(*this);
        n._body->accept(*this);
    }

    void on_function_signature(AST::FunctionSignature &n){}
    void visit(AST::FunctionSignature &fs)
    {
        on_function_signature(fs);
    }

    void on_number(AST::Number &n){}
    void visit(AST::Number &n)
    {
        on_number(n);
    }

    void on_variable(AST::Variable &n) {}
    void visit(AST::Variable &n)
    {
        on_variable(n);
    }

    void on_binary_expr(AST::BinaryExpr &n){}
    void visit(AST::BinaryExpr &n)
    {
        on_binary_expr(n);
        n._left->accept(*this);
        n._right->accept(*this);
    }

    void on_node(AST::Node &n) {
        throw std::runtime_error("i think this should not happen");
    }
    void visit(AST::Node &n)
    {
        on_node(n);
    }
};
} // namespace HoneyBadger