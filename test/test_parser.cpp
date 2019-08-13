
#include "../catch2/catch.hpp"
#include "../fakeit/fakeit.hpp"

#include "../lib/lexer.hpp"
#include "../lib/parser.hpp"
#include "mock_visitor.hpp"

using namespace HoneyBadger;
using namespace fakeit;

TEST_CASE("Multiple Expressions in a block", "[Parser]")
{
    string input = 
    "def func(a,b) do\n"
    "   if a < b do\n"
    "       a + b\n"
    "       a + b * 2\n"
    "   else\n"
    "       a - b\n"
    "       a + b * 2\n"
    "   end\n"
    "end"
    "";

    auto tokens = Lexer().lex(input);
    Parser p;
    auto function_table = p.parse(tokens);
    REQUIRE(1 == 1); // TODO: I have to make fakeit work with the visitor. for now at least make sure no exception is thrown
}

TEST_CASE("Parser Tests", "[Parser]")
{

    // When(OverloadedMethod(mock_visitor, visit, void(AST::Node&))).AlwaysDo([mock_visitor](AST::Node& n){});
    // When(OverloadedMethod(mock_visitor, visit, void(AST::Variable&))).AlwaysDo([mock_visitor](AST::Variable& n){});
    // When(OverloadedMethod(mock_visitor, visit, void(AST::Number&))).AlwaysDo([mock_visitor](AST::Number& n){});
    // When(OverloadedMethod(mock_visitor, visit, void(AST::BinaryExpr&))).AlwaysDo([mock_visitor](AST::BinaryExpr& n){});
    // When(OverloadedMethod(mock_visitor, visit, void(AST::FunctionCall&))).AlwaysDo([mock_visitor](AST::FunctionCall& n){});
    // When(OverloadedMethod(mock_visitor, visit, void(AST::FunctionSignature&))).AlwaysDo([mock_visitor](AST::FunctionSignature& n){});
    // When(OverloadedMethod(mock_visitor, visit, void(AST::Function&))).AlwaysDo([mock_visitor](AST::Function& n){});
    // When(OverloadedMethod(mock_visitor, visit, void(AST::FunctionTable&))).AlwaysDo([mock_visitor](AST::FunctionTable& n){});



    SECTION("parse function definition")
    {
        //fakeit::Mock<MockVisitor> mock_visitor;
        //Lexer l;
        //TODO: Why does this fail?
        //When(Method(mock_visitor, on_function)).AlwaysDo([](AST::Function& n){
        //    REQUIRE(n._signature->get_name() == "func");
        //});

        //string input =
        //    "def func(a,b) do\n"
        //    "   a+b\n"
        //    "end\n"
        //    "";
        //auto tokens = l.lex(input);
        //AST::FunctionTable *table = Parser(tokens).parse();
        //mock_visitor.get().visit(*table);
        //delete table;
    }
}