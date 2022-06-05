#include <xc.h>
#include "avr.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
		
		avr_wait(150);
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
	
	// Password
	int prev_pass[10];
	int pass_index = 0;
	int current_pass = -1;
	int pass_len = 4;
	int max_incorrect_attempt = 3;
	int current_attempts = 0;
	
	// Beep Codes
	int locked = 2;
	int unlocked = 3;
	int incorrect = 4;
	int prev_used = 5;
	
    while(1) {
        
		if (!is_locked) {
			int input_count = 0;
			char input[4];
			
			// Input not 4 digits
			while (input_count < pass_len) {
				
				// Get input
				int num = num_to_digit(get_key());
				
				if (num != -1) {
					beep(1);
					
					// Convert to string
					char num_str[1];
					sprintf(num_str, "%d", num);
					strncat(input, num_str, 1);
					
					input_count++;
				}
				
				avr_wait(500);
			}
			
			// Input now 4 digits
			current_pass = atoi(input);	
			
			// Wait til either locked or enter new pass
			int is_waiting_for_input = 1;
			while (is_waiting_for_input) {
				int num = get_key();
				
				// Lock
				if (num == 4) {
					beep(1);
					
					int is_prev_used_pass = 0;
					
					for (int i = 0; i < pass_index; i++) {
						if (current_pass == prev_pass[i]) {
							avr_wait(500);
							beep(prev_used);
							is_prev_used_pass = 1;
							break;
						}
					}
					
					if (!is_prev_used_pass) {
						avr_wait(500);
						beep(locked);
						is_locked = 1;	
					}
					
					is_waiting_for_input = 0;
					avr_wait(500);
				}
				
				// Enter new pass
				if (num == 8) {
					beep(1);
					
					prev_pass[pass_index] = current_pass;
					pass_index++;
					current_pass = -1;
					is_waiting_for_input = 0;
					avr_wait(500);
				}
			}
			
		} else {
			if (current_attempts == max_incorrect_attempt) {
				locked_out();
				current_attempts = 0;
				avr_wait(500);
			} else {
				int attempt_input_count = 0;
				char attempt[4];
				
				// Locked beep count
				int locked_count = 0;
				
				// Attempt not 4 digits
				while (attempt_input_count < pass_len) {
					
					// Get input
					int num = num_to_digit(get_key());
					
					if (num != -1) {
						beep(1);
						
						// Convert to string
						char num_str[1];
						sprintf(num_str, "%d", num);
						strncat(attempt, num_str, 1);
						
						attempt_input_count++;
					}

					if (locked_count == 10) {
						beep(2);
						locked_count = 0;
					}

					avr_wait(500);
					locked_count++;
				}
				
				// Input now 4 digits
				int current_attempt = atoi(attempt);
				
				if (current_attempt == current_pass) {
					avr_wait(500);
					beep(unlocked);
					
					prev_pass[pass_index] = current_pass;
					pass_index++;
					current_pass = -1;
					
					is_locked = 0;
					avr_wait(500);
				} else {
					avr_wait(500);
					beep(incorrect);
					current_attempts++;
				}
			}
		}
    }
}