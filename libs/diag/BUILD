cc_library(
    name = "diag",
    srcs = [
        "src/logger.cpp"
    ],
    hdrs = [
        "include/diag/diag.hpp",
        "include/diag/logger.hpp"
    ],
    includes = ["include"],
    deps = [
        "@spdlog//:spdlog"
    ],
	copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"]
    }),
    visibility = ["//visibility:public"]
)
