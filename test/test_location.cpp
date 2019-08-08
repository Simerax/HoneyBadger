#include"../catch2/catch.hpp"
#include"../lib/lexer.hpp"

namespace hb = HoneyBadger;

TEST_CASE("location with initial values","[Location]") {

    hb::Location a(1,2);

    REQUIRE(a.column == 2);
    REQUIRE(a.line == 1);
}

TEST_CASE("location can be compared","[Location]") {
    REQUIRE(hb::Location(1,2) == hb::Location(1,2));
}