#pragma once

namespace HoneyBadger {

    // lowest number => lowest precedence
    enum PrecedenceTable {
        ASSIGN = 2,
        LESS = 10,
        GREATER = 10,
        PLUS = 20,
        MINUS = 20,
        MULTIPLY = 40,
        DIVIDE = 40,
        UNKNOWN = -1
    };

}