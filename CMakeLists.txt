cmake_minimum_required(VERSION 3.16)

project("ar_grapher")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_MAIN "Build main programs" ON)

find_package(GTest REQUIRED)
include(GoogleTest)

enable_testing()
add_executable(unit_tests tests/unit/test_compute_stats.cpp tests/unit/test_get_run_stats.cpp tests/unit/test_armodel_generate.cpp src/ar_model.cpp)
target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)
gtest_discover_tests(unit_tests)
target_link_libraries(unit_tests GTest::gtest_main)
target_compile_options(unit_tests PRIVATE -Wno-stringop-overflow)

add_executable(integration_tests tests/integration/test_ar_properties.cpp src/ar_model.cpp)
target_include_directories(integration_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)
gtest_discover_tests(integration_tests)
target_link_libraries(integration_tests GTest::gtest_main)
target_compile_options(integration_tests PRIVATE -Wno-stringop-overflow)

if(BUILD_MAIN)
    add_executable(ar_graphed src/main.cpp src/ar_model.cpp src/renderer.cpp)

    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
    target_link_libraries(ar_graphed sfml-graphics sfml-window sfml-system)
endif()