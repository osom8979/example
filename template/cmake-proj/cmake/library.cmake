## CMake custom library process.

#! run step.
#
# \param _name        [in]  Step name.
# \param _result_code [out] Value name of return code.
# \param _cmd         [in]  Command-line.
# \param _working_dir [in]  Working directory.
# \param _log_path    [in]  Log file path.
function (run_step _name _result_code _cmd _working_dir _log_path)
    set (${_result_code})

    list (LENGTH _cmd _cmd_length)
    if (_cmd_length GREATER 0)
        set (_stdout_log_path "${_log_path}.${_name}.stdout")
        set (_stderr_log_path "${_log_path}.${_name}.stderr")

        message (">> ${_name} step process: ${_cmd}")
        message (">> ${_name} step stdout path: ${_stdout_log_path}")

        execute_process (
            COMMAND ${_cmd}
            WORKING_DIRECTORY "${_working_dir}"
            RESULT_VARIABLE   ${_result_code}
            OUTPUT_FILE       "${_stdout_log_path}")
        set (${_result_code} ${${_result_code}} PARENT_SCOPE)
    else ()
        message (">> Undefined ${_name} process.")
        set (${_result_code} 0 PARENT_SCOPE)
    endif ()
endfunction ()

#! Install third-party library.
#
# \param VERBOSE    [in] Print verbose message. (ON/OFF)
# \param CLEAN      [in] Remove extracted files. (ON/OFF)
# \param NAME       [in] Project name. (e.g. zlib)
# \param DIR_NAME   [in] Extracted directory name. (e.g. zlib-1.2.8)
# \param URL        [in] Download URL. (e.g. http://zlib.net/zlib-1.2.8.tar.gz)
# \param MD5        [in] MD5 hash of downloaded file. (e.g. 44d667c142d7cda120332623eab69f40)
# \param TEMP_DIR   [in] Temporary directory. (e.g. ${PREFIX}/temp)
# \param DEST_NAME  [in] Destination file name of the download. (e.g. ${DIR_NAME}.tar.gz)
# \param WORK_NAME  [in] Working directory name. (e.g. ${DIR_NAME})
# \param PREFIX     [in] Install prefix. (e.g. ${PREFIX})
# \param ALREADY    [in] Installed file path. (e.g. ${PREFIX}/lib/libz.a)
# \param UPDATE     [in] Command-line args for the update/patch. (default: skip)
# \param AUTOCONFIG [in] Command-line args for the autoconfig. (default: skip)
# \param CONFIGURE  [in] Command-line args for the configure. (e.g. ./configure --prefix=${PREFIX})
# \param BUILD      [in] Command-line args for the build. (e.g. make -j8)
# \param INSTALL    [in] Command-line args for the install. (e.g. make install)
# \param TEST       [in] Command-line args for the test. (default: skip)
function (run_custom_library)
    include (CMakeParseArguments)
    set (_options           VERBOSE CLEAN)
    set (_one_value_args    NAME DIR_NAME URL MD5 TEMP_DIR DEST_NAME WORK_NAME PREFIX ALREADY)
    set (_multi_value_args  UPDATE AUTOCONFIG CONFIGURE BUILD INSTALL TEST)
    cmake_parse_arguments (_RUN_CUSTOM_PROCESS_COMMAND
        "${_options}" "${_one_value_args}" "${_multi_value_args}" ${ARGN})

    set (_verbose "${_RUN_CUSTOM_PROCESS_COMMAND_VERBOSE}")
    set (_clean   "${_RUN_CUSTOM_PROCESS_COMMAND_CLEAN}")

    set (_name      "${_RUN_CUSTOM_PROCESS_COMMAND_NAME}")
    set (_dir_name  "${_RUN_CUSTOM_PROCESS_COMMAND_DIR_NAME}")
    set (_url       "${_RUN_CUSTOM_PROCESS_COMMAND_URL}")
    set (_md5       "${_RUN_CUSTOM_PROCESS_COMMAND_MD5}")
    set (_temp_dir  "${_RUN_CUSTOM_PROCESS_COMMAND_TEMP_DIR}")
    set (_dest_name "${_RUN_CUSTOM_PROCESS_COMMAND_DEST_NAME}")
    set (_work_name "${_RUN_CUSTOM_PROCESS_COMMAND_WORK_NAME}")
    set (_prefix    "${_RUN_CUSTOM_PROCESS_COMMAND_PREFIX}")
    set (_already   "${_RUN_CUSTOM_PROCESS_COMMAND_ALREADY}")

    string (TIMESTAMP _timestamp "%Y%m%d_%H%M%S")

    set (_dest_path   "${_temp_dir}/${_dest_name}")
    set (_extract_dir "${_temp_dir}/${_dir_name}")
    set (_work_dir    "${_temp_dir}/${_work_name}")
    set (_log_path    "${_temp_dir}/${_dir_name}-${_timestamp}.log")

    set (_cmd_update     "${_RUN_CUSTOM_PROCESS_COMMAND_UPDATE}")
    set (_cmd_autoconfig "${_RUN_CUSTOM_PROCESS_COMMAND_AUTOCONFIG}")
    set (_cmd_configure  "${_RUN_CUSTOM_PROCESS_COMMAND_CONFIGURE}")
    set (_cmd_build      "${_RUN_CUSTOM_PROCESS_COMMAND_BUILD}")
    set (_cmd_install    "${_RUN_CUSTOM_PROCESS_COMMAND_INSTALL}")
    set (_cmd_test       "${_RUN_CUSTOM_PROCESS_COMMAND_TEST}")

    set (_cmd_download curl -k -o "${_dest_path}" "${_url}")
    set (_cmd_extract ${CMAKE_COMMAND} -E tar xf ${_dest_path})

    message (">> Run ${_name} build script.")
    message (">> Log file prefix: ${_log_path}")

    # Check install.
    if (EXISTS "${_already}")
        message (">> Already installed: ${_already}")
        return ()
    else ()
        message (">> Not found installed file: ${_already}")
    endif ()

    # Check downloaded file.
    set (_is_downloaded OFF)
    if (EXISTS "${_dest_path}")
        file (MD5 "${_dest_path}" _dest_md5)
        if ("${_md5}" STREQUAL "${_dest_md5}")
            message (">> MD5 check true. (${_md5})")
            set (_is_downloaded ON)
        else ()
            message (">> MD5 check false. (${_md5} != ${_dest_md5})")
            message (">> Remove downloaded file: ${_dest_path}")
            file (REMOVE "${_dest_path}")
        endif ()
    else ()
        message (">> Not found downloaded file: ${_dest_path}")
    endif ()

    # Download file.
    if (_is_downloaded)
        message (">> Skip download: ${_url}")
    else ()
        run_step (Download _last_error_code "${_cmd_download}" "${_temp_dir}" "${_log_path}")
    endif ()

    # Check MD5.
    file (MD5 "${_dest_path}" _dest_md5)
    if ("${_md5}" STREQUAL "${_dest_md5}")
        message (">> MD5 check true. (${_md5})")
    else ()
        message (WARNING ">> MD5 check false. (${_md5} != ${_dest_md5})")
        return ()
    endif ()

    # Extract downloaded file.
    if (NOT EXISTS "${_extract_dir}")
        file (REMOVE_RECURSE "${_extract_dir}")
    endif ()
    run_step (Extract _last_error_code "${_cmd_extract}" "${_temp_dir}" "${_log_path}")

    # Check the working directory.
    if (NOT EXISTS "${_work_dir}")
        message (WARNING ">> Not found working directory: ${_work_dir}")
        return ()
    endif ()

    run_step (Update      _last_error_code "${_cmd_update}"      "${_temp_dir}"  "${_log_path}")
    run_step (Autoconfig  _last_error_code "${_cmd_autoconfig}"  "${_work_dir}"  "${_log_path}")
    run_step (Configure   _last_error_code "${_cmd_configure}"   "${_work_dir}"  "${_log_path}")
    run_step (Build       _last_error_code "${_cmd_build}"       "${_work_dir}"  "${_log_path}")
    run_step (Install     _last_error_code "${_cmd_install}"     "${_work_dir}"  "${_log_path}")
    run_step (Test        _last_error_code "${_cmd_test}"        "${_work_dir}"  "${_log_path}")

    if (_clean)
        message (">> Cleaning.")
        file (REMOVE_RECURSE "${_extract_dir}")
    endif ()
