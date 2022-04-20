#include "avr.h"

while (1) {
	
	
	SET_BIT(DDRB, 0);
	avr_wait(500);
	CLR_BIT(DDRB, 0);
	avr_wait(500);
	
}