# Copyright (c) 2024, Benjamin Shropshire,
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

"""
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
"""

def _gen_dummy_impl(ctx):
    if not ctx.outputs.json and not ctx.outputs.pb:
        fail("json or pb is requiered")

    outs = []
    ins = []

    args = ctx.actions.args()

    # args.add("--stderrthreshold=0")
    args.add("--message_name=%s" % ctx.attr.message_name)

    if ctx.outputs.json:
        json_file = ctx.actions.declare_file(ctx.outputs.json.basename)
        outs += [json_file]
        args.add("--json=%s" % json_file.path)

    if ctx.outputs.pb:
        pb_file = ctx.actions.declare_file(ctx.outputs.pb.basename)
        outs += [pb_file]
        args.add("--pb=%s" % pb_file.path)

    if ctx.file.counts:
        ins += [ctx.file.counts]
        args.add("--counts=%s" % ctx.file.counts.path)

    if ctx.file.enums:
        ins += [ctx.file.enums]
        args.add("--enums=%s" % ctx.file.enums.path)

    args.add_all([
        f.path
        for f in ctx.attr.proto[ProtoInfo].transitive_descriptor_sets.to_list()
    ])

    ctx.actions.run(
        inputs = depset(
            ins,
            transitive = [ctx.attr.proto[ProtoInfo].transitive_descriptor_sets],
        ),
        outputs = outs,
        executable = ctx.file._tool,
        arguments = [args],
    )
    return

gen_dummy = rule(
    doc = "",
    #
    implementation = _gen_dummy_impl,
    attrs = {
        "proto": attr.label(
            doc = "A proto_library rule to generate from.",
            allow_single_file = True,
            mandatory = True,
            providers = [ProtoInfo],
        ),
        "message_name": attr.string(
            doc = "A proto buff name. Must be fully qualified.",
            mandatory = True,
        ),
        "counts": attr.label(
            doc = "A json file mapping fully qualified repated field name to lengths.",
            allow_single_file = True,
        ),
        "enums": attr.label(
            doc = "A json file holding a map from enum names to values to skip.",
            allow_single_file = True,
        ),
        "json": attr.output(
            doc = "The generated JavaScript file.",
        ),
        "pb": attr.output(
            doc = "The generated protobuf text format file.",
        ),
        "_tool": attr.label(
            doc = "The generater.",
            allow_single_file = True,
            default = ":gen_dummy",
        ),
    },
)
