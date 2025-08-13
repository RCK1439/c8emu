set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/client/client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/core/debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/core/memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/chip8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/cpu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/instructions.c
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/ram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/rom.c
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/stack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/debug_overlay.c
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/renderer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/client/client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/client/config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/core/debug.h
    ${CMAKE_CURRENT_SOURCE_DIR}/core/memory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/core/platform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/core/types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/chip8.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/cpu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/instructions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/keyboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/ram.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/rom.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/specifications.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator/stack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/debug_overlay.h
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/renderer.h
)

add_executable(c8emu ${SOURCES} ${HEADERS})

if(MSVC)
    target_compile_options(c8emu PRIVATE /W4 /WX)
    target_compile_definitions(c8emu PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(c8emu PRIVATE -Wall -Werror -Wpedantic -Wextra)
endif()

target_include_directories(c8emu PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(c8emu PROPERTIES
    OUTPUT_NAME "c8emu"
    VERSION ${c8emu_VERSION_MAJOR}.${c8emu_VERSION_MINOR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)