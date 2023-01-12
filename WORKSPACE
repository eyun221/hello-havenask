workspace(name = "hello_havenask")

load(
    "@bazel_tools//tools/build_defs/repo:git.bzl",
    "git_repository",
    "new_git_repository",
)



new_git_repository(
    name = "havenask",
    remote = "https://github.com/alibaba/havenask.git",
    tag = "v0.2.2",
    build_file = "@hello_havenask//:ha.BUILD",
)
