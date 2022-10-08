#include "include/types.h"

TEST_CASE("Empty constructor") {
    UBigInt i{};

    CHECK(i.bytes == nullptr);
    CHECK(i.length == 0);
}

TEST_CASE("u64 constructor") {
    // 0001111111011111000110111100010001000101101010101100111100010011 (binary) = 2296584864778604307 (decimal) is a number that uses 8 bytes
    UBigInt i{ 2296584864778604307 };

    u64* i_as_u64 = (u64*) i.bytes;

    REQUIRE(i.length > 0);
    CHECK(*i_as_u64 == (u64) 2296584864778604307);
}

TEST_CASE("String constructor") {
    SUBCASE("Binary") {
        UBigInt i{"0b0001111111011111000110111100010001000101101010101100111100010011"}; 

        u64* i_as_u64 = (u64*) i.bytes;

        REQUIRE(i.length > 0);
        CHECK(*i_as_u64 == (u64) 2296584864778604307);
    }

    SUBCASE("Octal") {
        UBigInt i{"0177370674210552547423"}; 

        u64* i_as_u64 = (u64*) i.bytes;

        REQUIRE(i.length > 0);
        CHECK(*i_as_u64 == (u64) 2296584864778604307);
    }

    SUBCASE("Hexacedimal") {
        UBigInt i{"0x1FDF1BC445AACF13"}; 

        u64* i_as_u64 = (u64*) i.bytes;

        REQUIRE(i.length > 0);
        CHECK(*i_as_u64 == (u64) 2296584864778604307);
    }

    SUBCASE("Decimal") {
        UBigInt i{"2296584864778604307"}; 

        u64* i_as_u64 = (u64*) i.bytes;

        REQUIRE(i.length > 0);
        CHECK(*i_as_u64 == (u64) 2296584864778604307);
    }
}

TEST_CASE("Copy constructor") {
    UBigInt x{2296584864778604307};
    UBigInt y{x};

    u64* y_as_u64 = (u64*) y.bytes;

    REQUIRE(y.length > 0);
    CHECK(*y_as_u64 == (u64) 2296584864778604307);
}
