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

    class If : public Node {
        public:
            Node* _condition;
            Node* _then; 
            Node* _else; 

            ~If() {
                if(_condition != nullptr)
                    delete _condition;
                if(_then != nullptr)
                    delete _then;
                if(_else != nullptr)
                    delete _else;
            }

            If(Node* condition, Node* then, Node* els) : _condition(condition), _then(then), _else(els) {}

            void accept(Visitor &v) {
                v.visit(*this);
            }
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
            ~BinaryExpr() {
                if(_left != nullptr)
                    delete _left;
                if(_right != nullptr)
                    delete _right;
            }
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
            ~FunctionCall() {
                for(auto &e : _args)
                    if(e != nullptr)
                        delete e;
            }
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
            ~Function() {
                if(_signature != nullptr)
                    delete _signature;
                if(_body != nullptr)
                    delete _body;
            }
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
            ~FunctionTable() {
                for(auto &e : functions)
                    if(e != nullptr)
                        delete e;
            }
            void add_function(Function* f) { functions.push_back(f); }
            std::vector<Function*> get_functions() { return functions; }
            void accept(Visitor &v){
                v.visit(*this);
            }
    };
}

}