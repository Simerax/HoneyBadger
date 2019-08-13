#pragma once
#include<vector>
#include "string.hpp"

#include"token.hpp"

namespace HoneyBadger {
    class Lexer {
        private:
            string input;
            unsigned int index;

            bool is_delimiter(char c);
            bool is_newline(char c);
            Token::Type get_type(string thing);
            bool looks_like_number(string thing);
            bool is_operator(char op);
            bool peek(char expected);
        public: 
            std::vector<Token> lex(string);
    };
}
