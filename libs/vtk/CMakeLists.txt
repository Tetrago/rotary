cmake_minimum_required(VERSION 3.8)
project(vtk LANGUAGES CXX)

set(VTK_HEADERS
	"include/vtk/assert.hpp"
	"include/vtk/command.hpp"
	"include/vtk/extras.hpp"
	"include/vtk/framebuffer.hpp"
	"include/vtk/instance.hpp"
	"include/vtk/logical_device.hpp"
	"include/vtk/memory.hpp"
	"include/vtk/physical_device.hpp"
	"include/vtk/swapchain.hpp"
	"include/vtk/render_pass.hpp"
	"include/vtk/vtk.hpp"
)

set(VTK_SOURCES
	"src/command.cpp"
	"src/framebuffer.cpp"
	"src/instance.cpp"
	"src/logical_device.cpp"
	"src/physical_device.cpp"
	"src/render_pass.cpp"
	"src/swapchain.cpp"
)

add_library(vtk STATIC ${VTK_HEADERS} ${VTK_SOURCES})

target_compile_features(vtk PUBLIC cxx_std_20)
target_include_directories(vtk PUBLIC "include")
target_link_libraries(vtk
	PUBLIC Vulkan::Headers
	PRIVATE Vulkan::Vulkan
)
