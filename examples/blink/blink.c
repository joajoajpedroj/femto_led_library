#include "pico/stdlib.h"
#include "femto_led_library.h"

int main(){
	Strip strip = init_strip(1, 0);

	bool led_state;
	while(true){
		led_state = !led_state;
		strip.leds[0].R = 0xff * led_state;
		update_strip(strip);
		sleep_ms(1000);
	}
}
