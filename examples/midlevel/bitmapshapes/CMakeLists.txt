add_executable(sharpdisp_bitmapshapes
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_bitmapshapes
    pico_stdlib
    SHARPDISP
    SHARPDOUBLEBUFFER
    BITMAPSHAPES
    BITMAPTEXT
    LIBERATION_MONO_12
    LIBERATION_SANS_24
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_bitmapshapes 1)
#pico_enable_stdio_uart(sharpdisp_bitmapshapes 0)

pico_add_extra_outputs(sharpdisp_bitmapshapes)
