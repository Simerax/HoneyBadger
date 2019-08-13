#include <iostream>
#include "../lib/compiler.hpp"

using namespace HoneyBadger;

Compiler::Config parse_args(std::vector<std::string> args)
{
    Compiler::Config conf;

    for (auto &arg : args)
    {
        if (arg == "-d")
            conf.debug = true;
    }
    return conf;
}

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        std::cout << "You need to supply at least one file to compile";
        return 0;
    }

    std::vector<std::string> args;
    for (unsigned int i = 1; i < argc; ++i)
        args.push_back(argv[i]);
    auto conf = parse_args(args);

    Compiler compiler;

    auto file = args.at(args.size() - 1);

    bool ok = compiler.compile(file, conf);
    if (!ok)
        std::cout << compiler.get_last_error() << "\n";
    else
        std::cout << "done.\n";

    return 0;
}
