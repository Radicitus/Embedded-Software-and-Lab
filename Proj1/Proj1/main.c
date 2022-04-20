#include "avr.h"

void avr_init(void) {
	WDTCR = 15;
}

void avr_wait(unsigned short msec) {
	TCCR0 = 3;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

main () {
	
	// Set read from PB1, Write to PB0
	SET_BIT(DDRB, 0);
	CLR_BIT(DDRB, 1);
	
	while (1) {
		
		// Check if button pushed
		if (!GET_BIT(PINB, 1)) {
			
			// Blink LED
			SET_BIT(PORTB, 0);
			avr_wait(500);
			CLR_BIT(PORTB, 0);
			avr_wait(500);
		} 
		// When button not pushed, turn off
		else {
			CLR_BIT(PORTB, 0);
		}
	}
}