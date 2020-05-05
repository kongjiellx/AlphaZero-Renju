load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
########################################################
# tensorflow archive
########################################################
load(":repo.bzl", "tensorflow_http_archive")
tensorflow_http_archive(
    name = "org_tensorflow",
    sha256 = "1f4b09e6bff7f847bb1034699076055e50e87534d76008af8295ed71195b2b36",
    git_commit = "e5bf8de410005de06a7ff5393fafdf832ef1d4ad", # v2.1.0 release
)
########################################################
# Upstream TensorFlow dependencies
# TensorFlow build depends on these dependencies.
# Needs to be in-sync with TensorFlow sources.
########################################################
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
########################################################
# load tensorflow workspace
########################################################
load("@org_tensorflow//tensorflow:workspace.bzl", "tf_workspace")
tf_workspace(
 path_prefix = "",
 tf_repo_name = "org_tensorflow",
)
#########################################################
## glog
#########################################################
http_archive(
    name = "com_github_gflags_gflags",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
    strip_prefix = "gflags-2.2.2",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
)
http_archive(
    name = "com_github_google_glog",
    urls = ["https://github.com/google/glog/archive/v0.4.0.tar.gz"],
    strip_prefix = "glog-0.4.0",
    sha256 = "f28359aeba12f30d73d9e4711ef356dc842886968112162bc73002645139c39c",
)
########################################################
# grpc proto config
########################################################
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "rules_proto_grpc",
    urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/1.0.2.tar.gz"],
    sha256 = "5f0f2fc0199810c65a2de148a52ba0aff14d631d4e8202f41aff6a9d590a471b",
    strip_prefix = "rules_proto_grpc-1.0.2",
)
load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_toolchains", "rules_proto_grpc_repos")
rules_proto_grpc_toolchains()
rules_proto_grpc_repos()
load("@rules_proto_grpc//python:repositories.bzl", rules_proto_grpc_python_repos="python_repos")
rules_proto_grpc_python_repos()
load("@rules_proto_grpc//cpp:repositories.bzl", rules_proto_grpc_cpp_repos="cpp_repos")
rules_proto_grpc_cpp_repos()
########################################################
# load pip dependencies
########################################################
#git_repository(
#    name = "rules_python",
#    remote = "https://github.com/bazelbuild/rules_python.git",
#    commit = "748aa53d7701e71101dfd15d800e100f6ff8e5d1",
#)
#load("@rules_python//python:repositories.bzl", "py_repositories")
#py_repositories()
#load("@rules_python//python:pip.bzl", "pip_repositories")
#pip_repositories()
#load("@rules_python//python:pip.bzl", "pip_import")
#pip_import(   # or pip3_import
#   name = "pip_deps",
#   requirements = "//:requirements.txt",
#)
#load("@pip_deps//:requirements.bzl", "pip_install")
#pip_install()
http_archive(
    name = "com_github_ali5h_rules_pip",
    strip_prefix = "rules_pip-2.1.0",
    sha256 = "c8c11f219642ab94cb3f4a5ff25aadda6fb6dcb0c77329021e843a7e7ba294d1",
    urls = ["https://github.com/ali5h/rules_pip/archive/2.1.0.tar.gz"],
)
load("@com_github_ali5h_rules_pip//:defs.bzl", "pip_import")
pip_import(
   name = "pip_deps",
   requirements = "//:requirements.txt",
)
load("@pip_deps//:requirements.bzl", "pip_install")
pip_install()
########################################################
# absl
########################################################
http_archive(
  name = "com_google_absl",
  urls = ["https://github.com/abseil/abseil-cpp/archive/7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72.zip"],  # 2019-03-14
  strip_prefix = "abseil-cpp-7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72",
  sha256 = "71d00d15fe6370220b6685552fb66e5814f4dd2e130f3836fc084c894943753f",
)