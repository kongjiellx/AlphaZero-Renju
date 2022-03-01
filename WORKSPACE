load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

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
# http_archive(
#     name = "com_github_ali5h_rules_pip",
#     sha256 = "c8c11f219642ab94cb3f4a5ff25aadda6fb6dcb0c77329021e843a7e7ba294d1",
#     strip_prefix = "rules_pip-2.1.0",
#     urls = ["https://github.com/ali5h/rules_pip/archive/2.1.0.tar.gz"],
# )

# load("@com_github_ali5h_rules_pip//:defs.bzl", "pip_import")

# pip_import(
#     name = "pip_deps",
#     requirements = "//:requirements.txt",
# )

# load("@pip_deps//:requirements.bzl", "pip_install")

# pip_install()

########################################################
# absl
########################################################
http_archive(
    name = "com_google_absl",
    sha256 = "71d00d15fe6370220b6685552fb66e5814f4dd2e130f3836fc084c894943753f",
    strip_prefix = "abseil-cpp-7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72",
    urls = ["https://github.com/abseil/abseil-cpp/archive/7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72.zip"],  # 2019-03-14
)
