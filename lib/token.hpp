#pragma once
#include"location.hpp"

namespace HoneyBadger {

    struct Token {
        enum Type {
            UNKNOWN,
            IDENTIFIER,
            KEYWORD,
            EQUALS,
            IF,
            BIN_OP,
            NUMBER,
            OPENING_PARENTHESIS,
            CLOSING_PARENTHESIS,
            FUNCTION_DEFINITION,
            VARIABLE_DEFINITION,
            SEPARATOR,
            END_OF_FILE,
        };
        std::string value;
        Location location;
        Type type;

        Token(std::string value, Location location, Type type = UNKNOWN){
            this->value = value;
            this->location = location;
            this->type = type;
        }
    };

}
