TEST_CASE("Cast to u64") {
    u64 in = 12345678;

    UBigInt i{ 12345678 };

    u64 out{};

    i.to_u64(out);

    CHECK(out == in);
}
