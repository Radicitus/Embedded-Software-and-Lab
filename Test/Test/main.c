#include <xc.h>
#include "avr.h"

int num_to_digit(int num) {
	if (num == 1) {
		return 1;
	}
	if (num == 2) {
		return 2;
	}
	if (num == 3) {
		return 3;
	}
	if (num == 5) {
		return 4;
	}
	if (num == 6) {
		return 5;
	}
	if (num == 7) {
		return 6;
	}
	if (num == 9) {
		return 7;
	}
	if (num == 10) {
		return 8;
	}
	if (num == 11) {
		return 9;
	}
	if (num == 14) {
		return 0;
	}
	return -1;
}

void sound_wait(unsigned short nsec)
{
	TCCR0 = 2;
	while (nsec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 8) * 0.0001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

void beep(int times) {
	for (int t = 0; t < times; t++) {
		for (int i = 0; i < 50; i++) {
			SET_BIT(PORTA, 0);
			sound_wait(19);
			CLR_BIT(PORTA, 0);
			sound_wait(19);
		}
		
		avr_wait(50);
	}
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
	DDRC = 0;
	SET_BIT(DDRA, 0);
	
    while(1)
    {
        int k = num_to_digit(get_key());
		
		beep(k);
		avr_wait(500);
    }
}