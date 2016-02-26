## Find the C++ Obfuscator program.
#
# The following variables are optionally searched for defaults
#  CXX_OBFUSCATOR_ROOT_DIR
#
# The following are set after configuration is done:
#  CXX_OBFUSCATOR_FOUND
#  CXX_OBFUSCATOR_COMPILER

set (CXX_OBFUSCATOR_SEARCH_PATHS
  "/usr/bin"
  "/usr/local/bin"
  "/Applications/Run\ C++\ Obfuscator.app/Contents/Resources/bin/"
  "$ENV{CXX_OBFUSCATOR_ROOT_DIR}"
)

find_program (CXX_OBFUSCATOR_COMPILER NAMES "cxx-obfus" PATHS ${CXX_OBFUSCATOR_SEARCH_PATHS})

set (LOOKED_FOR
    CXX_OBFUSCATOR_COMPILER
)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (CXX_OBFUSCATOR DEFAULT_MSG ${LOOKED_FOR})

if (CXX_OBFUSCATOR_FOUND)
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found C++ Obfuscator (Compiler: ${CXX_OBFUSCATOR_COMPILER})")
endif ()

