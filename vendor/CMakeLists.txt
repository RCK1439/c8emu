cmake_policy(SET CMP0079 NEW)

add_subdirectory(rklog-cpp)
target_include_directories(rklog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/rklog-cpp/include)
target_link_libraries(c8emu rklog)

add_subdirectory(SFML-3.0.2)
target_include_directories(c8emu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SFML-3.0.2/include)
target_link_libraries(c8emu sfml-system sfml-main sfml-audio sfml-audio sfml-window sfml-graphics sfml-network)