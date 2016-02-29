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
    get_filename_component (_absolute "${_find_dir}" ABSOLUTE)

    set (_obj_suffix    ".o")
    set (_src_suffix    ".cpp")
    set (_obfus_suffix  ".obf.cpp")
    set (_proto_suffix  ".proto")
    set (_cuda_suffix   ".cu")

    set (${_objs})
    set (${_libs})
    set (_static_libs)

    # C++ Obfuscator or Native C/C++ Source files.
    if (USE_OBFUSCATE AND CXX_OBFUSCATOR_FOUND)
    else ()
        file (GLOB_RECURSE _find_srcs "${_absolute}/*${_src_suffix}")
    endif ()
    list (APPEND ${_objs} ${_find_srcs})

    # Google-protocol-buffers files.
    if (USE_PROTOBUF AND PROTOBUF_FOUND)
        file (GLOB_RECURSE _find_protos "${_absolute}/*${_proto_suffix}")
        protobuf_generate_cpp_bugfix (_proto_srcs _proto_headers "${_find_protos}")
        list (APPEND ${_objs} ${_proto_srcs})
        list (APPEND ${_libs} ${PROTOBUF_LIBRARIES})
        list (APPEND ${_libs} "-lz")
        #list (APPEND _static_libs "${PROTOBUF_LIBRARY}")
    endif ()

    # NVIDIA CUDA files.
    if (USE_CUDA AND CUDA_FOUND)
        file (GLOB_RECURSE _find_cudas "${_absolute}/*${_cuda_suffix}")
        foreach (_cuda_cusor ${_find_cudas})
            cuda_compile (_cuda_cusor_object ${_cuda_cusor})
            list (APPEND ${_objs} ${_cuda_cusor_object})
        endforeach ()
        list (APPEND ${_libs} ${CUDA_CUDART_LIBRARY})
    endif ()

    # whole-archive, static libraries.
    #insert_whole_archive_flags (_static_libs)
    #list (INSERT ${_libs} 0 ${_static_libs})

    # Result output.
    set (${_objs} ${${_objs}} PARENT_SCOPE)
    set (${_libs} ${${_libs}} PARENT_SCOPE)
endfunction ()

