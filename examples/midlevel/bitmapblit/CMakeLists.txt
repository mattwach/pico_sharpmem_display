add_executable(sharpdisp_bitmapblit
        rose.c
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_bitmapblit
    pico_stdlib
    SHARPDISP
    BITMAPIMAGE
    BITMAPSHAPES
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_bitmapblit 1)
#pico_enable_stdio_uart(sharpdisp_bitmapblit 0)

pico_add_extra_outputs(sharpdisp_bitmapblit)
