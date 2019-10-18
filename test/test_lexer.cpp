#include "../catch2/catch.hpp"
#include "../lib/lexer.hpp"

using namespace HoneyBadger;

TEST_CASE("lex variable definition", "[Lexer]")
{
    string input = "let x = 5 //test comment";

    Lexer l;
    std::vector<Token> tokens = l.lex(input);
    REQUIRE(tokens.at(0).value == "let");
    REQUIRE(tokens.at(0).type == Token::Type::VARIABLE_DEFINITION);
    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);
    REQUIRE(tokens.at(2).value == "=");
    REQUIRE(tokens.at(2).type == Token::Type::ASSIGN);
    REQUIRE(tokens.at(3).value == "5");
    REQUIRE(tokens.at(3).type == Token::Type::NUMBER);
}

TEST_CASE("lex list definition", "[Lexer]")
{
    string input = "let x = [1,2] //test comment";

    Lexer l;
    std::vector<Token> tokens = l.lex(input);
    REQUIRE(tokens.at(0).value == "let");
    REQUIRE(tokens.at(0).type == Token::Type::VARIABLE_DEFINITION);
    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);
    REQUIRE(tokens.at(2).value == "=");
    REQUIRE(tokens.at(2).type == Token::Type::ASSIGN);
    REQUIRE(tokens.at(3).value == "[");
    REQUIRE(tokens.at(3).type == Token::Type::OPENING_SQUARE_BRACKET);
    REQUIRE(tokens.at(4).value == "1");
    REQUIRE(tokens.at(4).type == Token::Type::NUMBER);
    REQUIRE(tokens.at(5).value == ",");
    REQUIRE(tokens.at(5).type == Token::Type::SEPARATOR);
    REQUIRE(tokens.at(6).value == "2");
    REQUIRE(tokens.at(6).type == Token::Type::NUMBER);
    REQUIRE(tokens.at(7).value == "]");
    REQUIRE(tokens.at(7).type == Token::Type::CLOSING_SQUARE_BRACKET);

}

TEST_CASE("lex simple comment", "[Lexer]")
{
    string input = 
    "//comment in first line\n"
    "//comment in second line with a // inside\n"
    "def func(a,b) do//comment\n"
    "//comment with a def inside\n"
    "end\n"
    "";

    Lexer l;
    std::vector<Token> tokens = l.lex(input);

    REQUIRE(tokens.at(0).value == "def");
    REQUIRE(tokens.at(0).type == Token::Type::FUNCTION_DEFINITION);
    REQUIRE(tokens.at(0).location == Location(3,1));

    REQUIRE(tokens.at(1).value == "func");
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);
    REQUIRE(tokens.at(1).location == Location(3,5));

    REQUIRE(tokens.at(2).value == "(");
    REQUIRE(tokens.at(2).type == Token::Type::OPENING_PARENTHESIS);
    REQUIRE(tokens.at(2).location == Location(3,9));

    REQUIRE(tokens.at(3).value == "a");
    REQUIRE(tokens.at(3).type == Token::Type::IDENTIFIER);
    REQUIRE(tokens.at(3).location == Location(3,10));

    REQUIRE(tokens.at(4).value == ",");
    REQUIRE(tokens.at(4).type == Token::Type::SEPARATOR);
    REQUIRE(tokens.at(4).location == Location(3,11));

    REQUIRE(tokens.at(5).value == "b");
    REQUIRE(tokens.at(5).type == Token::Type::IDENTIFIER);
    REQUIRE(tokens.at(5).location == Location(3,12));

    REQUIRE(tokens.at(6).value == ")");
    REQUIRE(tokens.at(6).type == Token::Type::CLOSING_PARENTHESIS);
    REQUIRE(tokens.at(6).location == Location(3,13));

    REQUIRE(tokens.at(7).value == "do");
    REQUIRE(tokens.at(7).type == Token::Type::KEYWORD);
    REQUIRE(tokens.at(7).location == Location(3,15));

    REQUIRE(tokens.at(8).value == "end");
    REQUIRE(tokens.at(8).type == Token::Type::KEYWORD);
    REQUIRE(tokens.at(8).location == Location(5,1));
}

