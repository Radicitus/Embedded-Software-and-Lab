#include <avr/io.h>
#include "avr.h"
#include "lcd.h"

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

typedef struct {
	int year;
	unsigned char month;
	} DateTime;


// MAIN LOOP

int main(void)
{
    // Set write to PB0
    SET_BIT(DDRB, 0);
    
	// Init display
	lcd_init();


	char word[6] = "Hello";
    while (1) {
		
		avr_wait(1000);
		lcd_puts2(word);
		avr_wait(1000);
		
		
		/*
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
		*/
		
		
		
		
    }
}