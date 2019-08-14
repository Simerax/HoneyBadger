#pragma once
#include"location.hpp"

namespace HoneyBadger {

    struct Token {
        enum Type {
            UNKNOWN,
            IDENTIFIER,
            KEYWORD,
            ASSIGN,
            IF,
            BIN_OP,
            NUMBER,
            OPENING_PARENTHESIS,
            CLOSING_PARENTHESIS,
            FUNCTION_DEFINITION,
            VARIABLE_DEFINITION,
            SEPARATOR,
            STRING_LITERAL,
            END_OF_FILE,
        };
        string value;
        Location location;
        Type type;

        Token(string value, Location location, Type type = UNKNOWN){
            this->value = value;
            this->location = location;
            this->type = type;
        }
    };

}
