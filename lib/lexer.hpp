#pragma once
#include<vector>
#include<string>

#include"token.hpp"

namespace HoneyBadger {
    class Lexer {
        private:
            bool is_delimiter(char c);
            bool is_newline(char c);
            Token::Type get_type(std::string thing);
            bool looks_like_number(std::string thing);
            bool is_operator(char op);
        public: 
            std::vector<Token> lex(std::string);
    };
}
