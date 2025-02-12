#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

TEST_CASE("random"){

    CHECK(1 == 1);
    SECTION("sekce"){
        std::string hello;
        REQUIRE(hello.empty());
    }
}