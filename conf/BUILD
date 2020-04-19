package(default_visibility = ["//visibility:public"])

load("@rules_proto_grpc//python:defs.bzl", "python_proto_library")
load("@rules_proto_grpc//cpp:defs.bzl", "cpp_proto_library")

proto_library(
    name = "conf_proto",
    srcs = ["conf.proto"]
)

cpp_proto_library(
    name = "conf_cc_proto",
    deps = [":conf_proto"],
)

python_proto_library(
    name = "conf_py_proto",
    deps = [":conf_proto"]
)

exports_files(["conf.pbtxt"])

filegroup(
    name = "graph_def",
    srcs = ["graph.pbtxt"],
)

