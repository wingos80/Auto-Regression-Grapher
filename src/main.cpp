#include <iostream> // can delete this now that ar_model.h is including iostream?
#include <string>
#include <array>
#include "ar_model.h"
#include "renderer.h"
#include "exceptions.h"

int main(){
    std::array<double, 2> coefficients{0.5, -0.4};
    double constant = 1;
    int num_steps = 1000;
    std::vector<double> initial_values{10.0, 0.0};
    std::default_random_engine generator_1;
    ARModel<double, 2> ar_model(coefficients, constant, initial_values, generator_1);
    ARModel<double, 2> ar2_model(std::move(ar_model));
    std::vector<double> result = ar2_model.generate(num_steps);
    render_series(result);
    auto stats = compute_stats(result);  // this feels verbose, can i write it neater?
    std::cout << "mean and std of double are: " << stats.first << " " << stats.second << std::endl;
    
    std::default_random_engine generator_2;
    ARModel<float, 1> ar_float({0.8f}, 0.5f, {5.0f}, generator_2);
    std::vector<float> result_float = ar_float.generate(num_steps);
    std::vector<double> float_as_double(result_float.begin(), result_float.end());  // remind me how this line works?
    render_series(float_as_double);
    auto stats_float = compute_stats(float_as_double);  // this feels verbose, can i write it neater?
    std::cout << "mean and std of float are: " << stats_float.first << " " << stats_float.second << std::endl;

    int n = 10;
    auto ten_run_stats = ar_float.get_stats_history(n, num_steps);
    for (const auto& [key, value] : ten_run_stats){
        std::cout << "mean and std of run " << key << " are: " << value.first << " " << value.second << std::endl;
    }

    int run_index;
    std::cout << "Enter run index to print: ";
    std::cin >> run_index; // Reads until whitespace
    auto stats_new = ar_float.get_run_stats(ten_run_stats, run_index);
    if (stats_new) {
        std::cout << stats_new->first << " " << stats_new->second << std::endl;
    } else {
        std::cout << "run not found" << std::endl;
    }

    return 0;
}