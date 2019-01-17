## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()
tbag_modules__apply_shared_library ()

tbag_modules__append_definitions (
        C2RTSP_MAJOR_VERSION=0
        C2RTSP_MINOR_VERSION=0
        C2RTSP_EXPORT_API)

## Thread
tbag_modules__apply_thread ()

## Tbag
tbag_modules__check_tbag ()
tbag_modules__apply_tbag ()

## Live555
tbag_modules__append_include_dirs (${Live555_INCLUDE_DIRS})
tbag_modules__append_ldflags (${Live555_LIBRARIES})

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()
tbag_modules__update_default_version ()
tbag_modules__install_target ()
