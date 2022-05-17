#include <xc.h>
#include "avr.h"
#include "lcd.h"


int get_sample() {
	int result;
	
	ADMUX = 0b01000000;
	ADCSRA = 0b11000000;
	
	
	return result;
}


int main(void)
{
	// LCD Setup
	char display[20];
	avr_init();
	lcd_init();
	
	// Vars
	int max;
	int min;
	
    while(1)
    {
		
		int new_sample = get_sample();
		
		// Max calc
		if (new_sample > max) {
			max = new_sample;
		}
		
		// Min calc
		if (new_sample < min) {
			min = new_sample;
		}
		
        sprintf(display, "%.2f", (new_sample / 1023.0) * 5);
		lcd_clr();
		lcd_pos(0, 0);
		lcd_puts2(display);
		avr_wait(500);
    }
}