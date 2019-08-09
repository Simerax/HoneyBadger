#pragma once
#include<string>
#include<vector>
#include<memory>
#include"location.hpp"

namespace HoneyBadger::AST{
    class Node {
        Location begin;
        Location end;
        public:
            Node() = default;
            virtual ~Node() {}
    };

    class Variable : public Node {
        private: 
            std::string _name;

        public:
            Variable(std::string name);
            std::string name();
            void name(std::string name);
    };

    class Number : public Node {
        private:
            double _val;
        public:
            Number(double val) : _val(val) {}
    };

    class BinaryExpr : public Node {
        private: 
            char _op;
            std::unique_ptr<Node> _left;
            std::unique_ptr<Node> _right;
        public:
            BinaryExpr(char op, std::unique_ptr<Node> left, std::unique_ptr<Node> right) : _op(op), _left(std::move(left)), _right(std::move(right)) {}
    };

    class FunctionCall : public Node {
        private: 
            std::string _function_name;
            std::vector<std::unique_ptr<Node>> _args;
        public:
            FunctionCall(std::string name, std::vector<std::unique_ptr<Node>> args) : _function_name(name), _args(std::move(args)) {}
    };

    class FunctionSignature : public Node {
        private:
            std::string _name;
            std::vector<std::string> _args;
        public:
            FunctionSignature(std::string name, std::vector<std::string> args) : _name(name), _args(args) {}
            const std::string &get_name() const { return _name; }
    };

    class Function : public Node  {
        private:
            std::unique_ptr<FunctionSignature> _signature;
            std::unique_ptr<Node> _body;
        public:
            Function(std::unique_ptr<FunctionSignature> signature, std::unique_ptr<Node> body) : _signature(std::move(signature)), _body(std::move(body)) {}
    };
}
