from os.path import join, isfile

Import("env")

FRAMEWORK_DIR = env.PioPlatform().get_package_dir("framework-arduinogd32")
patchflag_path = join(FRAMEWORK_DIR, ".patching-done")

# patch file only if we didn't do it before
if not isfile(join(FRAMEWORK_DIR, ".patching-done")):
    original_file = join(FRAMEWORK_DIR, "system", "GD32F3x0_firmware", "CMSIS", "GD", "GD32F3x0", "Source", "system_gd32f3x0.c")
    patched_file = join("YardForce", "util", "system_gd32f3x0_modified.diff")

    assert isfile(original_file) and isfile(patched_file)

    env.Execute("patch %s %s" % (original_file, patched_file))
    # env.Execute("touch " + patchflag_path)


    def _touch(path):
        with open(path, "w") as fp:
            fp.write("")

    env.Execute(lambda *args, **kwargs: _touch(patchflag_path))