## LevelDB library install script.
#
# _build_dir
# _prefix
# _static_lib_name

execute_process (COMMAND ${CMAKE_COMMAND} -E copy "${_build_dir}/${_static_lib_name}"  "${_prefix}/lib/${_static_lib_name}")
execute_process (COMMAND ${CMAKE_COMMAND} -E copy_directory "${_build_dir}/include"    "${_prefix}/include")

