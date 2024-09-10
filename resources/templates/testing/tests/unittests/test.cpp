$$SIGNATURE$$#include "gtest/gtest.h"

/// This is a test fixture that does a init for each test
class MyTest : public ::testing::Test
{
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

// example test
TEST_F(MyTest, test_0)
{
    EXPECT_TRUE(true);
}
