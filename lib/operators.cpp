#include"operators.hpp"
#include"precedence_table.hpp"

namespace HoneyBadger {
    std::shared_ptr<Operators> Operators::get_instance() {
        static std::shared_ptr<Operators> op{new Operators};
        return op;
    }

    int Operators::get_precedence(char op) {
        if(op == '=')
            return PrecedenceTable::ASSIGN;
        if(op == '<')
            return PrecedenceTable::LESS;
        if(op == '+')
            return PrecedenceTable::PLUS;
        if(op == '-')
            return PrecedenceTable::MINUS;
        if(op == '*')
            return PrecedenceTable::MULTIPLY;
        if(op == '/')
            return PrecedenceTable::DIVIDE;

        return PrecedenceTable::UNKNOWN;
    }
    int Operators::s_get_precedence(char op) {
        auto opr = Operators::get_instance();
        return opr->get_precedence(op);
    }
}