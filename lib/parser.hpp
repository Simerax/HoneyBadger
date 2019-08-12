#pragma once
#include<vector>
#include<string>
#include"token.hpp"
#include"ast.hpp"

namespace HoneyBadger{
    class Parser {
        private:
            unsigned int current_index = 0;
            std::vector<Token> tokens;
            bool inside_if_then_block = false;

            int current_token_precedence();
            Token current_token();
            void next_token();
            bool accept(std::string thing);
            bool expect(std::string thing);
            bool expect(int thing);

            AST::FunctionSignature* parse_function_signature();
            AST::Function* parse_function();
            AST::Function* parse_top_level_expression();
            AST::Block* parse_block();
            AST::Node* parse_identifier_expression();
            AST::Node* parse_parenthesis();
            AST::Node* parse_number_expression();
            AST::Node* parse_primary(); 
            AST::Node* parse_expression(); 
            AST::Node* parse_if_expression();
            AST::Node* parse_binary_op_right_side(int expression_precedence, AST::Node* left); 
        public:
            AST::FunctionTable* parse(std::vector<Token> tokens);
    };
}
