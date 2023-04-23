cmake_minimum_required(VERSION 3.8)
project(rotary_workspace)

find_package(argparse)
find_package(glfw3)
find_package(glslang)
find_package(GTest)
find_package(spdlog)
find_package(spirv_cross_core)
find_package(spirv_cross_glsl)
find_package(spirv_cross_reflect)
find_package(Vulkan)
find_package(unofficial-vulkan-memory-allocator)

option(ROTARY_VULKAN "" ON)

add_subdirectory("libs/plat")
add_subdirectory("libs/shcc")
add_subdirectory("libs/vtk")
add_subdirectory("libs/rotary")

add_subdirectory("extras/player")
add_subdirectory("extras/shaderx")

add_subdirectory("tests")
