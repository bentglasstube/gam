package(default_visibility = ["//visibility:public"])

cc_library(
    name = "audio",
    srcs = ["audio.cc"],
    hdrs = ["audio.h"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_mixer",
    ],
)

cc_library(
    name = "backdrop",
    srcs = ["backdrop.cc"],
    hdrs = ["backdrop.h"],
    deps = [":graphics"],
)

cc_library(
    name = "box",
    srcs = ["box.cc"],
    hdrs = ["box.h"],
    deps = [":graphics"],
)

cc_library(
    name = "client",
    srcs = ["client.cc"],
    hdrs = ["client.h"],
    deps = [":net"],
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
    linkopts = ["-lSDL2"],
)

cc_library(
    name = "graphics",
    srcs = ["graphics.cc"],
    hdrs = ["graphics.h"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
    ],
)

genrule(
    name = "generate_controllerdb",
    cmd = "./$(location :controllerdb_generator) $(location @gamecontrollerdb//:database) > $@",
    outs = ["controllerdb.cc"],
    tools = [":controllerdb_generator"],
    srcs = ["@gamecontrollerdb//:database"],
)

cc_binary(
    name = "controllerdb_generator",
    srcs = ["generate_controllerdb.cc"],
    data = ["@gamecontrollerdb//:database"],
)

cc_library(
    name = "controllerdb",
    srcs = [":generate_controllerdb"],
    hdrs = ["controllerdb.h"],
    linkopts = ["-lSDL2"],
)

cc_library(
    name = "input",
    srcs = ["input.cc"],
    hdrs = ["input.h"],
    deps = [
        ":controllerdb",
        ":util",
    ],
    linkopts = ["-lSDL2"],
)

cc_library(
    name = "menu",
    srcs = ["menu.cc"],
    hdrs = ["menu.h"],
    deps = [
        ":graphics",
        ":text",
    ],
)

cc_library(
    name = "net",
    srcs = ["net.cc"],
    hdrs = ["net.h"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_net",
    ],
)

cc_library(
    name = "parallax_backdrop",
    srcs = ["parallax_backdrop.cc"],
    hdrs = ["parallax_backdrop.h"],
    deps = [":graphics"],
)

cc_library(
    name = "particle",
    srcs = ["particle.cc"],
    hdrs = ["particle.h"],
    deps = [":graphics"],
)

cc_library(
    name = "rect",
    srcs = ["rect.cc"],
    hdrs = ["rect.h"],
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
    name = "server",
    srcs = ["server.cc"],
    hdrs = ["server.h"],
    deps = [":net"],
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

cc_library(
    name = "util",
    srcs = ["util.cc"],
    hdrs = ["util.h"],
)
