#include <gtest/gtest.h>

#include "vector.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>


TEST(VectorIteratorTest, RangeBasedFor) {
    Vector<int> v = {1, 2, 3, 4, 5};
    
    std::string ans;
    for (int x : v) {
        ans += std::to_string(x);
    }

    EXPECT_EQ(ans, "12345");
}

TEST(VectorIteratorTest, IteratorModification) {
    Vector<int> v = {1, 2, 3, 4, 5};

    for (auto it = v.begin(); it != v.end(); ++it) {
        *it *= 2;
    }

    std::string ans;
    for (int x : v) {
        ans += std::to_string(x);
    }

    EXPECT_EQ(ans, "246810");
}

TEST(VectorIteratorTest, ConstantVector) {
    Vector<int> v = {1, 2, 3, 4, 5};
    const Vector<int>& cv = v;

    std::string ans;
    for (auto it = cv.begin(); it != cv.end(); ++it) {
        ans += std::to_string(*it);
    }

    EXPECT_EQ(ans, "12345");
}

TEST(VectorIteratorTest, ConstantIterators) {
    Vector<int> v = {1, 2, 3, 4, 5};

    std::string ans;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        std::cout << *it << " ";
        ans += std::to_string(*it);
    }

    EXPECT_EQ(ans, "12345");
}

TEST(VectorIteratorTest, RandomAccessOperations) {
    Vector<int> v = {1, 2, 3, 4, 5};

    auto it = v.begin();
    EXPECT_EQ(it[2], 3);
    EXPECT_EQ(*(it + 3), 4);
}

TEST(VectorIteratorTest, IteratorDifference) {
    Vector<int> v = {1, 2, 3, 4, 5};
    EXPECT_EQ((v.end() - v.begin()), 5);
}

TEST(VectorIteratorTest, IteratorComparison) {
    Vector<int> v = {1, 2, 3, 4, 5};
    EXPECT_TRUE(v.begin() < v.end());
}

TEST(VectorIteratorTest, StdAlgorithms) {
    Vector<int> v = {1, 2, 3, 4, 5};
    std::sort(v.begin(), v.end(), std::greater<int>());

    std::string ans;
    for (int x : v) {
        ans += std::to_string(x);
    }
    EXPECT_EQ(ans, "54321");

    int sum = std::accumulate(v.begin(), v.end(), 0);
    EXPECT_EQ(sum, 15);

    auto found = std::find(v.begin(), v.end(), 3);
    EXPECT_TRUE(found != v.end());
    EXPECT_EQ(*found, 3);
}

TEST(VectorIteratorTest, IteratorImplicitConstCast) {
    Vector<int> v = {1, 2, 3, 4, 5};
    Vector<int>::const_iterator cit = v.begin();
    EXPECT_EQ(*cit, 1);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}