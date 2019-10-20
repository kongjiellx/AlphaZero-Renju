cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
    ],
    deps = [
        "@org_tensorflow//tensorflow/cc:cc_ops",
        "@org_tensorflow//tensorflow/cc:client_session",
        "@org_tensorflow//tensorflow/core:framework",
        "@org_tensorflow//tensorflow/core:tensorflow",
    ],
)

cc_library(
    name = "board_lib",
    srcs = [
        "board.cpp",
        "board.h",
    ],
    visibility = ["//test:__pkg__"],
    deps = [
        "//conf:configure_cc_proto",
    ],
)

cc_library(
    name = "model_lib",
    srcs = [
        "model.cpp",
        "model.h",
    ],
    deps = [
        ":tf_utils_lib",
        "@org_tensorflow//tensorflow/cc:cc_ops",
        "@org_tensorflow//tensorflow/cc:client_session",
        "@org_tensorflow//tensorflow/cc:grad_ops",
        "@org_tensorflow//tensorflow/cc:gradients",
        "@org_tensorflow//tensorflow/core:tensorflow",
    ],
)

cc_library(
    name = "tf_utils_lib",
    srcs = [
        "tf_utils.cpp",
        "tf_utils.h",
    ],
    deps = [
        "@org_tensorflow//tensorflow/cc:cc_ops",
        "@org_tensorflow//tensorflow/cc:client_session",
        "@org_tensorflow//tensorflow/core:tensorflow",
    ],
)
