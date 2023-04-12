cc_library(
    name = "plat",
    srcs = [
        "src/vulkan.cpp",
        "src/window.cpp"
    ],
    hdrs = [
        "include/plat/plat.hpp",
        "include/plat/vulkan.hpp",
        "include/plat/window.hpp"
    ],
    includes = ["include"],
    deps = [
        "@glfw//:glfw",
        "@Vulkan-Headers//:Vulkan-Headers"
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"]
    }),
    visibility = ["//visibility:public"]
)
