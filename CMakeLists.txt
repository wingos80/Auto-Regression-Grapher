cmake_minimum_required(VERSION 3.16)

project("ar_grapher")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_MAIN "Build main programs" ON)

find_package(GTest REQUIRED)
include(GoogleTest)

enable_testing()
add_executable(tests tests/test_compute_stats.cpp tests/test_get_run_stats.cpp tests/test_armodel_generate.cpp src/ar_model.cpp)
gtest_discover_tests(tests)
target_link_libraries(tests GTest::gtest_main)
target_compile_options(tests PRIVATE -Wno-stringop-overflow)

if(BUILD_MAIN)
    add_executable(ar_graphed src/main.cpp src/ar_model.cpp src/renderer.cpp)

    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
    target_link_libraries(ar_graphed sfml-graphics sfml-window sfml-system)
endif()