cc_binary(
    name = "example",
    data = ["//content"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
    ],
    srcs = ["main.cc"],
    deps = [
        "//:box",
        "//:game",
        "//:screen",
        "//:sprite",
        "//:text",
    ],
)