endfunction ()

#! Build third-party libraries.
#
# \param _lib_list [in] list of library name.
# \param _prefix   [in] install prefix.
function (build_libraries _lib_list _prefix)

    set (_temp_prefix     "${_prefix}/temp")
    set (_include_prefix  "${_prefix}/include")
    set (_library_prefix  "${_prefix}/lib")

    if (NOT EXISTS ${_temp_prefix})
        file (MAKE_DIRECTORY ${_temp_prefix})
    endif ()

    # loop of library.
    foreach (_cursor ${_lib_list})
        set (_script_path "${PROJECT_SOURCE_DIR}/cmake/libraries/${_cursor}.cmake")

        if (USE_${_cursor})
            if (EXISTS "${_script_path}")
                message ("** Use the ${_cursor} library.")

                set (_library_verbose OFF)
                set (_library_clean   OFF)

                set (_library_name      "NAME")
                set (_library_dir_name  "NAME-VERSION")
                set (_library_url       "Download url")
                set (_library_md5       "MD5 hashcode")
                set (_library_temp_dir  "${_temp_prefix}")
                set (_library_dest_name "Download destination name")
                set (_library_work_name "Working directory of build process.")
                set (_library_prefix    "${_prefix}")
                set (_library_already   "Installed file name.")

                set (_library_update)
                set (_library_autoconfig)
                set (_library_configure  ./configure "--prefix=${_library_prefix}")
                set (_library_build      make)
                set (_library_install    make install)
                set (_library_test)

                include ("${_script_path}")

                run_custom_library (
                    VERBOSE     ${_library_verbose}
                    CLEAN       ${_library_clean}
                    NAME        ${_library_name}
                    DIR_NAME    ${_library_dir_name}
                    URL         ${_library_url}
                    MD5         ${_library_md5}
                    TEMP_DIR    ${_library_temp_dir}
                    DEST_NAME   ${_library_dest_name}
                    WORK_NAME   ${_library_work_name}
                    PREFIX      ${_library_prefix}
                    ALREADY     ${_library_already}
                    UPDATE      ${_library_update}
                    AUTOCONFIG  ${_library_autoconfig}
                    CONFIGURE   ${_library_configure}
                    BUILD       ${_library_build}
                    INSTALL     ${_library_install}
                    TEST        ${_library_test})

            else ()
                message ("** Not found ${_script_path}.")
            endif ()
        else ()
            message ("** Disable ${_cursor} library.")
        endif ()
    endforeach ()
endfunction ()

