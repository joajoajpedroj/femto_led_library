add_executable(test
	test.c
)

target_link_libraries(test
	femto_led_library
)

pico_add_extra_outputs(test)