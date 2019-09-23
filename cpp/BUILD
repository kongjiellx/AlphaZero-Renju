cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    deps = [
        "@org_tensorflow//tensorflow/cc:cc_ops",
        "@org_tensorflow//tensorflow/cc:client_session",
        "@org_tensorflow//tensorflow/core:tensorflow",
        "@org_tensorflow//tensorflow/core:framework",
    ],
)

cc_library(
    name = "board_lib",
    srcs = [
        "board.h",
        "board.cpp"
    ],
    deps = [
        "//conf:configure_cc_proto"
    ],
    visibility = ["//test:__pkg__"]
)