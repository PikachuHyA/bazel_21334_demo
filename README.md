# Bazel 21334 Reproducer

This repository serves as a reproducer for the Bazel issue [collect2: fatal error: cannot find 'ld' with build](https://github.com/bazelbuild/bazel/issues/21334).

see my [blog](https://pikachuhya.github.io/posts/bazel-cannot-find-ld/) and [CI logs](https://github.com/PikachuHyA/bazel_21334_demo/actions/runs/13432086115/job/37526003234) for details.

## Summary

The issue at hand stems from the linker `ld.lld`, which is specified by the `-fuse-ld=lld` option, not being present in the PATH during the binary linking process. The error message "cannot find 'ld'" can be misleading; it does not refer to `/usr/bin/ld`, but rather indicates a failure to locate `ld.lld`. The resolution involves adding the directory containing `ld.lld` to your **PATH**, either by overriding the PATH variable directly or by using `--host_action_env=PATH=xxx`.

**Note:** The **PATH** variable will not work if the `--incompatible_strict_action_env` option is enabled.


## Q&A

**Q: Why is `-fuse-ld=lld` used when compiling with GCC?**  
**A:** Bazel automatically detects the available linker during the initialization of `local_config_cc`. The relevant code can be found [here](https://github.com/bazelbuild/rules_cc/blob/main/cc/private/toolchain/unix_cc_configure.bzl#L178-L190).

Bazel first checks whether `lld` is available by executing `gcc xxx.cc -Wl,--start-lib -Wl,--end-lib -fuse-ld=lld -v`. If `lld` is not found, it checks for `ld.gold` with the command `gcc xxx.cc -Wl,--start-lib -Wl,--end-lib -fuse-ld=gold -v`. You can review the source for this process [here](https://github.com/bazelbuild/rules_cc/blob/main/cc/private/toolchain/unix_cc_configure.bzl#L476-L493).

Therefore, if `ld.lld` is present in your PATH, `local_config_cc` will register that `lld` is being used.

**Q: Why does a binary linking error occur even when `lld` is specified in the PATH using `--action_env=PATH=xxx`?**  
**A:** The tool we are using, such as `protoc`, is built with the following configuration:

```python
"_tool": attr.label(
    executable = True,
    cfg = "exec",
    allow_files = True,
    mandatory = True,
),
```

The `cfg = "exec"` setting indicates that the tool is built for the execution platform. For further details, please refer to the documentation on [attr.label](https://bazel.build/rules/lib/toplevel/attr#label).

In Bazel, platforms can be categorized into three roles: Host, Execution, and Target. Generally, we build binaries or libraries for the Target platform, for instance, when executing `bazel build :demo`. However, the PATH specified with `--action_env=PATH=xxx` applies to the Target platform. If we are building for the Host platform, this PATH will not be effective and will inherit from the shell environment, defaulting to `/bin:/usr/bin:/usr/local/bin` if the `--incompatible_strict_action_env` option is enabled.
