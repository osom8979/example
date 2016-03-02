# CMake compile utility script.

#! find object & library files.
#
# \param _objs      [out] value name of result object files.
# \param _libs      [out] value name of result library files.
# \param _find_dir  [in]  find directory.
function (find_compile_object _objs _libs _find_dir)
    #message ("_objs: ${_objs}")
    #message ("_libs: ${_libs}")
    #message ("_find_dir: ${_find_dir}")

    set (_obj_suffix   ".o")
    set (_src_suffix   ".cpp")
    set (_obfus_suffix ".obf.cpp")
    set (_proto_suffix ".proto")
    set (_cuda_suffix  ".cu")

    set (${_objs})
    set (${_libs})
    set (_static_libs)


    # source files.
    get_filename_component (_absolute "${_find_dir}" ABSOLUTE)
    file (GLOB_RECURSE _find_srcs "${_absolute}/*${_src_suffix}")


    # C++ Obfuscator or Native C/C++ Source files.
    set (_obfus_excepted_path "${CMAKE_CURRENT_SOURCE_DIR}/obfus_excepted")
    if (USE_OBFUSCATE)
        find_package (CxxObfuscator)
    endif ()
    if (USE_OBFUSCATE AND CXX_OBFUSCATOR_FOUND AND (EXISTS "${_obfus_excepted_path}"))
        obfus_generate_cpp (_find_obfus "${_find_srcs}" "${_obfus_excepted_path}")
        list (APPEND ${_objs} ${_find_obfus})
    else ()
        list (APPEND ${_objs} ${_find_srcs})
    endif ()

    # Google-protocol-buffers files.
    if (USE_PROTOBUF)
        find_package (Protobuf)
    endif ()
    if (USE_PROTOBUF AND PROTOBUF_FOUND)
        file (GLOB_RECURSE _find_protos "${_absolute}/*${_proto_suffix}")
        list (LENGTH _find_protos _find_protos_length)
        if (${_find_protos_length} GREATER 0)
            protobuf_generate_cpp_bugfix (_proto_srcs _proto_headers "${_find_protos}")
            list (APPEND ${_objs} ${_proto_srcs})
            list (APPEND ${_libs} ${PROTOBUF_LIBRARIES})
            list (APPEND ${_libs} "-lz")
            #list (APPEND _static_libs "${PROTOBUF_LIBRARY}")
        endif ()
    endif ()

    # NVIDIA CUDA files.
    if (USE_CUDA)
        find_package (CUDA)
    endif ()
    if (USE_CUDA AND CUDA_FOUND)
        file (GLOB_RECURSE _find_cudas "${_absolute}/*${_cuda_suffix}")
        list (LENGTH _find_cudas _find_cudas_length)
        if (${_find_cudas_length} GREATER 0)
            foreach (_cuda_cusor ${_find_cudas})
                cuda_compile (_cuda_cusor_object ${_cuda_cusor})
                list (APPEND ${_objs} ${_cuda_cusor_object})
            endforeach ()
            list (APPEND ${_libs} ${CUDA_CUDART_LIBRARY})
        endif ()
    endif ()

    # whole-archive, static libraries.
    list (LENGTH _static_libs _static_libs_length)
    if (${_static_libs_length} GREATER 0)
        insert_whole_archive_flags (_static_libs)
        list (INSERT ${_libs} 0 ${_static_libs})
    endif ()

    # Result output.
    set (${_objs} ${${_objs}} PARENT_SCOPE)
    set (${_libs} ${${_libs}} PARENT_SCOPE)
endfunction ()

