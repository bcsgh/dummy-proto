####################################
cc_proto_library(
    name = "descriptor_proto_cc",
    deps = ["@protobuf//:descriptor_proto"],
)

cc_binary(
    name = "gen_dummy",
    srcs = ["gen_dummy.cc"],
    deps = [
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/log:flags",
        "@abseil-cpp//absl/log:initialize",
        "@abseil-cpp//absl/log:log",
        "@jsoncpp//:jsoncpp",
        "@protobuf//:protobuf",
        ":descriptor_proto_cc",
    ],
    visibility = ["//visibility:public"],
)
