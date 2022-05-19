#include <xc.h>
#include "avr.h"
#include "lcd.h"



float get_voltage(int sample) {
	return (sample / 1023.0) * 5;
}

int get_sample() {
	ADMUX = 0b01000000;
	ADCSRA = 0b11000000;
	while(GET_BIT(ADCSRA, 6));
	return ADC;
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
	// LCD Setup
	char display1[40];
	char display2[40];
	avr_init();
	lcd_init();
	
	// Vars
	float max = 0.0;
	float min = 0.0;
	float sum = 0.0; 
	float count = 0.0;
	float average = 0.0;
	int display_vals = 0;
	
	// DisplayVars

	
    while(1)
    {
		
		// Get sample
		int new_sample = get_sample();
		
		
		// Clear min/max/avg
		if (get_key() == 1) {
			display_vals = 0;
		}
		
		// Restart min/max/avg sampling
		if (get_key() == 2) {
			max = 0.0, min = new_sample;
			sum = 0.0, count = 0.0;
			display_vals = 1;
		}
		
		// Max calc
		if (new_sample > max) {
			max = new_sample;
		}
		
		// Min calc
		if (new_sample < min) {
			min = new_sample;
		}
		
		// Avg calc
		sum += new_sample;
		count++;
		average = sum/count;
		
		
		if(display_vals == 0){
			sprintf(display1, "V %.2f  Avg ----", get_voltage(new_sample));
			sprintf(display2, "Mx ----  Mn ----");
			lcd_clr();
			lcd_pos(0, 0);
			lcd_puts2(display1);
			lcd_pos(1, 0);
			lcd_puts2(display2);
			avr_wait(500);
		}
		else{
			sprintf(display1, "V %.2f Avg %.2f", get_voltage(new_sample), get_voltage(average));
			sprintf(display2, "Mx %.2f Mn %.2f", get_voltage(max), get_voltage(min));
			lcd_clr();
			lcd_pos(0, 0);
			lcd_puts2(display1);
			lcd_pos(1, 0);
			lcd_puts2(display2);
			avr_wait(500);
		}
        
    }
}