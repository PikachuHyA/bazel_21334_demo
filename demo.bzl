def _impl(ctx):
    ctx.actions.run(
        inputs = [],
        outputs = [ctx.outputs.output_file],
        arguments = [ctx.outputs.output_file.path],
        executable = ctx.executable._tool,
    )
    return

demo_rule = rule(
    implementation = _impl,
    attrs = {
        "_tool": attr.label(
            executable = True,
            cfg = "exec",
            allow_files = True,
            default = Label("//:demo"),
        ),
        "output_file": attr.output(mandatory = True),
    },
)
