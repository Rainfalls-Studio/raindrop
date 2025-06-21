#include <gtest/gtest.h>
#include "Raindrop/Core/Utility/Flags.hpp"
#include <limits>

enum class TestEnum : uint8_t {
    None = 0,
    FlagA = 1 << 0,
    FlagB = 1 << 1,
    FlagC = 1 << 2,
    All = FlagA | FlagB | FlagC
};

using Raindrop::Utils::Flags;

class FlagsTest : public ::testing::Test {
protected:
    using TestFlags = Flags<TestEnum>;
};

// Test: Code compiles successfully with valid syntax and no errors.
TEST_F(FlagsTest, test_code_compiles_successfully) {
    // This test will always pass if the code compiles.
    SUCCEED();
}

// Test: Code executes and produces the expected output for typical input values.
TEST_F(FlagsTest, test_code_executes_expected_output) {
    TestFlags f1(TestEnum::FlagA);
    TestFlags f2(TestEnum::FlagB);
    TestFlags f3 = f1 | f2;

    EXPECT_EQ(
        static_cast<uint8_t>(TestEnum::FlagA) | static_cast<uint8_t>(TestEnum::FlagB),
        static_cast<uint8_t>(f3.operator|(TestEnum::None))
    );

    f3 |= TestEnum::FlagC;

    EXPECT_EQ(
        static_cast<uint8_t>(TestEnum::All),
        static_cast<uint8_t>(f3.operator|(TestEnum::None))
    );

    TestFlags f4({TestEnum::FlagA, TestEnum::FlagB});

    EXPECT_EQ(
        static_cast<uint8_t>(TestEnum::FlagA) | static_cast<uint8_t>(TestEnum::FlagB),
        static_cast<uint8_t>(f4.operator|(TestEnum::None))
    );
}

// Test: Code handles invalid or unexpected input gracefully without crashing.
TEST_F(FlagsTest, test_handles_invalid_input) {
    // Using an invalid enum value (not defined in TestEnum)
    TestFlags f(static_cast<TestEnum>(0xFF));
    // Should not crash, and should store the bit pattern
    EXPECT_EQ(
        static_cast<uint8_t>(0xFF),
        static_cast<uint8_t>(f.operator|(TestEnum::None))
    );
}

// Test: Code behaves correctly when invoked with boundary values, such as minimum or maximum allowed parameters.
TEST_F(FlagsTest, test_boundary_value_handling) {
    using Bits = typename TestFlags::Bits;

    TestFlags minFlag(static_cast<Bits>(0));
    TestFlags maxFlag(std::numeric_limits<Bits>::max());

    EXPECT_EQ(
        static_cast<Bits>(0),
        static_cast<Bits>(minFlag.operator|(static_cast<Bits>(0)))
    );

    EXPECT_EQ(
        std::numeric_limits<Bits>::max(),
        static_cast<Bits>(maxFlag.operator|(static_cast<Bits>(0)))
    );

    minFlag |= TestEnum::FlagA;

    EXPECT_EQ(
        static_cast<Bits>(TestEnum::FlagA),
        static_cast<Bits>(minFlag.operator|(static_cast<Bits>(0)))
    );
}