cmake_policy(SET CMP0079 NEW)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
# rklog
add_subdirectory(rklog-cpp)
target_include_directories(rklog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/rklog-cpp/include)
target_link_libraries(c8emu rklog)
endif()

# raylib
add_subdirectory(raylib)
target_include_directories(raylib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/raylib/src/)
target_link_libraries(c8emu raylib)