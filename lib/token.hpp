#include"location.hpp"

namespace HoneyBadger {

    struct Token {
        std::string value;
        Location location;

        Token(std::string value, Location location){
            this->value = value;
            this->location = location;
        }
    };

}
