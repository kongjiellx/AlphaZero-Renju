package(default_visibility = ["//visibility:public"])

load("@build_stack_rules_proto//cpp:cpp_proto_library.bzl", "cpp_proto_library")

proto_library(
    name = "configure_proto",
    srcs = ["configure.proto"]
)

cpp_proto_library(
    name = "configure_cc_proto",
    deps = [":configure_proto"],
)

cc_binary(
    name = "test_conf",
    srcs = [
        "test_conf.cpp"
        ],
    deps = [
        ":configure_cc_proto"
    ],
    data = [
        "configure.txt"
    ]
)

