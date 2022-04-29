#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <stdio.h>

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

// Date Time

typedef struct {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	} DateTime;

void advance_dt(DateTime *dt) {
	++dt->second;
	
	// Second to Minute
	if (dt->second == 60) {
		dt->second = 0;
		++dt->minute;
	}
	
	// Minute to Hour
	if (dt->minute == 60) {
		dt->minute = 0;
		++dt->hour;
	}
	
	// Hour to Day
	if (dt->hour == 24) {
		dt->hour = 0;
		++dt->day;
	}
	
	// Day to Month
	int mDays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	if (dt->day > mDays[dt->month]) {
		dt->day = 1;
		++dt->month;
	}
	
	// Month to Year
	if (dt->month > 12) {
		dt->month = 1;
		++dt->year;
	}
}

void init_dt(DateTime *dt) {
	dt->year = 2022;
	dt->month = 12;
	dt->day = 31;
	dt->hour = 23;
	dt->minute = 59;
	dt->second = 50;
}

void print_dt(const DateTime *dt) {
	char date[17];
	// Print date on top row
	lcd_pos(0,0);
	sprintf(date, "%04d-%02d-%02d", dt->year, 
								    dt->month, 
								    dt->day);
	lcd_puts2(date);
	
	// Print time on bottom row
	char time[9];
	lcd_pos(1,0);
	sprintf(time, "%02d:%02d:%02d", dt->hour,
								    dt->minute,
								    dt->second);
	lcd_puts2(time);
}

// MAIN LOOP

int main(void)
{
    // Set write to PB0
    SET_BIT(DDRB, 0);
    
	// Init display
	DateTime dt;
	init_dt(&dt);
	lcd_init();

    while (1) {
		
		avr_wait(1000);
		advance_dt(&dt);
		print_dt(&dt);
		
		
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