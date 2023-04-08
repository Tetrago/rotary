cc_library(
    name = "vtk",
    srcs = [
        "src/instance.cpp",
        "src/physical_device.cpp"
    ],
    hdrs = [
        "include/vtk/instance.hpp",
        "include/vtk/physical_device.hpp"
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
