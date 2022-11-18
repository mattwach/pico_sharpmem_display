add_executable(sharpdisp_test_alldraw
        main.c
        bitmaptests.c
        consoletests.c
        common.c
        images.c
        imagetests.c
        shapetests.c
        texttests.c
        )


# pull in common dependencies
target_link_libraries(sharpdisp_test_alldraw
    pico_stdlib
    SHARPDISP
    SHARPDOUBLEBUFFER
    BITMAPCONSOLE
    BITMAPIMAGE
    BITMAPSHAPES
    BITMAPTEXT
    LIBERATION_MONO_10
    LIBERATION_SANS_18
)

# enable usb output, disable uart output
pico_enable_stdio_usb(sharpdisp_test_alldraw 1)
#pico_enable_stdio_uart(sharpdisp_bitmapimage 0)

pico_add_extra_outputs(sharpdisp_test_alldraw)
