#include "lexer.hpp"

namespace HoneyBadger {
    std::vector<Token> Lexer::lex(std::string input) {
        std::vector<Token> tokens;

        // in case the input has no EOL e.g newline we just add one for safety
        // the worst that could happen is that we parse two EOL...Oh well
        input += "\n"; 

        std::string word = "";
        uint line = 1;
        uint column = 1;
        uint word_line = line;
        uint word_column = column;

        for(uint index = 0; index < input.length(); ++index) {
            char cc = input.at(index);
            column++;

            if (is_delimiter(cc) || is_newline(cc) || index + 1 == input.length()){
                tokens.push_back(Token(word, Location(word_line, word_column)));
                word = "";
                word_line = line;
                word_column = column;
            }
            else 
                word += cc;

            if(is_newline(cc)){
                line++;
                column = 1;
                word_line = line;
                word_column = column;
            }
        }

        return tokens;
    }

    bool Lexer::is_delimiter(char c) {
        return c == ' ';
    }

    bool Lexer::is_newline(char c) {
        return c == '\n';
    }
}
