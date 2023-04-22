cmake_minimum_required(VERSION 3.8)
project(tests LANGUAGES CXX)

add_executable(TestVulkan "vulkan.cpp")
target_link_libraries(TestVulkan PRIVATE GTest::gtest GTest::gtest_main plat vtk)
add_test(NAME TestVulkan COMMAND TestVulkan)
