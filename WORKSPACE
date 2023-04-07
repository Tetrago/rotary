load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Compile Commands

http_archive(
    name = "hedron_compile_commands",
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/800b9cd260ce3878e94abb7d583a7c0865f7d967.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-800b9cd260ce3878e94abb7d583a7c0865f7d967"
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()

# Forgeign CC

http_archive(
    name = "rules_foreign_cc",
    strip_prefix = "rules_foreign_cc-d33d862abb4ce3ba178547ef58c9fcb24cec38ca",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/d33d862abb4ce3ba178547ef58c9fcb24cec38ca.tar.gz"
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

# Vulkan

http_archive(
    name = "Vulkan-Headers",
    build_file = "//third_party:Vulkan-Headers.BUILD",
    strip_prefix = "Vulkan-Headers-63af1cf1ee906ba4dcd5a324bdd0201d4f4bfd12",
    url = "https://github.com/KhronosGroup/Vulkan-Headers/archive/63af1cf1ee906ba4dcd5a324bdd0201d4f4bfd12.tar.gz"
)

http_archive(
    name = "Vulkan-Loader",
    build_file = "//third_party:Vulkan-Loader.BUILD",
    strip_prefix = "Vulkan-Loader-8c8619df2dfb693d8ce4d5b1904a4b7694d45fbd",
    url = "https://github.com/KhronosGroup/Vulkan-Loader/archive/8c8619df2dfb693d8ce4d5b1904a4b7694d45fbd.tar.gz"
)

# Glfw

http_archive(
    name = "glfw",
	build_file = "//third_party:glfw.BUILD",
    strip_prefix = "glfw-3fa2360720eeba1964df3c0ecf4b5df8648a8e52",
	url = "https://github.com/glfw/glfw/archive/3fa2360720eeba1964df3c0ecf4b5df8648a8e52.tar.gz"
)