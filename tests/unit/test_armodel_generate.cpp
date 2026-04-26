#include <algorithm>
#include <gtest/gtest.h>
#include "ar_model.h"

class ARModelGenerateTest : public ::testing::Test {
protected:
    std::vector<float> initial_values{5.0f};
    ARModel<float, 1> model{{0.8f}, 0.5f, initial_values, std::default_random_engine{}};
    int num_steps = 1000;
    std::vector<float> result = model.generate(num_steps);
};

TEST_F(ARModelGenerateTest, ResultLength){
    ASSERT_EQ(result.size(), static_cast<size_t>(num_steps));
}

TEST_F(ARModelGenerateTest, ResultStability){
    // in reasonable cases result should never explode
    ASSERT_TRUE(std::all_of(result.begin(), result.end(), [](float n) {return n < 100.0f && n > -100.0f;}));
}

TEST_F(ARModelGenerateTest, ResultInitialValues){
    ASSERT_EQ(result[0], initial_values[0]);
    // std::vector<int> sliced(result.begin(), result.begin() + 1);
}

TEST(ARModelExceptionsTest, TestConstructorException){
    using Model = ARModel<float, 2>;
    std::array<float, 2> coeffs{0.8f, 0.3f};
    std::vector<float> init{5.0f};
    std::default_random_engine gen{};
    EXPECT_THROW(Model(coeffs, 0.5f, init, gen), BadARModelParameter);

    std::vector<float> init_2{5.0f,0.0f};
    EXPECT_NO_THROW(Model(coeffs, 0.5f, init_2, gen));
}

TEST(ARModelExceptionsTest, TestGenerateExceptions){
    ARModel<float, 2> model{{0.8f, 1.0f}, 0.5f, {1.0f, 1.0f}, std::default_random_engine{}};
    EXPECT_NO_THROW(model.generate(2));
    EXPECT_THROW(model.generate(1), BadARModelParameter);
    EXPECT_THROW(model.generate(0), BadARModelParameter);
    EXPECT_THROW(model.generate(-1), BadARModelParameter);
}