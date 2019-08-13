#pragma once

namespace HoneyBadger {
    // we need to declare the classes here to avoid circular dependencies
    namespace AST {
        class Node;
        class Variable;
        class Number;
        class BinaryExpr;
        class FunctionCall;
        class FunctionSignature;
        class Function;
        class FunctionTable;
        class If;
        class Block;
        class VariableDefinition;
    }

    class Visitor{
        public:
        virtual void visit(AST::Node &n) = 0;
        virtual void visit(AST::Variable &n) = 0;
        virtual void visit(AST::Number &n) = 0;
        virtual void visit(AST::BinaryExpr &n) = 0;
        virtual void visit(AST::FunctionCall &n) = 0;
        virtual void visit(AST::FunctionSignature &n) = 0;
        virtual void visit(AST::Function &n) = 0;
        virtual void visit(AST::FunctionTable &n) = 0;
        virtual void visit(AST::If &n) = 0;
        virtual void visit(AST::Block &n) = 0;
        virtual void visit(AST::VariableDefinition &n) = 0;
    };
}