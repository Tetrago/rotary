load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("//third_party:tools.bzl", "auto_github")

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

# Dependencies

auto_github(remote = "KhronosGroup/Vulkan-Headers", commit = "63af1cf1ee906ba4dcd5a324bdd0201d4f4bfd12")
auto_github(remote = "KhronosGroup/Vulkan-Loader", commit = "8c8619df2dfb693d8ce4d5b1904a4b7694d45fbd")
auto_github(remote = "glfw/glfw", commit = "3fa2360720eeba1964df3c0ecf4b5df8648a8e52")
auto_github(remote = "gabime/spdlog", commit = "0ca574ae168820da0268b3ec7607ca7b33024d05")
