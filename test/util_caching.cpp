// google test docs
// wiki page: https://code.google.com/p/googletest/w/list
// primer: https://code.google.com/p/googletest/wiki/V1_7_Primer
// FAQ: https://code.google.com/p/googletest/wiki/FAQ
// advanced guide: https://code.google.com/p/googletest/wiki/V1_7_AdvancedGuide
// samples: https://code.google.com/p/googletest/wiki/V1_7_Samples
//
// List of some basic tests fuctions:
// Fatal assertion                      Nonfatal assertion Verifies /
// Description
//-------------------------------------------------------------------------------------------------------------------------------------------------------
// ASSERT_EQ(expected, actual);         EXPECT_EQ(expected, actual); expected ==
// actual ASSERT_NE(val1, val2);               EXPECT_NE(val1, val2); val1 !=
// val2 ASSERT_LT(val1, val2);               EXPECT_LT(val1, val2); val1 < val2
// ASSERT_LE(val1, val2);               EXPECT_LE(val1, val2); val1 <= val2
// ASSERT_GT(val1, val2);               EXPECT_GT(val1, val2); val1 > val2
// ASSERT_GE(val1, val2);               EXPECT_GE(val1, val2); val1 >= val2
//
// ASSERT_FLOAT_EQ(expected, actual);   EXPECT_FLOAT_EQ(expected, actual);   the
// two float values are almost equal (4 ULPs) ASSERT_DOUBLE_EQ(expected,
// actual); EXPECT_DOUBLE_EQ(expected, actual);  the two double values are
// almost equal (4 ULPs) ASSERT_NEAR(val1, val2, abs_error);  EXPECT_NEAR(val1,
// val2, abs_error); the difference between val1 and val2 doesn't exceed the
// given absolute error
//
// Note: more information about ULPs can be found here:
// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
//
// Example of two unit test:
// TEST(Math, Add) {
//    ASSERT_EQ(10, 5+ 5);
//}
//
// TEST(Math, Float) {
//	  ASSERT_FLOAT_EQ((10.0f + 2.0f) * 3.0f, 10.0f * 3.0f + 2.0f * 3.0f)
//}
//=======================================================================================================================================================
#include <thread>
#include "gtest/gtest.h"

#include "cache.hpp"

using namespace util_caching;
using namespace std::chrono_literals;

struct SomePolicyWithoutParams {
    SomePolicyWithoutParams() = default;
    bool operator()(const Time& /*lhs*/, const Time& /*rhs*/) const {
        return true;
    }
};


class CacheTest : public ::testing::Test {
protected:
    double key1{1.};
    double key2{1.2};
    double key3{1.6};
    Time time1 = std::chrono::steady_clock::now();
    Time time2 = time1 + 10ms;
    Time time3 = time1 + 1100ms;
    Time time4 = time1 + 2100ms;
    Cache<double, double> cacheByNumber;
    Cache<Time, double> cacheByTime;
    util_caching::policies::ApproximateNumber<double> approximateNumberPolicy{0.5};
    util_caching::policies::ApproximateTime<Time, std::chrono::milliseconds> approximateTimePolicy{100};
    util_caching::policies::ApproximateTime<Time, std::chrono::seconds> approximateTimePolicy2{1};
    SomePolicyWithoutParams dummyPolicy;
};

TEST_F(CacheTest, TestWithNumberKey) {
    EXPECT_FALSE(cacheByNumber.cached(key1));
    cacheByNumber.cache(key1, 1.);

    // exact match
    EXPECT_TRUE(cacheByNumber.cached(key1));
    EXPECT_DOUBLE_EQ(*cacheByNumber.cached(key1), 1.);

    // approximate match
    EXPECT_TRUE(cacheByNumber.cached(key2, approximateNumberPolicy));
    EXPECT_DOUBLE_EQ(*cacheByNumber.cached(key2, approximateNumberPolicy), 1.);

    // over threshold
    EXPECT_FALSE(cacheByNumber.cached(key3, approximateNumberPolicy));
}


TEST_F(CacheTest, TestWithTimeKey) {
    EXPECT_FALSE(cacheByTime.cached(time1));
    cacheByTime.cache(time1, 1.);

    // exact match
    EXPECT_TRUE(cacheByTime.cached(time1));
    EXPECT_DOUBLE_EQ(*cacheByTime.cached(time1), 1.);

    // approximate match with miliseconds
    EXPECT_TRUE(cacheByTime.cached(time2, approximateTimePolicy));
    EXPECT_DOUBLE_EQ(*cacheByTime.cached(time2, approximateTimePolicy), 1.);

    // approximate match with seconds
    EXPECT_TRUE(cacheByTime.cached(time2, approximateTimePolicy2));
    EXPECT_DOUBLE_EQ(*cacheByTime.cached(time2, approximateTimePolicy2), 1.);

    // over threshold
    EXPECT_FALSE(cacheByTime.cached(time3, approximateTimePolicy));
    // exactly 1s after rounding to integer
    EXPECT_TRUE(cacheByTime.cached(time3, approximateTimePolicy2));
    // expect 2s after rounding to integer which is over threshold
    EXPECT_FALSE(cacheByTime.cached(time4, approximateTimePolicy2));
}

TEST_F(CacheTest, TestWithOtherComparisonPolicy) {
    cacheByTime.cache(time1, 1.);
    EXPECT_TRUE(cacheByTime.cached<SomePolicyWithoutParams>(time2));
    EXPECT_TRUE(cacheByTime.cached<SomePolicyWithoutParams>(time2, dummyPolicy));
}
