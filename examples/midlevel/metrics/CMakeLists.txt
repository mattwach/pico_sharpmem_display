add_executable(sharpdisp_metrics
        main.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_metrics
    pico_stdlib
    SHARPDISP
    SHARPMETRICS
    BITMAPTEXT
    BITMAPSHAPES
    LIBERATION_SANS_18
)

# enable usb output, disable uart output
#pico_enable_stdio_usb(sharpdisp_metrics 1)
#pico_enable_stdio_uart(sharpdisp_metrics 0)

pico_add_extra_outputs(sharpdisp_metrics)
