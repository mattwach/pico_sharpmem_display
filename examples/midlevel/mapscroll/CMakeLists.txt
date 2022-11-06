add_executable(sharpdisp_mapscroll
        map.c
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_mapscroll
    pico_stdlib
    SHARPDISP
    SHARPDOUBLEBUFFER
    BITMAPIMAGE
    SHARPMETRICS
    BITMAPTEXT
    LIBERATION_MONO_12
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_mapscroll 1)
#pico_enable_stdio_uart(sharpdisp_mapscroll 0)

pico_add_extra_outputs(sharpdisp_mapscroll)
