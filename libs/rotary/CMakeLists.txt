cmake_minimum_required(VERSION 3.8)
project(rotary LANGUAGES CXX)

set(ROTARY_HEADERS
	"include/rotary/rotary.hpp"
	"include/rotary/core/log.hpp"
	"include/rotary/core/memory.hpp"
	"include/rotary/graphics/graphics.hpp"
	"include/rotary/graphics/shader.hpp"
)

set(ROTARY_SOURCES
	"src/core/log.cpp"
	"src/graphics/graphics.cpp"
)

set(ROTARY_DEFINITIONS)

set(ROTARY_LIBRARIES
	PUBLIC spdlog::spdlog
	PRIVATE plat
)

if(ROTARY_VULKAN)
	list(APPEND ROTARY_HEADERS
		"src/vulkan/graphics.hpp"
		"src/vulkan/shader.hpp"
	)

	list(APPEND ROTARY_SOURCES
		"src/vulkan/graphics.cpp"
		"src/vulkan/shader.cpp"
	)

	list(APPEND ROTARY_DEFINITIONS PRIVATE ROTARY_VULKAN)

	list(APPEND ROTARY_LIBRARIES
		PRIVATE vtk
	)
endif()

add_library(rotary STATIC ${ROTARY_HEADERS} ${ROTARY_SOURCES})

target_compile_features(rotary PUBLIC cxx_std_20)
target_compile_definitions(rotary ${ROTARY_DEFINITIONS})
target_include_directories(rotary PUBLIC "include" PRIVATE "src")
target_link_libraries(rotary ${ROTARY_LIBRARIES})
