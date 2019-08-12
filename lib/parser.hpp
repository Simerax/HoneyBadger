#pragma once
#include<vector>
#include<string>
#include"token.hpp"
#include"ast.hpp"
#include"ref.hpp"

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

            Ref<AST::FunctionSignature> parse_function_signature();
            Ref<AST::Function> parse_function();
            Ref<AST::Function> parse_top_level_expression();
            Ref<AST::Block> parse_block();
            Ref<AST::Node> parse_identifier_expression();
            Ref<AST::Node> parse_parenthesis();
            Ref<AST::Node> parse_number_expression();
            Ref<AST::Node> parse_primary(); 
            Ref<AST::Node> parse_expression(); 
            Ref<AST::Node> parse_if_expression();
            Ref<AST::Node> parse_binary_op_right_side(int expression_precedence, Ref<AST::Node> left); 
        public:
            Ref<AST::FunctionTable> parse(std::vector<Token> tokens);
    };
}
