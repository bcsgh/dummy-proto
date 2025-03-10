load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@stardoc//stardoc:stardoc.bzl", "stardoc")

exports_files(["gen_dummy.bzl"])

stardoc(
    name = "docs",
    input = "gen_dummy.bzl",
    out = "check.md",
)

####################################
cc_proto_library(
    name = "descriptor_proto_cc",
    deps = ["@protobuf//:descriptor_proto"],
)

cc_binary(
    name = "gen_dummy",
    srcs = ["gen_dummy.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":descriptor_proto_cc",
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/log:flags",
        "@abseil-cpp//absl/log:initialize",
        "@abseil-cpp//absl/log:log",
        "@jsoncpp",
        "@protobuf",
    ],
)
