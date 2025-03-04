// tests/point_test.cpp
#include <gtest/gtest.h>
#include "femto.hpp"
#include <sstream>
#include <chrono>

// Test fixture for point tests
template <typename T>
class PointTest : public testing::Test
{
protected:
    femto::point2_t<T> origin{}; // Test zero-initialized point
    femto::point2_t<T> p1{1, 2}; // Test basic point
    femto::point2_t<T> p2{3, 4}; // Test another point
};

// Test types we want to check
using TestTypes = ::testing::Types<int, float, double>;
TYPED_TEST_SUITE(PointTest, TestTypes);

// Constructor tests
TYPED_TEST(PointTest, DefaultConstructor)
{
    EXPECT_EQ(this->origin.x(), TypeParam{});
    EXPECT_EQ(this->origin.y(), TypeParam{});
}

// Addition operator
TYPED_TEST(PointTest, Addition)
{
    auto result = this->p1 + this->p2;
    EXPECT_EQ(result.x(), TypeParam{4});
    EXPECT_EQ(result.y(), TypeParam{6});
    std::cerr << result << std::endl;
}

// Subtraction operator
TYPED_TEST(PointTest, Subtraction)
{
    auto result = this->p2 - this->p1;
    EXPECT_EQ(result.x(), TypeParam{2});
    EXPECT_EQ(result.y(), TypeParam{2});
}

// Scalar multiplication
TYPED_TEST(PointTest, ScalarMultiplication)
{
    auto result = this->p1 * TypeParam{2};
    EXPECT_EQ(result.x(), TypeParam{2});
    EXPECT_EQ(result.y(), TypeParam{4});
}

// Dot product
TYPED_TEST(PointTest, DotProduct)
{
    auto result = this->p1 * this->p2;
    EXPECT_EQ(result, TypeParam{11}); // (1*3 + 2*4)
}

// Compound addition
TYPED_TEST(PointTest, CompoundAddition)
{
    auto point = this->p1;
    point += this->p2;
    EXPECT_EQ(point.x(), TypeParam{4});
    EXPECT_EQ(point.y(), TypeParam{6});
}

// Equality operator
TYPED_TEST(PointTest, Equality)
{
    femto::point2_t<TypeParam> p3{1, 2};
    EXPECT_EQ(this->p1, p3);
    EXPECT_NE(this->p1, this->p2);
}

// Non-member scalar multiplication
TYPED_TEST(PointTest, NonMemberScalarMultiplication)
{
    auto result = TypeParam{2} * this->p1;
    EXPECT_EQ(result.x(), TypeParam{2});
    EXPECT_EQ(result.y(), TypeParam{4});
}

// Edge cases
TEST(PointEdgeCases, LargeValues)
{
    femto::point2_t<int> large(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    femto::point2_t<int> one(1, 1);

    // This should cause overflow, testing if it handles gracefully
    auto result = large + one;
    EXPECT_NE(result, large);
}

// Floating point specific tests
TEST(PointFloatingPoint, NearZero)
{
    femto::point2_t<double> tiny(1e-15, 1e-15);
    femto::point2_t<double> zero;

    EXPECT_NE(tiny, zero);
    EXPECT_NEAR(tiny.magnitude(), 1.4142e-15, 1e-16);
}

TEST(ThreadTest, Priority)
{
    using namespace std::chrono_literals;
    std::thread t = std::thread([&t]
                                { std::this_thread::sleep_for(100ms); });
    EXPECT_TRUE(femto::utils::set_thread_priority(t, SCHED_OTHER, 30));
    t.join();
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}