## snappy library update script.
#
# _patch_path
# _working_dir

execute_process (
    COMMAND patch -p1
    INPUT_FILE "${_patch_path}"
    WORKING_DIRECTORY "${_working_dir}")

