#ifndef AR_MODEL_H
#define AR_MODEL_H

#include <concepts>
#include <vector>
#include <random>
#include <cmath>
#include <array>
#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <optional>
#include <format>
#include <iostream>
#include "exceptions.h"

template <std::floating_point T>
std::pair<T, T> compute_stats(const std::vector<T>& data){
    if (data.empty()) {
        std::cerr << "warning: compute_stats called with empty data" << std::endl;
    }
    // calculate mean
    int length = data.size();
    T mean = std::accumulate(data.begin(), data.end(), T{0})/length;  // accumulate returns type of init!!

    T var = std::inner_product(
            data.begin(), data.end(),
            data.begin(), T{0},
            std::plus<T>{},
            [mean](T a, T b){ return (a - mean) * (b - mean); }
    ) / length;
    return {mean, var};
}

class Buffer {
public:
    Buffer(int size) 
        : size(size), data(new double[size]) {};

    // copy constructor
    Buffer(const Buffer& other){
        size = other.size;
        data = new double[size];
        std::copy(other.data, other.data + size, data);
    };

    // copy assignment
    Buffer& operator=(const Buffer& other){
        size = other.size;
        delete[] data;
        data = new double[size];
        std::copy(other.data, other.data + size, data);

        return *this;
    };
 

    // move constructor
    Buffer(Buffer&& other){
        size = other.size;
        data = other.data;
        other.data = nullptr;
        other.size = 0;
    };

    Buffer& operator=(Buffer&& other){
        size = other.size;
        delete[] data;
        data = other.data;
        other.data = nullptr;
        other.size = 0;
        return *this;
    };

    ~Buffer() { delete[] data; };

private:
    int size;
    double* data;
};


template <std::floating_point T, int Order>
class ARModel {
public:
    // what is this constructor type called? 
    ARModel(std::array<T, Order> coefficients, T constant, std::vector<T> initial_values, std::default_random_engine generator)
    : coefficients(std::move(coefficients)), constant(std::move(constant)), initial_values(std::move(initial_values)), generator(std::move(generator))
    {
        if (this->initial_values.size() < Order) {
            std::string err_msg = std::format("initial_values.size() ({}) < Order ({}): AR series undefined", this->initial_values.size(), Order);
            throw BadARModelParameter(err_msg);
        }
    };
    
    // copy constructor
    ARModel(const ARModel& other)
    : coefficients(other.coefficients),  // why can other.coefficients work if coefficients is a private attribute?
      constant(other.constant), 
      initial_values(other.initial_values), 
      generator(other.generator)
    {};

    // copy assignment
    ARModel& operator=(const ARModel& other) {
        coefficients = other.coefficients;
        constant = other.constant;
        initial_values = other.initial_values;
        generator = other.generator;
        return *this;
    }

    // move constructor
    ARModel(ARModel&& other)
    : coefficients(std::move(other.coefficients)), 
      constant(std::move(other.constant)), 
      initial_values(std::move(other.initial_values)), 
      generator(std::move(other.generator))
    {};

    // move assignment
    ARModel& operator=(ARModel&& other) {
        coefficients = std::move(other.coefficients);
        constant = std::move(other.constant);
        initial_values = std::move(other.initial_values);
        generator = std::move(other.generator);
        return *this;
    }

    std::vector<T> generate(int num_steps){
    // runtime exceptions check
    if (num_steps < 0) {
        std::string msg = std::format("num_steps ({}) < 0: AR Series steps cannot be negative", num_steps);
        throw BadARModelParameter(msg);
    }
    if (num_steps < Order) {
        std::string msg = std::format("num_steps ({}) < Order ({}): AR Series generation undefined for this condition", num_steps, Order);
        throw BadARModelParameter(msg);
    }

    //1
    std::vector<T> result = initial_values;
    std::normal_distribution<T> distribution(0.0, 0.005);

    //2
    for (int i=Order; i< num_steps; i++){
        T new_value = constant;
        T epsilon = distribution(generator);
        for (int j=0; j<Order; j++){
            new_value += result[i-1-j]*coefficients[j];
        }
        result.push_back(new_value + epsilon);
    }

    return result;
    };

    std::unordered_map<int, std::pair<T,T>> get_stats_history(int num_runs, int num_steps){
        std::unordered_map<int, std::pair<T,T>> all_stats;
        for (int i = 0; i < num_runs; i++){
            std::pair<T, T> stats = compute_stats(generate(num_steps));
            all_stats[i] = stats;
        }
        return all_stats;
    };
    
    std::optional<std::pair<T, T>> get_run_stats(const std::unordered_map<int, std::pair<T,T>>& history, int run_index){
        if (history.contains(run_index)){
            return history.at(run_index);
        }
        else {
            return std::nullopt;
        };
    };
private:
    std::array<T, Order> coefficients;
    T constant;
    std::vector<T> initial_values;
    std::default_random_engine generator;
};

#endif
