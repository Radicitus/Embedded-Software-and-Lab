#include <xc.h>
#include "avr.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

void locked_out() {
	for (int i = 0; i < 15; i++) {
		beep(3);
		avr_wait(500);
	}
	
	beep(5);
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

main() {
	SET_BIT(DDRB, 0);
	SET_BIT(DDRA, 0);
	
	// State
	int is_locked = 0;
	int is_input_mode = 1;
	
	// Password
	int prev_pass[10];
	int pass_index = 0;
	int current_pass = -1;
	int pass_len = 4;
	int max_incorrect_attempt = 3;
	int current_attempts = 0;
	
	// Beep Codes
	int locked = 1;
	int unlocked = 2;
	int incorrect = 3;
	
    while(1) {
        
		if (!is_locked) {
			int input_count = 0;
			char input[4];
			
			// Input not 4 digits
			while (input_count < pass_len) {
				
				// Get input
				int num = get_key();
				
				if (num) {
					// Convert to string
					char num_str[1];
					sprintf(num_str, "%d", num);
					strncat(input, num_str, 1);
					
					input_count++;
				}
				
				avr_wait(200);
			}
			
			// Input now 4 digits
			current_pass = atoi(input);	
			
			// Wait til either locked or enter new pass
			int is_waiting_for_input = 1;
			while (is_waiting_for_input) {
				int num = get_key();
				
				// Lock
				if (num == 4) {
					beep(locked);
					is_locked = 1;
					is_waiting_for_input = 0;
				}
				
				// Enter new pass
				if (num == 8) {
					beep(1);
					prev_pass[pass_index] = current_pass;
					pass_index++;
					current_pass = -1;
					is_waiting_for_input = 0;
				}
			}
			
		} else {
			int attempt_count = 0;
			char attempt[4];
			
			// Attempt not 4 digits
			while (attempt_count < pass_len) {
				
				// Get input
				int num = get_key();
				
				if (num) {
					// Convert to string
					char num_str[1];
					sprintf(num_str, "%d", num);
					strncat(attempt, num_str, 1);
					
					attempt_count++;				
				}

				avr_wait(200);
				beep(1);
			}
			
			// Input now 4 digits
			int current_attempt = atoi(attempt);
		}
    }
}