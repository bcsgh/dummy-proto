load("@bazel_skylib//rules:diff_test.bzl", "diff_test")
load("//tools:gen_dummy.bzl", "gen_dummy")

####################################

proto_library(
    name = "api",
    srcs = ["test.proto"],
    visibility = ["//visibility:public"],
)

####################################

gen_dummy(
    name = "foo",
    message_name = "bcsgh.test.ExampleResponce",
    pb = "test_dummy.txt.pb",
    proto = ":api",
)

diff_test(
    name = "gen_test",
    file1 = ":test_dummy.txt.pb",
    file2 = ":testdata/test_dummy.gold.pb",
)
