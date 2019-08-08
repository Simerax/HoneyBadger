#define CATCH_CONFIG_MAIN
#include"../catch2/catch.hpp"
// #include"../fakeit/fakeit.hpp"
//
//  Do not add anything here. This file is just there to supply catch with an entry point.
//  Do not redefine CATCH_CONFIG_MAIN !
//
//

// Example Test with Catch2
// See https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md#writing-tests
//
//
// TEST_CASE("Adding 2 Numbers", "[tag]")
// {
//     REQUIRE(2 + 2 == 4);
// }


// Example Mock with FakeIt within a Catch2 Test
// See https://github.com/eranpeer/FakeIt/blob/master/README.md#fakeit
//
//
// TEST_CASE("FAKEIT", "[fakeit]")
// {
//     struct Interface {
//         virtual int foo(int) = 0;
//     };

//     fakeit::Mock<Interface> mock;
//     fakeit::When(Method(mock, foo)).Return(1);

//     Interface& i = mock.get();

//     REQUIRE(i.foo(0) == 1);
// }
