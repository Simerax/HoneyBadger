#pragma once
#include<vector>
#include<string>

#include"token.hpp"

namespace HoneyBadger {
    class Lexer {
        public: 
            std::vector<Token> lex(std::string);
            bool is_delimiter(char c);
            bool is_newline(char c);
    };
}
