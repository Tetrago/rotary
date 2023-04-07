cc_binary(
    name = "player",
    srcs = ["src/main.cpp"],
    deps = [
        "//libs/vtk:vtk",
		"//libs/plat:plat"
    ],
	linkopts = select({
        "@bazel_tools//src/conditions:windows": [
            "-DEFAULTLIB:user32.lib",
            "-DEFAULTLIB:gdi32.lib",
            "-DEFAULTLIB:shell32.lib"
        ],
        "//conditions:default": [""]
    }),
	copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++20"],
        "//conditions:default": ["-std=c++20"]
    })
)