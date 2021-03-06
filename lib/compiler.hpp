#pragma once
#include <string>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "llvm/codegen.hpp"
#include "llvm/optimizer.hpp"
#include "string.hpp"
#include "ref.hpp"
#include"llvm/Support/TargetSelect.h" // InitializeAllTargets etc.
#include"llvm/Support/Host.h" // llvm::sys::getDefaultTargetTriple();
#include"llvm/Support/TargetRegistry.h" // llvm::TargetRegistry::
#include"llvm/Target/TargetOptions.h"
#include"llvm/Target/TargetMachine.h"
#include"llvm/Support/FileSystem.h" // sys::fs::

namespace HoneyBadger
{
class Compiler
{
    public:

    struct Config 
    {
        bool debug;
        Config() : debug(false) {}

        static CodeGenerator::Config convert_to_code_generator_config(Config c) {
            CodeGenerator::Config cg_config;
            cg_config.debug = c.debug;

            return cg_config;
        }
    };
private:
    string last_error;


    std::tuple<bool, string> read_file(string path) {
        std::ifstream input(path);
        string content, line;
        if(!input.is_open()) {
            last_error = "Could not open file '"+path+"' reason: "+strerror(errno);
            return std::tuple<bool,string>(false, "");
        }


        while(std::getline(input, line))
            content += line+"\n";
        input.close();

        return std::tuple<bool,string>(true, content);
    }

public:
    string get_last_error() { return this->last_error; }
    bool compile(string file, Config conf = Config{})
    {
        Ref<AST::FunctionTable> function_table = nullptr;
        CodeGenerator cg(Config::convert_to_code_generator_config(conf));
        std::unique_ptr<llvm::Module> module;

        std::tuple<bool, string> result = read_file(file);
        bool ok = std::get<0>(result);
        string filecontent = std::get<1>(result);
        if(!ok)
            return false;


        if (filecontent == "") {
            last_error = "Empty Source-file";
            return false;
        }

        try
        {
            Lexer lexer;
            Parser parser;
            function_table = parser.parse(lexer.lex(filecontent));
            cg.visit(*function_table);
            module = cg.get_result();
        }
        catch (std::runtime_error &e)
        {
            last_error = e.what();
            return false;
        }

        if (!cg.main_function_defined())
        {
            last_error = "No 'main' function defined";
            return false;
        }

        auto target_triple = llvm::sys::getDefaultTargetTriple();
        init_llvm_target_settings();

        string target_registry_error;
        auto target = llvm::TargetRegistry::lookupTarget(target_triple, target_registry_error);

        if(!target) {
            last_error = target_registry_error;
            return false;
        }

        const char* CPU = "generic";
        const char* features = "";
        llvm::TargetOptions opt;
        llvm::Reloc::Model rm;
        auto target_machine = target->createTargetMachine(target_triple, CPU, features, opt, rm);

        module->setDataLayout(target_machine->createDataLayout());
        module->setTargetTriple(target_triple);

        auto filename = file+".o";

        std::error_code err_code;
        llvm::raw_fd_ostream dest(filename, err_code, llvm::sys::fs::OpenFlags::F_None);

        if(err_code) {
            last_error = "Could not open destination file '"+filename+"' Error Code: "+err_code.message();
            return false;
        }

        Optimizer optimizer;

        auto filetype = target_machine->CGFT_ObjectFile;

        if(target_machine->addPassesToEmitFile(optimizer, dest, filetype)) {
            last_error = "Target Machine cannot emit Code.";
            return false;
        }

        optimizer.run(*module);
        dest.flush();
        return true;
    }

private:
    void init_llvm_target_settings()
    {
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmParsers();
        llvm::InitializeAllAsmPrinters();
    }
};
} // namespace HoneyBadger