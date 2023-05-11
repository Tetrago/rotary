cmake_minimum_required(VERSION 3.8)
project(plat LANGUAGES CXX)

set(PLAT_HEADERS
	"include/plat/plat.hpp"
	"include/plat/filesystem.hpp"
	"include/plat/window.hpp"
)

set(PLAT_SOURCES
	"src/filesystem.cpp"
	"src/window.cpp"
)

set(PLAT_LIBRARIES
	PRIVATE glfw
)

set(PLAT_DEFINITIONS)

if(ROTARY_VULKAN)
	list(APPEND PLAT_SOURCES "src/vulkan.cpp")
	list(APPEND PLAT_LIBRARIES PUBLIC Vulkan::Headers)
	list(APPEND PLAT_DEFINITIONS PUBLIC PLAT_VULKAN)
endif()

add_library(plat STATIC ${PLAT_HEADERS} ${PLAT_SOURCES})

target_compile_features(plat PUBLIC cxx_std_20)
target_include_directories(plat PUBLIC "include")
target_link_libraries(plat ${PLAT_LIBRARIES})
target_compile_definitions(plat ${PLAT_DEFINITIONS})
