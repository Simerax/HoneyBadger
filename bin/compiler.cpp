#include<iostream>
#include"../lib/compiler.hpp"

using namespace HoneyBadger;
int main(int argc, char** argv) {

    Compiler compiler;

    if(argc != 2) {
        std::cout << "Wrong number of arguments. try: ./compiler file.hb";
        return 1;
    }

    std::string file = argv[1];

    bool ok = compiler.compile(file);
    if(!ok)
        std::cout << compiler.get_last_error() << "\n";
    else
        std::cout << "done.\n";

    return 0;
}