TEST_CASE("lex if statement", "[Lexer]")
{
    string input = "if x > 3 do\nprint x\nend";

    Lexer l;
    std::vector<Token> tokens = l.lex(input);
    REQUIRE(tokens.at(0).value == "if");
    REQUIRE(tokens.at(0).location == Location(1, 1));
    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).location == Location(1, 4));
    REQUIRE(tokens.at(2).value == ">");
    REQUIRE(tokens.at(3).value == "3");
    REQUIRE(tokens.at(4).value == "do");
    REQUIRE(tokens.at(5).value == "print");
    REQUIRE(tokens.at(6).value == "x");
    REQUIRE(tokens.at(7).value == "end");
    REQUIRE(tokens.at(7).location == Location(3, 1));

    REQUIRE(tokens.at(8).value == "");
    REQUIRE(tokens.at(8).location == Location(4, 1));
    REQUIRE(tokens.at(8).type == Token::Type::END_OF_FILE);
}

TEST_CASE("lex function definition", "[Lexer]")
{
    string input = "def func(a, b) do\na + b\nend";
    Lexer l;
    auto tokens = l.lex(input);

    REQUIRE(tokens.at(0).value == "def");
    REQUIRE(tokens.at(0).location == Location(1, 1));
    REQUIRE(tokens.at(0).type == Token::Type::FUNCTION_DEFINITION);

    REQUIRE(tokens.at(1).value == "func");
    REQUIRE(tokens.at(1).location == Location(1, 5));
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(2).value == "(");
    REQUIRE(tokens.at(2).location == Location(1, 9));
    REQUIRE(tokens.at(2).type == Token::Type::OPENING_PARENTHESIS);

    REQUIRE(tokens.at(3).value == "a");
    REQUIRE(tokens.at(3).location == Location(1, 10));
    REQUIRE(tokens.at(3).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(4).value == ",");
    REQUIRE(tokens.at(4).location == Location(1, 11));
    REQUIRE(tokens.at(4).type == Token::Type::SEPARATOR);

    REQUIRE(tokens.at(5).value == "b");
    REQUIRE(tokens.at(5).location == Location(1, 13));
    REQUIRE(tokens.at(5).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(6).value == ")");
    REQUIRE(tokens.at(6).location == Location(1, 14));
    REQUIRE(tokens.at(6).type == Token::Type::CLOSING_PARENTHESIS);

    REQUIRE(tokens.at(7).value == "do");
    REQUIRE(tokens.at(7).location == Location(1, 16));
    REQUIRE(tokens.at(7).type == Token::Type::KEYWORD);

    REQUIRE(tokens.at(8).value == "a");
    REQUIRE(tokens.at(8).location == Location(2, 1));
    REQUIRE(tokens.at(8).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(9).value == "+");
    REQUIRE(tokens.at(9).location == Location(2, 3));
    REQUIRE(tokens.at(9).type == Token::Type::BIN_OP);

    REQUIRE(tokens.at(10).value == "b");
    REQUIRE(tokens.at(10).location == Location(2, 5));
    REQUIRE(tokens.at(10).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(11).value == "end");
    REQUIRE(tokens.at(11).location == Location(3, 1));
    REQUIRE(tokens.at(11).type == Token::Type::KEYWORD);
}

