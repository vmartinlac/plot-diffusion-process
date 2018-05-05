find_path(MuParser_INCLUDE_DIR NAMES "muparser")
find_library(MuParser_LIBRARY "muParser.h")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MuParser DEFAULT_MSG MuParser_LIBRARY MuParser_INCLUDE_DIR)

set(MuParser_INCLUDE_DIRS ${MuParser_INCLUDE_DIR})
set(MuParser_LIBRARIES ${MuParser_LIBRARY})

add_library(MuParser INTERFACE)
target_link_libraries(MuParser INTERFACE ${MuParser_LIBRARIES})
target_include_directories(MuParser INTERFACE ${MuParser_INCLUDE_DIRS})

