add_executable(sharpdisp_console_count
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_console_count
    pico_stdlib
    CONSOLE
    CONSOLE_DEFAULT_FONT
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_console_count 1)
#pico_enable_stdio_uart(sharpdisp_console_count 0)

pico_add_extra_outputs(sharpdisp_console_count)
