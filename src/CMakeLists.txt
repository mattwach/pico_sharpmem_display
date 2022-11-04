cmake_minimum_required(VERSION 3.12)

#project(SHARPDISP C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(BITMAP INTERFACE)
target_include_directories(BITMAP  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_sources(BITMAP  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/bitmap.c
)

add_library(BITMAPTEXT INTERFACE)
target_include_directories(BITMAPTEXT  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(BITMAPTEXT  INTERFACE BITMAP)
target_sources(BITMAPTEXT  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/bitmaptext.c
    ${CMAKE_CURRENT_LIST_DIR}/rle.c
)

add_library(BITMAPIMAGE INTERFACE)
target_include_directories(BITMAPIMAGE  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(BITMAPIMAGE  INTERFACE BITMAP)
target_sources(BITMAPIMAGE  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/bitmapimage.c
    ${CMAKE_CURRENT_LIST_DIR}/rle.c
)

add_library(BITMAPSHAPES INTERFACE)
target_include_directories(BITMAPSHAPES  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(BITMAPSHAPES  INTERFACE BITMAP)
target_sources(BITMAPSHAPES  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/bitmapshapes.c
)

add_library(BITMAPCONSOLE INTERFACE)
target_include_directories(BITMAPCONSOLE  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(BITMAPCONSOLE  INTERFACE BITMAPTEXT)
target_sources(BITMAPCONSOLE  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/bitmapconsole.c
)

add_library(SHARPDISP INTERFACE)
target_include_directories(SHARPDISP  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(SHARPDISP  INTERFACE pico_stdlib hardware_spi BITMAP)
target_sources(SHARPDISP  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/sharpdisp.c
)

add_library(CONSOLE INTERFACE)
target_include_directories(CONSOLE  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(CONSOLE  INTERFACE BITMAPCONSOLE SHARPDISP)
target_sources(CONSOLE  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/console.c
)

add_library(SHARPDOUBLEBUFFER INTERFACE)
target_include_directories(SHARPDOUBLEBUFFER  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(SHARPDOUBLEBUFFER  INTERFACE SHARPDISP pico_multicore)
target_sources(SHARPDOUBLEBUFFER  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/doublebuffer.c
)

add_library(SHARPMETRICS INTERFACE)
target_include_directories(SHARPMETRICS  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(SHARPMETRICS  INTERFACE SHARPDISP)
target_sources(SHARPMETRICS  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/metrics.c
)
