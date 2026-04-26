#include <gtest/gtest.h>
#include "ar_model.h"

TEST(ARModelPropertiesTest, SeriesStatistics){
    double constant = 0.9;
    double coefficient = 0.4;
    ARModel<double, 1> model{{coefficient}, constant, {0.0}, std::default_random_engine{}};
    double expected_mean = constant / (1 - coefficient);

    auto results = model.generate(10000);
    auto [mean, _] = compute_stats(results);
    EXPECT_NEAR(mean, expected_mean, 0.05);
}