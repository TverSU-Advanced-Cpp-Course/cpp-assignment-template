#include <gtest/gtest.h>

#include "placeholder.hpp"

TEST(SmokeTest, PlaceholderWorks) {
    EXPECT_EQ(Placeholder(), 42);
}