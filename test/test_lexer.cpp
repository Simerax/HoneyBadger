#include"../catch2/catch.hpp"
#include"../lib/lexer.hpp"
#include"../lib/parser.hpp"

using namespace HoneyBadger;

TEST_CASE("lex variable definition","[Lexer]") {
    std::string input = "let x = 5";

    Lexer l;
    std::vector<Token> tokens = l.lex(input);
    REQUIRE(tokens.at(0).value == "let");
    REQUIRE(tokens.at(0).type == Token::Type::VARIABLE_DEFINITION);
    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);
    REQUIRE(tokens.at(2).value == "=");
    REQUIRE(tokens.at(2).type == Token::Type::EQUALS);
    REQUIRE(tokens.at(3).value == "5");
    REQUIRE(tokens.at(3).type == Token::Type::NUMBER);

}

TEST_CASE("lex if statement","[Lexer]") {
    std::string input = "if x > 3 do\nprint x\nend";

    Lexer l;
    std::vector<Token> tokens = l.lex(input);
    REQUIRE(tokens.at(0).value == "if");
    REQUIRE(tokens.at(0).location == Location(1,1));
    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).location == Location(1,4));
    REQUIRE(tokens.at(2).value == ">");
    REQUIRE(tokens.at(3).value == "3");
    REQUIRE(tokens.at(4).value == "do");
    REQUIRE(tokens.at(5).value == "print");
    REQUIRE(tokens.at(6).value == "x");
    REQUIRE(tokens.at(7).value == "end");
    REQUIRE(tokens.at(7).location == Location(3,1));

    REQUIRE(tokens.at(8).value == "");
    REQUIRE(tokens.at(8).location == Location(4,1));
    REQUIRE(tokens.at(8).type == Token::Type::END_OF_FILE);
}

TEST_CASE("lex function definition", "[Lexer]"){
    std::string input = "def func(a, b) do\na + b\nend";
    Lexer l;
    auto tokens = l.lex(input);

    REQUIRE(tokens.at(0).value == "def");
    REQUIRE(tokens.at(0).location == Location(1,1));
    REQUIRE(tokens.at(0).type == Token::Type::FUNCTION_DEFINITION);

    REQUIRE(tokens.at(1).value == "func");
    REQUIRE(tokens.at(1).location == Location(1,5));
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(2).value == "(");
    REQUIRE(tokens.at(2).location == Location(1,9));
    REQUIRE(tokens.at(2).type == Token::Type::OPENING_PARENTHESIS);

    REQUIRE(tokens.at(3).value == "a");
    REQUIRE(tokens.at(3).location == Location(1,10));
    REQUIRE(tokens.at(3).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(4).value == ",");
    REQUIRE(tokens.at(4).location == Location(1,11));
    REQUIRE(tokens.at(4).type == Token::Type::SEPARATOR);
    
    REQUIRE(tokens.at(5).value == "b");
    REQUIRE(tokens.at(5).location == Location(1,12));
    REQUIRE(tokens.at(5).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(6).value == ")");
    REQUIRE(tokens.at(6).location == Location(1,14));
    REQUIRE(tokens.at(6).type == Token::Type::CLOSING_PARENTHESIS);

    REQUIRE(tokens.at(7).value == "do");
    REQUIRE(tokens.at(7).location == Location(1,15)); //TODO: shouldn't this be 1,16 ?
    REQUIRE(tokens.at(7).type == Token::Type::KEYWORD);

    REQUIRE(tokens.at(8).value == "a");
    REQUIRE(tokens.at(8).location == Location(2,1));
    REQUIRE(tokens.at(8).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(9).value == "+");
    REQUIRE(tokens.at(9).location == Location(2,3));
    REQUIRE(tokens.at(9).type == Token::Type::BIN_OP);

    REQUIRE(tokens.at(10).value == "b");
    REQUIRE(tokens.at(10).location == Location(2,5));
    REQUIRE(tokens.at(10).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(11).value == "end");
    REQUIRE(tokens.at(11).location == Location(3,1));
    REQUIRE(tokens.at(11).type == Token::Type::KEYWORD);
}

TEST_CASE("parse this shit", "[Parser]") {
    std::string input = "def func(x, y) do\nx + y\nend";

    Lexer l;
    auto tokens = l.lex(input);
    Parser p(tokens);
    p.parse();
}