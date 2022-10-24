# This is only used with PI PICO and NOT AVR.
cmake_minimum_required(VERSION 3.12)

#project(SHARPDISP C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(LIBERATION_MONO_9 INTERFACE)
target_include_directories(LIBERATION_MONO_9  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_MONO_9  INTERFACE)
target_sources(LIBERATION_MONO_9  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_mono_9.c
)

add_library(LIBERATION_MONO_12 INTERFACE)
target_include_directories(LIBERATION_MONO_12  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_MONO_12  INTERFACE)
target_sources(LIBERATION_MONO_12  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_mono_12.c
)

add_library(LIBERATION_MONO_24 INTERFACE)
target_include_directories(LIBERATION_MONO_24  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_MONO_24  INTERFACE)
target_sources(LIBERATION_MONO_24  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_mono_24.c
)

add_library(LIBERATION_MONO_32 INTERFACE)
target_include_directories(LIBERATION_MONO_32  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_MONO_32  INTERFACE)
target_sources(LIBERATION_MONO_32  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_mono_32.c
)

add_library(CONSOLE_DEFAULT_FONT INTERFACE)
target_link_libraries(CONSOLE_DEFAULT_FONT  INTERFACE LIBERATION_MONO_12)

add_library(LIBERATION_SANS_12 INTERFACE)
target_include_directories(LIBERATION_SANS_12  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_SANS_12  INTERFACE)
target_sources(LIBERATION_SANS_12  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_sans_12.c
)

add_library(LIBERATION_SANS_18 INTERFACE)
target_include_directories(LIBERATION_SANS_18  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_SANS_18  INTERFACE)
target_sources(LIBERATION_SANS_18  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_sans_18.c
)

add_library(LIBERATION_SANS_24 INTERFACE)
target_include_directories(LIBERATION_SANS_24  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_SANS_24  INTERFACE)
target_sources(LIBERATION_SANS_24  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_sans_24.c
)

add_library(LIBERATION_SANS_36 INTERFACE)
target_include_directories(LIBERATION_SANS_36  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_SANS_36  INTERFACE)
target_sources(LIBERATION_SANS_36  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_sans_36.c
)

add_library(LIBERATION_SANS_48 INTERFACE)
target_include_directories(LIBERATION_SANS_48  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_SANS_48  INTERFACE)
target_sources(LIBERATION_SANS_48  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_sans_48.c
)

add_library(LIBERATION_SANS_60 INTERFACE)
target_include_directories(LIBERATION_SANS_60  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_SANS_60  INTERFACE)
target_sources(LIBERATION_SANS_60  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_sans_60.c
)

add_library(LIBERATION_SANS_80 INTERFACE)
target_include_directories(LIBERATION_SANS_80  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(LIBERATION_SANS_80  INTERFACE)
target_sources(LIBERATION_SANS_80  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/liberation_sans_80.c
)
