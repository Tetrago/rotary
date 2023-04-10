load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(name = "srcs", srcs = glob(["**"]))

cmake(
    name = "glfw3",
    lib_source = ":srcs"
)

alias(
    name = "glfw",
    actual = "glfw3",
    visibility = ["//visibility:public"]
)