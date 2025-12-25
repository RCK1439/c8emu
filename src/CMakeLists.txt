set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Client.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/Chip8.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/CPU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/Instructions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/RAM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/ROM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/CallStack.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Renderer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/EntryPoint.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Client.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Client/Config.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Debug.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Platform.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Types.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/Chip8.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/CPU.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/Instructions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/Keyboard.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/RAM.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/ROM.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/Spec.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Emulator/CallStack.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/DebugOverlay.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Renderer.hpp
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