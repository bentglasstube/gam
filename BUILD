package(default_visibility = ["//visibility:public"])

cc_library(
    name = "audio",
    srcs = ["audio.cc"],
    hdrs = ["audio.h"],
)

cc_library(
    name = "backdrop",
    srcs = ["backdrop.cc"],
    hdrs = ["backdrop.h"],
    deps = [":graphics"],
)

cc_library(
    name = "game",
    srcs = ["game.cc"],
    hdrs = ["game.h"],
    deps = [
        ":audio",
        ":graphics",
        ":input",
        ":screen",
    ],
)

cc_library(
    name = "graphics",
    srcs = ["graphics.cc"],
    hdrs = ["graphics.h"],
)

cc_library(
    name = "input",
    srcs = ["input.cc"],
    hdrs = ["input.h"],
)

cc_library(
    name = "particle",
    srcs = ["particle.cc"],
    hdrs = ["particle.h"],
    deps = [":graphics"],
)

cc_library(
    name = "screen",
    srcs = ["screen.cc"],
    hdrs = ["screen.h"],
    deps = [
        ":audio",
        ":graphics",
        ":input",
    ],
)

cc_library(
    name = "sprite",
    srcs = ["sprite.cc"],
    hdrs = ["sprite.h"],
    deps = [":graphics"],
)

cc_library(
    name = "spritemap",
    srcs = ["spritemap.cc"],
    hdrs = ["spritemap.h"],
    deps = [":graphics"],
)

cc_library(
    name = "text",
    srcs = ["text.cc"],
    hdrs = ["text.h"],
    deps = [":graphics"],
)
