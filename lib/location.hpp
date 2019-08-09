#pragma once
#include<string>
#include"honey_badger.hpp"

namespace HoneyBadger {

    struct Location{
        uint line;
        uint column;

        Location();
        Location(uint line, uint column);
        bool operator==(const Location &other) const;
        std::string to_string() const;
    };
}