TEST_CASE("function call followed by function definition", "[Lexer]")
{
    string input =
        "func(2.5, 5.0)\n"
        "// a comment in the middle\n"
        "def func(x, y) do\n"
        "   x+y//another comment\n"
        "end\n"
        "";

    Lexer l;
    auto tokens = l.lex(input);

    REQUIRE(tokens.at(0).value == "func");
    REQUIRE(tokens.at(0).location == Location(1, 1));
    REQUIRE(tokens.at(0).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(1).value == "(");
    REQUIRE(tokens.at(1).location == Location(1, 5));
    REQUIRE(tokens.at(1).type == Token::Type::OPENING_PARENTHESIS);

    REQUIRE(tokens.at(2).value == "2.5");
    REQUIRE(tokens.at(2).location == Location(1, 6));
    REQUIRE(tokens.at(2).type == Token::Type::NUMBER);

    REQUIRE(tokens.at(3).value == ",");
    REQUIRE(tokens.at(3).location == Location(1, 9));
    REQUIRE(tokens.at(3).type == Token::Type::SEPARATOR);

    REQUIRE(tokens.at(4).value == "5.0");
    REQUIRE(tokens.at(4).location == Location(1, 11));
    REQUIRE(tokens.at(4).type == Token::Type::NUMBER);

    REQUIRE(tokens.at(5).value == ")");
    REQUIRE(tokens.at(5).location == Location(1, 14));
    REQUIRE(tokens.at(5).type == Token::Type::CLOSING_PARENTHESIS);

    REQUIRE(tokens.at(6).value == "def");
    REQUIRE(tokens.at(6).location == Location(3, 1));
    REQUIRE(tokens.at(6).type == Token::Type::FUNCTION_DEFINITION);

    REQUIRE(tokens.at(7).value == "func");
    REQUIRE(tokens.at(7).location == Location(3, 5));
    REQUIRE(tokens.at(7).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(8).value == "(");
    REQUIRE(tokens.at(8).location == Location(3, 9));
    REQUIRE(tokens.at(8).type == Token::Type::OPENING_PARENTHESIS);

    REQUIRE(tokens.at(9).value == "x");
    REQUIRE(tokens.at(9).location == Location(3, 10));
    REQUIRE(tokens.at(9).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(10).value == ",");
    REQUIRE(tokens.at(10).location == Location(3, 11));
    REQUIRE(tokens.at(10).type == Token::Type::SEPARATOR);

    REQUIRE(tokens.at(11).value == "y");
    REQUIRE(tokens.at(11).location == Location(3, 13));
    REQUIRE(tokens.at(11).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(12).value == ")");
    REQUIRE(tokens.at(12).location == Location(3, 14));
    REQUIRE(tokens.at(12).type == Token::Type::CLOSING_PARENTHESIS);

    REQUIRE(tokens.at(13).value == "do");
    REQUIRE(tokens.at(13).location == Location(3, 16));
    REQUIRE(tokens.at(13).type == Token::Type::KEYWORD);

    REQUIRE(tokens.at(14).value == "x");
    REQUIRE(tokens.at(14).location == Location(4, 4));
    REQUIRE(tokens.at(14).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(15).value == "+");
    REQUIRE(tokens.at(15).location == Location(4, 5));
    REQUIRE(tokens.at(15).type == Token::Type::BIN_OP);

    REQUIRE(tokens.at(16).value == "y");
    REQUIRE(tokens.at(16).location == Location(4, 6));
    REQUIRE(tokens.at(16).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(17).value == "end");
    REQUIRE(tokens.at(17).location == Location(5, 1));
    REQUIRE(tokens.at(17).type == Token::Type::KEYWORD);
}

TEST_CASE("lex if expression", "[Lexer]")
{
    string input =
    "if 3 < 2 do\n"
    "   25\n"
    "end"
    "";
    Lexer l;
    auto tokens = l.lex(input);

    REQUIRE(tokens.at(0).value == "if");
    REQUIRE(tokens.at(0).location == Location(1,1));
    REQUIRE(tokens.at(0).type == Token::Type::IF);

    REQUIRE(tokens.at(1).value == "3");
    REQUIRE(tokens.at(1).location == Location(1,4));
    REQUIRE(tokens.at(1).type == Token::Type::NUMBER);

    REQUIRE(tokens.at(2).value == "<");
    REQUIRE(tokens.at(2).location == Location(1,6));
    REQUIRE(tokens.at(2).type == Token::Type::BIN_OP);

    REQUIRE(tokens.at(3).value == "2");
    REQUIRE(tokens.at(3).location == Location(1,8));
    REQUIRE(tokens.at(3).type == Token::Type::NUMBER);

    REQUIRE(tokens.at(4).value == "do");
    REQUIRE(tokens.at(4).location == Location(1,10));
    REQUIRE(tokens.at(4).type == Token::Type::KEYWORD);

    REQUIRE(tokens.at(5).value == "25");
    REQUIRE(tokens.at(5).location == Location(2,4));
    REQUIRE(tokens.at(5).type == Token::Type::NUMBER);

    REQUIRE(tokens.at(6).value == "end");
    REQUIRE(tokens.at(6).location == Location(3,1));
    REQUIRE(tokens.at(6).type == Token::Type::KEYWORD);

}

