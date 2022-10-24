add_executable(sharpdisp_hello_world
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_hello_world
    pico_stdlib
    SHARPDISP
    BITMAPSHAPES
    BITMAPTEXT
    LIBERATION_SANS_36
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_hello_world 1)
#pico_enable_stdio_uart(sharpdisp_hello_world 0)

pico_add_extra_outputs(sharpdisp_hello_world)
