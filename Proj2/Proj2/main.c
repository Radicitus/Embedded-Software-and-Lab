#include "avr.h"
#include "lcd.h"


void
avr_wait(unsigned short msec)
{
	TCCR0 = 3;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

int is_pressed(int r, int c) {
	// Set all 8 GPIOs to N/C
	DDRC = 0;
	PORTC = 0;
	
	// Set r to "0"
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	
	// Set c to "w1"
	CLR_BIT(DDRC, c + 4);
	SET_BIT(PORTC, c + 4);
	
	if (GET_BIT(PINC, c + 4)) {
		return 0;
	}
	return 1;
}

int get_key() {
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			if (is_pressed(r, c)) {
				return ((4 * r) + c + 1);
			}
		}
	}
	return 0;
}


int main(void)
{
    // Set write to PB0
    SET_BIT(DDRB, 0);
    	
    while (1) {
		
		int i, k;
		k = get_key();
		for (i=0; i < k; i++) {
			// Blink LED
			SET_BIT(PORTB, 0);
			avr_wait(200);
			CLR_BIT(PORTB, 0);
			avr_wait(200);
		}
		avr_wait(500);
		
		
		/*
		if (is_pressed(0,0)) {
			SET_BIT(PORTB, 0);
		} else {
			CLR_BIT(PORTB, 0);	
		}
		*/
    }
}