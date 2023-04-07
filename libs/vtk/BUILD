cc_library(
    name = "vtk",
    srcs = ["src/instance.cpp"],
    hdrs = ["include/vtk/instance.hpp"],
	includes = ["include"],
    deps = [
        "@rules_vulkan//vulkan:vulkan_cc_library"
    ],
	copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"]
    }),
    visibility = ["//visibility:public"]
)
