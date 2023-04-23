cmake_minimum_required(VERSION 3.8)
project(shcc LANGUAGES CXX)

set(SHCC_HEADERS
	"include/shcc/shcc.hpp"
)

set(SHCC_SOURCES
	"src/shcc.cpp"
)

add_library(shcc STATIC ${SHCC_HEADERS} ${SHCC_SOURCES})

target_compile_features(shcc PUBLIC cxx_std_20)
target_include_directories(shcc PUBLIC "include")
target_link_libraries(shcc PRIVATE spirv-cross-core spirv-cross-glsl spirv-cross-reflect)
