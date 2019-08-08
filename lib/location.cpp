#include"location.hpp"

namespace HoneyBadger {

    Location::Location(){
        this->line = 0;
        this->column = 0;
    }
    Location::Location(uint line, uint column){
        this->line = line;
        this->column = column;
    }

    bool Location::operator==(const Location &other) const {
        return this->line == other.line && this->column == other.column;
    }
}