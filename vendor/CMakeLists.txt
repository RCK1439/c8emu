cmake_policy(SET CMP0079 NEW)

# rklog
add_library(rklog INTERFACE)
target_include_directories(rklog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/rklog)
target_link_libraries(c8emu rklog)

# raylib
add_subdirectory(raylib)
target_include_directories(raylib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/raylib/src/)
target_link_libraries(c8emu raylib)