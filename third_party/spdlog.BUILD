load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

package(default_visibility = ["//visibility:public"])

filegroup(name = "srcs", srcs = glob(["**"]))

cmake(
	name = "spdlog",
	lib_source = ":srcs"
)