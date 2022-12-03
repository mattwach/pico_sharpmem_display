add_executable(sharpdisp_console_advanced_setup
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_console_advanced_setup
    pico_stdlib
    SHARPCONSOLE
    SHARPDISP
    LIBERATION_SANS_18
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_console_advanced_setup 1)
#pico_enable_stdio_uart(sharpdisp_console_advanced_setup 0)

pico_add_extra_outputs(sharpdisp_console_advanced_setup)
