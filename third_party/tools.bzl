load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def auto_github(remote, commit):
    repo = remote.partition("/")[2]
    http_archive(
        name = repo,
        build_file = "//third_party:%s.BUILD" % (repo),
        strip_prefix = "%s-%s" % (repo, commit),
        url = "https://github.com/%s/archive/%s.tar.gz" % (remote, commit)
    )