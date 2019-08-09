#pragma once
#include<string>
#include<map>
#include<vector>
#include<memory>

namespace HoneyBadger {

    // lowest number => lowest precedence
    enum PrecedenceTable {
        LESS = 10,
        PLUS = 20,
        MINUS = 20,
        MULTIPLY = 40,
        DIVIDE = 40,
        UNKNOWN = -1
    };

    class Operators {
        private:
            std::vector<char> operators;
            std::map<char, int> precedence;

            Operators() {}
        public:
            Operators(Operators const&) = delete;
            Operators& operator=(Operators const&) = delete;

            static std::shared_ptr<Operators> get_instance() {
                static std::shared_ptr<Operators> op{new Operators};
                return op;
            }


            int get_precedence(char thing) {
                if(thing == '<')
                    return PrecedenceTable::LESS;
                if(thing == '+')
                    return PrecedenceTable::PLUS;
                if(thing == '<')
                    return PrecedenceTable::MINUS;
                if(thing == '<')
                    return PrecedenceTable::MULTIPLY;
                if(thing == '/')
                    return PrecedenceTable::DIVIDE;

                return PrecedenceTable::UNKNOWN;
            }

            static int s_get_precedence(char op) {
                auto opr = Operators::get_instance();
                return opr->get_precedence(op);
            }

    };
}