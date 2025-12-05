set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/client/client.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/chip8.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/cpu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/instructions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/ram.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/rom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/stack.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/renderer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/client/client.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/client/config.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/core/debug.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/platform.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/types.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/chip8.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/cpu.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/instructions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/keyboard.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/ram.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/rom.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/specifications.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/stack.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/debug_overlay.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/renderer.hpp
)

add_executable(c8emu ${SOURCES} ${HEADERS})

if(MSVC)
    target_compile_options(c8emu PRIVATE /W4 /WX)
    target_compile_definitions(c8emu PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(c8emu PRIVATE -Wall -Wextra -Wpedantic -Werror -fno-exceptions)
endif()

target_include_directories(c8emu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(c8emu PROPERTIES
    OUTPUT_NAME "c8emu"
    VERSION ${c8emu_VERSION_MAJOR}.${c8emu_VERSION_MINOR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)