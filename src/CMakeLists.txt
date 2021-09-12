add_library(femto_led_library INTERFACE)

pico_generate_pio_header(femto_led_library ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

target_include_directories(femto_led_library INTERFACE
${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(femto_led_library INTERFACE
	pico_stdlib
	hardware_pio
	hardware_dma
)