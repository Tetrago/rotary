cc_library(
    name = "rotary",
    srcs = [
        "src/graphics/graphics.cpp",
        "src/vulkan/graphics.cpp"
    ],
    hdrs = [
        "include/rotary/rotary.hpp",
        "include/rotary/core/memory.hpp",
        "include/rotary/graphics/graphics.hpp",
        "src/vulkan/graphics.hpp"
    ],
    includes = ["include"],
    deps = [
        "//libs/diag",
        "//libs/plat",
        "//libs/vtk"
    ],
	copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"]
    }),
    visibility = ["//visibility:public"]
)
