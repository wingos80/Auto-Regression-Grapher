#include <gtest/gtest.h>
#include "../src/ar_model.h"

class GetRunStatsTest : public ::testing::Test {
protected:
    ARModel<double, 2> model{{0.5, -0.4}, 1.0, {10.0, 0.0}, std::default_random_engine{}};
};

TEST_F(GetRunStatsTest, ExistingRun) {
    std::unordered_map<int, std::pair<double, double>> test_map = {{1, std::pair{1.0,1.1}}};
    auto result = model.get_run_stats(test_map, 1); 
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->first, 1.0);
    EXPECT_EQ(result->second, 1.1);
}

TEST_F(GetRunStatsTest, NonExistingRun) {
    std::unordered_map<int, std::pair<double, double>> test_map = {{1, std::pair{1.0,1.0}}};
    auto result = model.get_run_stats(test_map, 2);
    EXPECT_FALSE(result.has_value());
}