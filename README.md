<!-- Generated with Stardoc: http://skydoc.bazel.build -->

# A Bazel rule for generating a "text proto" populated with dummy data.

## `MODULE.bazel`

```
bazel_dep(
    name = "com_github_bcsgh_dummy_proto",
    version = ...,
)
```

## Usage

```
load("@com_github_bcsgh_dummy_proto//tools:gen_dummy.bzl", "gen_dummy")

proto_library(
    name = "api",
    srcs = ["api.proto"],
)

gen_dummy(
    name = "dummy_api_data",
    proto = ":api",
    message_name = "some.thing.Responce",
    pb = "test_dummy.txt.pb",
)
```

<a id="gen_dummy"></a>

## gen_dummy

<pre>
load("@com_github_bcsgh_dummy_proto//tools:gen_dummy.bzl", "gen_dummy")

gen_dummy(<a href="#gen_dummy-name">name</a>, <a href="#gen_dummy-counts">counts</a>, <a href="#gen_dummy-json">json</a>, <a href="#gen_dummy-message_name">message_name</a>, <a href="#gen_dummy-pb">pb</a>, <a href="#gen_dummy-proto">proto</a>)
</pre>



**ATTRIBUTES**


| Name  | Description | Type | Mandatory | Default |
| :------------- | :------------- | :------------- | :------------- | :------------- |
| <a id="gen_dummy-name"></a>name |  A unique name for this target.   | <a href="https://bazel.build/concepts/labels#target-names">Name</a> | required |  |
| <a id="gen_dummy-counts"></a>counts |  A json file mapping fully qualified repated field name to lengths.   | <a href="https://bazel.build/concepts/labels">Label</a> | optional |  `None`  |
| <a id="gen_dummy-json"></a>json |  The generated JavaScript file.   | <a href="https://bazel.build/concepts/labels">Label</a> | optional |  `None`  |
| <a id="gen_dummy-message_name"></a>message_name |  A proto buff name. Must be fully qualified.   | String | required |  |
| <a id="gen_dummy-pb"></a>pb |  The generated protobuf text format file.   | <a href="https://bazel.build/concepts/labels">Label</a> | optional |  `None`  |
| <a id="gen_dummy-proto"></a>proto |  A proto_library rule to generate from.   | <a href="https://bazel.build/concepts/labels">Label</a> | required |  |


## Setup (for development)
To configure the git hooks, run `./.git_hooks/setup.sh`
