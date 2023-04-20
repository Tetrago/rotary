cc_test(
    name = "vulkan",
    srcs = ["vulkan.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//libs/plat",
        "//libs/vtk"
    ],
    linkopts = select({
        "@bazel_tools//src/conditions:windows": [
            "-DEFAULTLIB:user32.lib",
            "-DEFAULTLIB:gdi32.lib",
            "-DEFAULTLIB:shell32.lib"
        ],
        "//conditions:default": []
    }),
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"]
    })
)
