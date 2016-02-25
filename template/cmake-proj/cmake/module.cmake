# CMake module utility script.

#! Find extension file & find package.
#
# \param _dir        [in] find directory.
# \param _extension  [in] file extension.
# \param _module     [in] assign module.
function (find_extension_and_assign_module _dir _extension _module)
    #message ("_dir: ${_dir}")
    #message ("_extension: ${_extension}")
    #message ("_module: ${_module}")

    file (GLOB_RECURSE _find_files "${_dir}" "*.${_extension}")
    list (LENGTH _find_files _find_files_length)

    if (${_find_files_length} GREATER 0)
        message ("** Find ${_extension} extension.")
        find_package (${_module})
    endif ()
endfunction () 