TEST_CASE("lex string literal", "[Lexer,String]")
{
    string input =
    "let x = \"string with keyword do inside\"";

    Lexer l;
    auto tokens = l.lex(input);

    REQUIRE(tokens.at(0).value == "let");
    REQUIRE(tokens.at(0).location == Location(1,1));
    REQUIRE(tokens.at(0).type == Token::Type::VARIABLE_DEFINITION);

    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).location == Location(1,5));
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(2).value == "=");
    REQUIRE(tokens.at(2).location == Location(1,7));
    REQUIRE(tokens.at(2).type == Token::Type::ASSIGN);

    REQUIRE(tokens.at(3).value == "string with keyword do inside");
    REQUIRE(tokens.at(3).location == Location(1,9));
    REQUIRE(tokens.at(3).type == Token::Type::STRING_LITERAL);
}

TEST_CASE("lex string literal with content after string", "[Lexer,String]")
{
    string input =
    "let x = \"string with keyword do inside\"\n"
    "let y = 25"
    "";

    Lexer l;
    auto tokens = l.lex(input);

    REQUIRE(tokens.at(0).value == "let");
    REQUIRE(tokens.at(0).location == Location(1,1));
    REQUIRE(tokens.at(0).type == Token::Type::VARIABLE_DEFINITION);

    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).location == Location(1,5));
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(2).value == "=");
    REQUIRE(tokens.at(2).location == Location(1,7));
    REQUIRE(tokens.at(2).type == Token::Type::ASSIGN);

    REQUIRE(tokens.at(3).value == "string with keyword do inside");
    REQUIRE(tokens.at(3).location == Location(1,9));
    REQUIRE(tokens.at(3).type == Token::Type::STRING_LITERAL);

    REQUIRE(tokens.at(4).value == "let");
    REQUIRE(tokens.at(4).location == Location(2,1));
    REQUIRE(tokens.at(4).type == Token::Type::VARIABLE_DEFINITION);

    REQUIRE(tokens.at(5).value == "y");
    REQUIRE(tokens.at(5).location == Location(2,5));
    REQUIRE(tokens.at(5).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(6).value == "=");
    REQUIRE(tokens.at(6).location == Location(2,7));
    REQUIRE(tokens.at(6).type == Token::Type::ASSIGN);

    REQUIRE(tokens.at(7).value == "25");
    REQUIRE(tokens.at(7).location == Location(2,9));
    REQUIRE(tokens.at(7).type == Token::Type::NUMBER);
}

TEST_CASE("lex multiline string", "[Lexer,String]")
{
    string input =
    "let x = \"string with keyword do inside\n"
    "still inside the string\"\n"
    "let y = 25"
    "";

    Lexer l;
    auto tokens = l.lex(input);

    REQUIRE(tokens.at(0).value == "let");
    REQUIRE(tokens.at(0).location == Location(1,1));
    REQUIRE(tokens.at(0).type == Token::Type::VARIABLE_DEFINITION);

    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).location == Location(1,5));
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(2).value == "=");
    REQUIRE(tokens.at(2).location == Location(1,7));
    REQUIRE(tokens.at(2).type == Token::Type::ASSIGN);

    REQUIRE(tokens.at(3).value == "string with keyword do inside\nstill inside the string");
    REQUIRE(tokens.at(3).location == Location(1,9));
    REQUIRE(tokens.at(3).type == Token::Type::STRING_LITERAL);

    REQUIRE(tokens.at(4).value == "let");
    REQUIRE(tokens.at(4).location == Location(3,1));
    REQUIRE(tokens.at(4).type == Token::Type::VARIABLE_DEFINITION);

    REQUIRE(tokens.at(5).value == "y");
    REQUIRE(tokens.at(5).location == Location(3,5));
    REQUIRE(tokens.at(5).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(6).value == "=");
    REQUIRE(tokens.at(6).location == Location(3,7));
    REQUIRE(tokens.at(6).type == Token::Type::ASSIGN);

    REQUIRE(tokens.at(7).value == "25");
    REQUIRE(tokens.at(7).location == Location(3,9));
    REQUIRE(tokens.at(7).type == Token::Type::NUMBER);
}


