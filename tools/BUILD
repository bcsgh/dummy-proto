####################################
cc_proto_library(
    name = "descriptor_proto_cc",
    deps = ["@protobuf//:descriptor_proto"],
)

cc_binary(
    name = "gen_dummy",
    srcs = ["gen_dummy.cc"],
    deps = [
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log:check",
        "@com_google_absl//absl/log:flags",
        "@com_google_absl//absl/log:initialize",
        "@com_google_absl//absl/log:log",
        "@jsoncpp//:jsoncpp",
        "@protobuf//:protobuf",
        ":descriptor_proto_cc",
    ],
    visibility = ["//visibility:public"],
)
