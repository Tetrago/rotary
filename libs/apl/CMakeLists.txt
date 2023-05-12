cmake_minimum_required(VERSION 3.8)
project(apl LANGUAGES CXX)

set(APL_HEADERS
	"include/apl/apl.hpp"
	"include/apl/generator.hpp"
	"include/apl/manifest.hpp"
)

set(APL_SOURCES
	"src/generator.cpp"
	"src/resource.cpp"
	"src/manifest.cpp"
)

set(APL_LIBRARIES
	PUBLIC yaml-cpp
	PRIVATE shcc plat-filesystem
)

add_library(apl STATIC ${APL_HEADERS} ${APL_SOURCES})

target_compile_features(apl PUBLIC cxx_std_20)
target_include_directories(apl PUBLIC "include")
target_link_libraries(apl ${APL_LIBRARIES})
