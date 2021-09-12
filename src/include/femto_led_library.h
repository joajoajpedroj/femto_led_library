#include "hardware/pio.h"
#include "hardware/dma.h"
#include "ws2812.pio.h"
#include <stdlib.h>
#include "pico/stdlib.h"
// LED data type containing the red, green and blue values
typedef struct {
	uint8_t G, R, B;
} Led;

/*! \brief Strip data type containing corresponding LED array and information about the PIO state machine used and DMA configuration.
 *	
 *	\param leds pointer to LEDs array
 *
 *  \param pio PIO instance used
 *  \param sm SM used
 *  
 *  \param channel DMA channel used
 *  \param c DMA configuration
 */
typedef struct {
	PIO pio;
	int sm;

	int channel;
	dma_channel_config c;

	unsigned int nLeds;
	Led* leds;
} Strip;

/*!
 * \brief Helper function to create an led strip
 *
 * \param nLeds Number of leds in the strip
 */

Strip init_strip(unsigned int nLeds, int pin){
	Strip result;

	Led* leds = malloc(sizeof(*leds) * nLeds); // Allocate memory for the LEDs "frame buffer"

	for(int i = 0; i < nLeds; i++){
		leds[0].R = 0;
		leds[0].G = 0;
		leds[0].B = 0;
	}

	// todo ensure PIO is available
	PIO pio = pio0;
	int sm = pio_claim_unused_sm(pio, true);
	uint offset = pio_add_program(pio, &ws2812_program);

	ws2812_program_init(pio, sm, offset, pin, 800000, true);

	int channel = dma_claim_unused_channel(true); // Require the dma
	dma_channel_config c = dma_channel_get_default_config(channel);
	channel_config_set_transfer_data_size(&c, DMA_SIZE_8); // Set transfer size to a byte
	channel_config_set_dreq(&c, pio_get_dreq(pio, sm, true)); // SM will determine the transfer speed

	result.pio = pio;
	result.sm = sm;

	result.channel = channel;
	result.c = c;

	result.nLeds = nLeds;
	result.leds = leds;
	return result;
}

/*!
 * \brief Helper function to update the physical LEDS
 *
 * \param strip pointer to strip to be updated.
 */
void update_strip(Strip strip){
	dma_channel_config c = strip.c;
	PIO pio = strip.pio;

//	if(dma_channel_is_busy(strip.channel)){
//		dma_channel_wait_for_finish_blocking(strip.channel);
//	}

	dma_channel_configure(strip.channel, &c,
		&pio->txf[strip.sm], // Destination pointer (the state machine)
		strip.leds,          // Source pointer (LEDs array)
		3 * strip.nLeds,     // Number of transfers
		true                 // Start immediately
	);
}