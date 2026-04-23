#include <gtest/gtest.h>
#include "../src/ar_model.h"
// #define CHECK_STATS(data, expected_mean, expected_var) \
//     do { \
//         auto [mean, var] = compute_stats(data); \
//         EXPECT_DOUBLE_EQ(mean, expected_mean); \
//         EXPECT_DOUBLE_EQ(var, expected_var); \
//     } while (0)
    
TEST(ComputeStatsTest, PositiveMeanPositiveVariance) {
    // +ve mean & +ve var
    std::vector<double> data{2.0, 4.0, 6.0, 8.0, 10.0};
    auto [mean, var] = compute_stats(data);
    EXPECT_DOUBLE_EQ(mean, 6.0);
    EXPECT_DOUBLE_EQ(var, 8.0);
}

TEST(ComputeStatsTest, ZeroMeanZeroVarianceIdenticalData) {
    // 0 mean & 0 var, identical data
    std::vector<double> data(3, 0.0);
    auto [mean, var] = compute_stats(data);
    EXPECT_DOUBLE_EQ(mean, 0.0);
    EXPECT_DOUBLE_EQ(var, 0.0);
}

TEST(ComputeStatsTest, NegativeMeanPositiveVariance) {
    // -ve mean & +ve var
    std::vector<double> data{-2.0, -2.0, -2.0, -2.0, 0.0};
    auto [mean, var] = compute_stats(data);
    EXPECT_DOUBLE_EQ(mean, -1.6);
    EXPECT_DOUBLE_EQ(var, 0.64);
}

TEST(ComputeStatsTest, SingleData) {
    std::vector<double> data{0.0};
    auto [mean, var] = compute_stats(data);
    EXPECT_DOUBLE_EQ(mean, 0.0);
    EXPECT_DOUBLE_EQ(var, 0.0);
}

TEST(ComputeStatsTest, EmptyData) {
    std::vector<double> data;
    auto [mean, var] = compute_stats(data);
    EXPECT_TRUE(std::isnan(mean));
}