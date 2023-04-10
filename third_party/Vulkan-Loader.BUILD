load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(name = "srcs", srcs = glob(["**"]))

cmake(
    name = "vulkan-1",
    generate_args = ["-DUPDATE_DEPS=ON"],
    lib_source = ":srcs"
)

alias(
    name = "Vulkan-Loader",
    actual = "vulkan-1",
    visibility = ["//visibility:public"]
)