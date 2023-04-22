cmake_minimum_required(VERSION 3.8)
project(rotary_workspace)

find_package(glfw3)
find_package(GTest)
find_package(spdlog)
find_package(Vulkan)

option(ROTARY_VULKAN "" ON)

add_subdirectory("libs/plat")
add_subdirectory("libs/vtk")
add_subdirectory("libs/rotary")

add_subdirectory("extras/player")

add_subdirectory("tests")
