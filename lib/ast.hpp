#pragma once
#include<string>
#include<vector>
#include<memory>
#include"location.hpp"
#include"visitor.hpp"


namespace HoneyBadger::AST{
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
        private: 
            char _op;
            std::unique_ptr<Node> _left;
            std::unique_ptr<Node> _right;
        public:
            BinaryExpr(char op, std::unique_ptr<Node> left, std::unique_ptr<Node> right) : _op(op), _left(std::move(left)), _right(std::move(right)) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };

    class FunctionCall : public Node {
        private: 
            std::string _function_name;
            std::vector<std::unique_ptr<Node>> _args;
        public:
            FunctionCall(std::string name, std::vector<std::unique_ptr<Node>> args) : _function_name(name), _args(std::move(args)) {}
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
        private:
            std::unique_ptr<FunctionSignature> _signature;
            std::unique_ptr<Node> _body;
        public:
            Function(std::unique_ptr<FunctionSignature> signature, std::unique_ptr<Node> body) : _signature(std::move(signature)), _body(std::move(body)) {}
            void accept(Visitor &v) {
                v.visit(*this);
            };
    };
}
