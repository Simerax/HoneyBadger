#pragma once
#include<string>
#include<vector>
#include"location.hpp"
#include"visitor.hpp"
#include"ref.hpp"
#include "string.hpp"
#include "type.hpp"

namespace HoneyBadger {

namespace AST{

    class Node {
        public:
            Location location;
            Node() = default;
            virtual ~Node() {}
            virtual void accept(Visitor &v) = 0;
    };

    /// A list of Expressions
    ///
    /// A Block contains a list of expressions. For example a Function has a body that body is a block
    class Block : public Node {
        private:
            std::vector<Ref<Node>> _expressions;
        public:
            Block(){}
            Block(std::vector<Ref<Node>> expressions) : _expressions(expressions) {}
            void accept(Visitor &v){ v.visit(*this); }
            std::vector<Ref<Node>> get_expressions() { return _expressions; }
            void add_expression(Ref<Node> expression) { _expressions.push_back(expression); }
    };

    /// Keeps the subexpressions which make up an if. those are a condition, a then-block and an else-block
    ///
    class If : public Node {
        public:
            Ref<Node> _condition;
            Ref<Block> _then; 
            Ref<Block> _else; 

            If(Ref<Node> condition, Ref<Block> then, Ref<Block> els) : _condition(condition), _then(then), _else(els) {}

            void accept(Visitor &v) {
                v.visit(*this);
            }
    };

    /// Keeps a Variable Name
    class Variable : public Node {
        private: 
            string _name;

        public:
            Variable(string name) : _name(name) {}
            string name() { return this->_name; }
            void name(string name) { this->_name = name; }
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    class VariableDefinition : public Node
    {
        public:
            Ref<Variable> variable;
            Ref<Node> value;
            VariableDefinition(Ref<Variable> var, Ref<Node> val) : variable(var), value(val) {}
            void accept(Visitor &v) { v.visit(*this); }
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
    class BinaryExpr : public Node {
        public:
            char _op;
            Ref<Node> _left;
            Ref<Node> _right;
            BinaryExpr(char op, Ref<Node> left, Ref<Node> right) : _op(op), _left(left), _right(right) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    /// Keeps the expressions which make up the parameters of a given Function
    ///
    /// Note: This is different then FunctionSignature!
    class FunctionCall : public Node {
        public:
            string _function_name;
            std::vector<Ref<Node>> _args;
            FunctionCall(string name, std::vector<Ref<Node>> args) : _function_name(name), _args(args) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    class Argument : public Node 
    {
        public:
            string name;
            Type type;
            Argument(string name, Type type, Location loc) : name(name), type(type){
                this->location = loc;
            }
            void accept(Visitor &v) { v.visit(*this); }
        static Argument make_argument(string name, Token from)
        {
            return Argument(name, Type::convert_token_type_to_type(from.type), from.location);
        }
    };

    /// Keeps the name of a function and its arguments(signature)
    ///
    /// Does not contain the functions return value. At the moment Functions return the result of their last expression
    /// Note: This is different then the FunctionCall. The Signature consists of the arguments as parameters in variable form. 
    /// A FunctionCall contains the actual expressions that define the values of the parameters
    class FunctionSignature : public Node {
        private:
            string _name;
            std::vector<Argument> _args;
            Type return_type;
        public:
            FunctionSignature(string name, std::vector<Argument> args, Type return_type) : _name(name), _args(args), return_type(return_type) {}
            const string &get_name() const { return _name; }
            Type get_return_type() { return return_type; }
            std::vector<Argument> &get_args() { return _args; }
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };


    /// Function
    ///
    class Function : public Node  {
        public:
            Ref<FunctionSignature> _signature;
            Ref<Block> _body;
            Function(Ref<FunctionSignature> signature, Ref<Block> body) : _signature(signature), _body(body) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    /// List of Functions
    ///
    /// Usually you create a function table per Module. This Table keeps a reference to all of the modules functions.
    class FunctionTable : public Node {
            std::vector<Ref<Function>> functions;
        public:
            void add_function(Ref<Function> f) { functions.push_back(f); }
            std::vector<Ref<Function>> get_functions() { return functions; }
            void accept(Visitor &v){
                v.visit(*this);
            }
    };
}

}