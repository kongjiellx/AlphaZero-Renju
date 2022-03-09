load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

########################################################
# tensorflow archive
########################################################
load(":repo.bzl", "tensorflow_http_archive")
# copy from tensorflow serving
tensorflow_http_archive(
    name = "org_tensorflow",
    sha256 = "3e6c98de0842520a65978549be7b1b6061080ecf9fa9f3a87739e19a0447a85c",
    git_commit = "1f8f692143aa9a42c55f8b35d09aeed93bdab66e",
)

# Check bazel version requirement, which is stricter than TensorFlow's.
load(
    "@org_tensorflow//tensorflow:version_check.bzl",
    "check_bazel_version_at_least"
)
check_bazel_version_at_least("5.0.0")

# Initialize TensorFlow's external dependencies.
load("@org_tensorflow//tensorflow:workspace3.bzl", "workspace")
workspace()
load("@org_tensorflow//tensorflow:workspace2.bzl", "workspace")
workspace()
load("@org_tensorflow//tensorflow:workspace1.bzl", "workspace")
workspace()
load("@org_tensorflow//tensorflow:workspace0.bzl", "workspace")
workspace()

########################################################
# grpc proto config
########################################################

http_archive(
    name = "rules_proto_grpc",
    sha256 = "5f0f2fc0199810c65a2de148a52ba0aff14d631d4e8202f41aff6a9d590a471b",
    strip_prefix = "rules_proto_grpc-1.0.2",
    urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/1.0.2.tar.gz"],
)

load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_repos", "rules_proto_grpc_toolchains")

rules_proto_grpc_toolchains()

rules_proto_grpc_repos()

load("@rules_proto_grpc//python:repositories.bzl", rules_proto_grpc_python_repos = "python_repos")

rules_proto_grpc_python_repos()

load("@rules_proto_grpc//cpp:repositories.bzl", rules_proto_grpc_cpp_repos = "cpp_repos")

rules_proto_grpc_cpp_repos()

########################################################
# absl
########################################################
http_archive(
    name = "com_google_absl",
    sha256 = "71d00d15fe6370220b6685552fb66e5814f4dd2e130f3836fc084c894943753f",
    strip_prefix = "abseil-cpp-7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72",
    urls = ["https://github.com/abseil/abseil-cpp/archive/7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72.zip"],  # 2019-03-14
)

########################################################
# logging
########################################################
http_archive(
    name = "spdlog",
    sha256 = "130bd593c33e2e2abba095b551db6a05f5e4a5a19c03ab31256c38fa218aa0a6",
    strip_prefix = "spdlog-1.9.2",
    urls = ["https://github.com/gabime/spdlog/archive/refs/tags/v1.9.2.zip"],
    build_file = "//cpp/third_party:spdlog.BUILD"
)
