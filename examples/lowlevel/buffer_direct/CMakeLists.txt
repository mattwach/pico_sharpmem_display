add_executable(sharpdisp_buffer_direct
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_buffer_direct
    pico_stdlib
    SHARPDISP
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_buffer_direct 1)
#pico_enable_stdio_uart(sharpdisp_buffer_direct 0)

pico_add_extra_outputs(sharpdisp_buffer_direct)
