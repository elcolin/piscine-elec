#include "game.h"

void start_game(uint8_t im_master) {
	leds_countdown();
	while (1) {
		// if MASTER => check if sw1 pressed or recv from slave
			// if sw1 pressed => send loose to slave
			// if recv send win to slave
		
		// if SLAVE => check if sw1 press or mode changed
			// if sw1 pressed send sw1pressed to master
			// if mode changed => read it then check if its a loose or win
		// if (im_master) {
		// 	if (SW1_PRESSED) {
		// 		break;
		// 	}
		// 	// if () recv
		// 	// break;
		// }
		// else {
		// 	if (SW1_PRESSED)
		// }
	}
}