TEST_CASE("string literal escape", "[Lexer,String]")
{
    string input =
    "let x = \"string with \\\" escaped string end\"\n"
    "let y = 25"
    "";

    Lexer l;
    auto tokens = l.lex(input);

    REQUIRE(tokens.at(0).value == "let");
    REQUIRE(tokens.at(0).location == Location(1,1));
    REQUIRE(tokens.at(0).type == Token::Type::VARIABLE_DEFINITION);

    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).location == Location(1,5));
    REQUIRE(tokens.at(1).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(2).value == "=");
    REQUIRE(tokens.at(2).location == Location(1,7));
    REQUIRE(tokens.at(2).type == Token::Type::ASSIGN);

    REQUIRE(tokens.at(3).value == "string with \\\" escaped string end");
    REQUIRE(tokens.at(3).location == Location(1,9));
    REQUIRE(tokens.at(3).type == Token::Type::STRING_LITERAL);

    REQUIRE(tokens.at(4).value == "let");
    REQUIRE(tokens.at(4).location == Location(2,1));
    REQUIRE(tokens.at(4).type == Token::Type::VARIABLE_DEFINITION);

    REQUIRE(tokens.at(5).value == "y");
    REQUIRE(tokens.at(5).location == Location(2,5));
    REQUIRE(tokens.at(5).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(6).value == "=");
    REQUIRE(tokens.at(6).location == Location(2,7));
    REQUIRE(tokens.at(6).type == Token::Type::ASSIGN);

    REQUIRE(tokens.at(7).value == "25");
    REQUIRE(tokens.at(7).location == Location(2,9));
    REQUIRE(tokens.at(7).type == Token::Type::NUMBER);
}

TEST_CASE("lex function with types", "[Lexer,Types]")
{
    string input =
    "def func(a:F64, b : String) : String do\n"
    "end"
    "";

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

    REQUIRE(tokens.at(4).value == ":");
    REQUIRE(tokens.at(4).location == Location(1,11));
    REQUIRE(tokens.at(4).type == Token::Type::TYPE_SPECIFIER);

    REQUIRE(tokens.at(5).value == "F64");
    REQUIRE(tokens.at(5).location == Location(1,12));
    REQUIRE(tokens.at(5).type == Token::Type::TYPE_FLOAT_64);

    REQUIRE(tokens.at(6).value == ",");
    REQUIRE(tokens.at(6).location == Location(1,15));
    REQUIRE(tokens.at(6).type == Token::Type::SEPARATOR);

    REQUIRE(tokens.at(7).value == "b");
    REQUIRE(tokens.at(7).location == Location(1,17));
    REQUIRE(tokens.at(7).type == Token::Type::IDENTIFIER);

    REQUIRE(tokens.at(8).value == ":");
    REQUIRE(tokens.at(8).location == Location(1,19));
    REQUIRE(tokens.at(8).type == Token::Type::TYPE_SPECIFIER);

    REQUIRE(tokens.at(9).value == "String");
    REQUIRE(tokens.at(9).location == Location(1,21));
    REQUIRE(tokens.at(9).type == Token::Type::TYPE_STRING);

    REQUIRE(tokens.at(10).value == ")");
    REQUIRE(tokens.at(10).location == Location(1,27));
    REQUIRE(tokens.at(10).type == Token::Type::CLOSING_PARENTHESIS);

    REQUIRE(tokens.at(11).value == ":");
    REQUIRE(tokens.at(11).location == Location(1,29));
    REQUIRE(tokens.at(11).type == Token::Type::TYPE_SPECIFIER);

    REQUIRE(tokens.at(12).value == "String");
    REQUIRE(tokens.at(12).location == Location(1,31));
    REQUIRE(tokens.at(12).type == Token::Type::TYPE_STRING);

    REQUIRE(tokens.at(13).value == "do");
    REQUIRE(tokens.at(13).location == Location(1,38));
    REQUIRE(tokens.at(13).type == Token::Type::KEYWORD);

    REQUIRE(tokens.at(14).value == "end");
    REQUIRE(tokens.at(14).location == Location(2,1));
    REQUIRE(tokens.at(14).type == Token::Type::KEYWORD);
}