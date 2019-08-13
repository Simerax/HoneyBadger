#pragma once
#include<string>
#include"honey_badger.hpp"
#include "string.hpp"

namespace HoneyBadger {

    struct Location{
        uint line;
        uint column;

        Location();
        Location(uint line, uint column);
        bool operator==(const Location &other) const;
        string to_string() const;
    };
}
