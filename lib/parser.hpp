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

            int current_token_precedence();
            Token current_token();
            void next_token();
            bool accept(std::string thing);
            bool expect(std::string thing);
            bool expect(int thing);

            std::unique_ptr<AST::FunctionSignature> parse_function_signature();
            std::unique_ptr<AST::Function> parse_function();
            std::unique_ptr<AST::Function> parse_top_level_expression();
            std::unique_ptr<AST::Node> parse_identifier_expression();
            std::unique_ptr<AST::Node> parse_parenthesis();
            std::unique_ptr<AST::Node> parse_number_expression();
            std::unique_ptr<AST::Node> parse_primary(); 
            std::unique_ptr<AST::Node> parse_expression(); 
            std::unique_ptr<AST::Node> parse_binary_op_right_side(int expression_precedence, std::unique_ptr<AST::Node> left); 
        public:
            Parser(std::vector<Token> tokens);
            std::unique_ptr<AST::Function> parse();
            //static void parse(std::vector<Token> tokens);
    };
}
