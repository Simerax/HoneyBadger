#pragma once
#include<string>
#include<vector>
#include<memory>
#include"location.hpp"
#include"visitor.hpp"

namespace HoneyBadger {

namespace AST{
    class Node {
        private:
            Location begin;
            Location end;
        public:
            Node() = default;
            virtual ~Node() {}
            virtual void accept(Visitor &v) = 0;
    };

    class Variable : public Node {
        private: 
            std::string _name;

        public:
            Variable(std::string name);
            std::string name();
            void name(std::string name);
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    class Number : public Node {
        private:
            double _val;
        public:
            Number(double val) : _val(val) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
            double get_number() { return this->_val; }
    };

    class BinaryExpr : public Node {
        public:
            char _op;
            Node* _left;
            Node* _right;
            BinaryExpr(char op, Node* left, Node* right) : _op(op), _left(left), _right(right) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    class FunctionCall : public Node {
        public:
            std::string _function_name;
            std::vector<Node*> _args;
            FunctionCall(std::string name, std::vector<Node*> args) : _function_name(name), _args(args) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    class FunctionSignature : public Node {
        private:
            std::string _name;
            std::vector<std::string> _args;
        public:
            FunctionSignature(std::string name, std::vector<std::string> args) : _name(name), _args(args) {}
            const std::string &get_name() const { return _name; }
            std::vector<std::string> &get_args() { return _args; }
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    class Function : public Node  {
        public:
            FunctionSignature* _signature;
            Node* _body;
            Function(FunctionSignature* signature, Node* body) : _signature(signature), _body(body) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    class FunctionTable : public Node {
            std::vector<Function*> functions;
        public:
            void add_function(Function* f) { functions.push_back(f); }
            std::vector<Function*> get_functions() { return functions; }
            void accept(Visitor &v){
                v.visit(*this);
            }
    };
}

}