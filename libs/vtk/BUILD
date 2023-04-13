cc_library(
    name = "vtk",
    srcs = [
        "src/instance.cpp",
        "src/logical_device.cpp",
        "src/physical_device.cpp",
        "src/swapchain.cpp"
    ],
    hdrs = [
        "include/vtk/vtk.hpp",
        "include/vtk/instance.hpp",
        "include/vtk/logical_device.hpp",
        "include/vtk/memory.hpp",
        "include/vtk/physical_device.hpp",
        "include/vtk/swapchain.hpp"
    ],
	includes = ["include"],
    deps = [
        "@Vulkan-Headers//:Vulkan-Headers",
        "@Vulkan-Loader//:Vulkan-Loader",
        "//libs/diag"
    ],
	copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"]
    }),
    visibility = ["//visibility:public"]
)
