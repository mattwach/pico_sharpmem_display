add_executable(sharpdisp_doublebuffer
        ball.c
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_doublebuffer
    pico_stdlib
    SHARPDISP
    SHARPDOUBLEBUFFER
    SHARPMETRICS
    BITMAPTEXT
    BITMAPSHAPES
    LIBERATION_SANS_18
    LIBERATION_MONO_14
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_doublebuffer 1)
#pico_enable_stdio_uart(sharpdisp_doublebuffer 0)

pico_add_extra_outputs(sharpdisp_doublebuffer)
