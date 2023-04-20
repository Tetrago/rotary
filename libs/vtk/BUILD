cc_library(
    name = "vtk",
    srcs = [
        "src/command.cpp",
        "src/framebuffer.cpp",
        "src/instance.cpp",
        "src/logical_device.cpp",
        "src/physical_device.cpp",
        "src/render_pass.cpp",
        "src/swapchain.cpp"
    ],
    hdrs = [
        "include/vtk/assert.hpp",
        "include/vtk/command.hpp",
        'include/vtk/extras.hpp',
        "include/vtk/framebuffer.hpp",
        "include/vtk/instance.hpp",
        "include/vtk/logical_device.hpp",
        "include/vtk/memory.hpp",
        "include/vtk/physical_device.hpp",
        "include/vtk/swapchain.hpp",
        "include/vtk/render_pass.hpp",
        "include/vtk/vtk.hpp"
    ],
    includes = ["include"],
    deps = [
        "@Vulkan-Headers//:Vulkan-Headers",
        "@Vulkan-Loader//:Vulkan-Loader"
    ],
	copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"]
    }),
    visibility = ["//visibility:public"]
)
