cc_library(
    name = "plat",
    srcs = [
        "src/extras.cpp",
        "src/window.cpp"
    ],
    hdrs = [
        "include/plat/plat.hpp",
        "include/plat/extras.hpp",
        "include/plat/window.hpp"
    ],
    includes = ["include"],
    deps = [
        "@glfw//:glfw"
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"]
    }),
    visibility = ["//visibility:public"]
)
