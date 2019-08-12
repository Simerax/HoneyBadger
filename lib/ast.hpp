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

    /// A list of Expressions
    ///
    /// A Block contains a list of expressions. For example a Function has a body that body is a block
    /// Will delete all of its Expressions when deleted
    class Block : public Node {
        private:
            std::vector<Node*> _expressions;
        public:
            Block(){}
            Block(std::vector<Node*> expressions) : _expressions(expressions) {}
            ~Block() {
                for(auto &ex : _expressions)
                    if(ex != nullptr)
                        delete ex;
            }
            void accept(Visitor &v){ v.visit(*this); }
            std::vector<Node*> get_expressions() { return _expressions; }
            void add_expression(Node* expression) { _expressions.push_back(expression); }
    };

    /// Keeps the subexpressions which make up an if. those are a condition, a then-block and an else-block
    ///
    /// Deletes its condition, then and else nodes when deleted
    class If : public Node {
        public:
            Node* _condition;
            Block* _then; 
            Block* _else; 

            ~If() {
                if(_condition != nullptr)
                    delete _condition;
                if(_then != nullptr)
                    delete _then;
                if(_else != nullptr)
                    delete _else;
            }

            If(Node* condition, Block* then, Block* els) : _condition(condition), _then(then), _else(els) {}

            void accept(Visitor &v) {
                v.visit(*this);
            }
    };

    /// Keeps a Variable Name
    class Variable : public Node {
        private: 
            std::string _name;

        public:
            Variable(std::string name) : _name(name) {}
            std::string name() { return this->_name; }
            void name(std::string name) { this->_name = name; }
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    /// Keeps the Value of a Number
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

    /// Keeps a binary expression such as a + b or 2 * b.
    ///
    /// This kind of node is often used recursivly. the _left or _right can also be BinaryExpressions to make complex expressions such as 1 + 2 * x
    /// Deletes its left and right node when deleted
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

    /// Keeps the expressions which make up the parameters of a given Function
    ///
    /// Note: This is different then FunctionSignature!
    /// Deletes its arguments when deleted
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

    /// Keeps the name of a function and its arguments(signature)
    ///
    /// Does not contain the functions return value. At the moment Functions return the result of their last expression
    /// Note: This is different then the FunctionCall. The Signature consists of the arguments as parameters in variable form. 
    /// A FunctionCall contains the actual expressions that define the values of the parameters
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


    /// Function
    ///
    /// Will delete its signature and body when deleted 
    class Function : public Node  {
        public:
            ~Function() {
                if(_signature != nullptr)
                    delete _signature;
                if(_body != nullptr)
                    delete _body;
            }
            FunctionSignature* _signature;
            Block* _body;
            Function(FunctionSignature* signature, Block* body) : _signature(signature), _body(body) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    /// List of Functions
    ///
    /// Usually you create a function table per Module. This Table keeps a reference to all of the modules functions.
    /// Will delete all of its functions when deleted
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