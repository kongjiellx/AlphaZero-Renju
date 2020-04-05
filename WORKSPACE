load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load(":repo.bzl", "tensorflow_http_archive")

tensorflow_http_archive(
    name = "org_tensorflow",
    sha256 = "1f4b09e6bff7f847bb1034699076055e50e87534d76008af8295ed71195b2b36",
    git_commit = "e5bf8de410005de06a7ff5393fafdf832ef1d4ad", # v2.1.0 release
)

# START: Upstream TensorFlow dependencies
# TensorFlow build depends on these dependencies.
# Needs to be in-sync with TensorFlow sources.
http_archive(
    name = "io_bazel_rules_closure",
    sha256 = "5b00383d08dd71f28503736db0500b6fb4dda47489ff5fc6bed42557c07c6ba9",
    strip_prefix = "rules_closure-308b05b2419edb5c8ee0471b67a40403df940149",
    urls = [
        "https://storage.googleapis.com/mirror.tensorflow.org/github.com/bazelbuild/rules_closure/archive/308b05b2419edb5c8ee0471b67a40403df940149.tar.gz",
        "https://github.com/bazelbuild/rules_closure/archive/308b05b2419edb5c8ee0471b67a40403df940149.tar.gz",  # 2019-06-13
    ],
)
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "bazel_skylib",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
    ],
    sha256 = "97e70364e9249702246c0e9444bccdc4b847bed1eb03c5a3ece4f83dfe6abc44",
)
load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")
bazel_skylib_workspace()
# END: Upstream TensorFlow dependencies

load("@org_tensorflow//tensorflow:workspace.bzl", "tf_workspace")

tf_workspace(
 path_prefix = "",
 tf_repo_name = "org_tensorflow",
)

########################################################
#    grpc proto config
########################################################
http_archive(
    name = "build_stack_rules_proto",
    urls = ["https://github.com/stackb/rules_proto/archive/b93b544f851fdcd3fc5c3d47aee3b7ca158a8841.tar.gz"],
    sha256 = "c62f0b442e82a6152fcd5b1c0b7c4028233a9e314078952b6b04253421d56d61",
    strip_prefix = "rules_proto-b93b544f851fdcd3fc5c3d47aee3b7ca158a8841",
)
# cpp grpc proto
load("@build_stack_rules_proto//cpp:deps.bzl", "cpp_proto_library")
cpp_proto_library()
# python grpc proto
load("@build_stack_rules_proto//python:deps.bzl", "python_proto_library")
python_proto_library()
load("@io_bazel_rules_python//python:pip.bzl", "pip_import", "pip_repositories")
pip_repositories()
pip_import(
    name = "protobuf_py_deps",
    requirements = "@build_stack_rules_proto//python/requirements:protobuf.txt",
)
load("@protobuf_py_deps//:requirements.bzl", protobuf_pip_install = "pip_install")
protobuf_pip_install()

