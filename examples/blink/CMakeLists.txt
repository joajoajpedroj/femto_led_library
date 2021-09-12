add_executable(blink
	blink.c
)

target_link_libraries(blink
	pico_stdlib
	femto_led_library
)

pico_add_extra_outputs(blink)