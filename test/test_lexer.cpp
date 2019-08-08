#include"../catch2/catch.hpp"
#include"../lib/lexer.hpp"

TEST_CASE("lex variable definition","[Lexer]") {
    std::string input = "let x = 5";

    namespace hb = HoneyBadger;
    hb::Lexer l;
    std::vector<hb::Token> tokens = l.lex(input);
    REQUIRE(tokens.at(0).value == "let");
    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(2).value == "=");
    REQUIRE(tokens.at(3).value == "5");

}

TEST_CASE("lex if statement","[Lexer]") {
    std::string input = "if x > 3 do\nprint x\nend";

    namespace hb = HoneyBadger;
    hb::Lexer l;
    std::vector<hb::Token> tokens = l.lex(input);
    REQUIRE(tokens.at(0).value == "if");
    REQUIRE(tokens.at(0).location == hb::Location(1,1));
    REQUIRE(tokens.at(1).value == "x");
    REQUIRE(tokens.at(1).location == hb::Location(1,4));
    REQUIRE(tokens.at(2).value == ">");
    REQUIRE(tokens.at(3).value == "3");
    REQUIRE(tokens.at(4).value == "do");
    REQUIRE(tokens.at(5).value == "print");
    REQUIRE(tokens.at(6).value == "x");
    REQUIRE(tokens.at(7).value == "end");
    REQUIRE(tokens.at(7).location == hb::Location(3,1));
}
