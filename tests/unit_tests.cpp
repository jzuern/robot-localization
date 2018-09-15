//
// Created by jannik on 15.09.18.
//

#include <gtest/gtest.h>

TEST(MyTestSuitName, MyTestCaseName) {
    int actual = 1;
    EXPECT_GT(actual, 0);
    EXPECT_EQ(1, actual) << "Should be equal to one";
}