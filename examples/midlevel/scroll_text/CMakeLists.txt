add_executable(sharpdisp_scroll_text
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_scroll_text
    pico_stdlib
    SHARPDISP
    BITMAPTEXT
    LIBERATION_SANS_36
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_scroll_text 1)
#pico_enable_stdio_uart(sharpdisp_scroll_text 0)

pico_add_extra_outputs(sharpdisp_scroll_text)
