add_executable(sharpdisp_bitmapimage
        images.c
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_bitmapimage
    pico_stdlib
    SHARPDISP
    BITMAPIMAGE
    BITMAPTEXT
    LIBERATION_MONO_14
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_bitmapimage 1)
#pico_enable_stdio_uart(sharpdisp_bitmapimage 0)

pico_add_extra_outputs(sharpdisp_bitmapimage)
