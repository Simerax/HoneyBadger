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
    std::string Location::to_string() const {
        return std::string(std::to_string(this->line) +":"+std::to_string(this->column));
    }

    std::ostream& operator<<(std::ostream &os, const Location &loc) {
        return os << loc.to_string();
    }
}