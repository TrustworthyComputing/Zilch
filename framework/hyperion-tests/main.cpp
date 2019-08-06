#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    // ::testing::GTEST_FLAG(filter) = "zMIPS*";
    ::testing::GTEST_FLAG(filter) = "instructions*";
    return RUN_ALL_TESTS();
}
