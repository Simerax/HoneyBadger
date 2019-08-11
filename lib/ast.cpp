#include"ast.hpp"

namespace HoneyBadger {
namespace AST {
    Variable::Variable(std::string name) {
        //TODO: Add Location
        this->_name = name;
    }

    std::string Variable::name(){
        return this->_name;
    }
    void Variable::name(std::string name){
        this->_name = name;
    }
}
}
