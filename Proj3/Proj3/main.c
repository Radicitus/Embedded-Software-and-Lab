#include "avr.h"
#include "lcd.h"

typedef enum {
	A, As, B, C, Cs, D, Ds, E, F, Fs, G, Gs
} Note;

typedef enum {
	Wn, Hn, Qn, En
} Duration;

typedef struct {
	Note note;
	Duration duration;
} PlayingNote;


void play_song(const PlayingNote song[], int length) {
	int i;
	for (i = 0; i < length; i++) {
		play_note(&song[i]);
	}
}

void play_note(const PlayingNote* note) {
	int i, k;
	int th;
	double duration = note->duration;
	
	switch(note->duration){
		case Wn:
			duration = 4;
			break;
		case Hn:
			duration = 2;
			break;
		case Qn:
			duration = 1;
			break;
		case En:
			duration = .5;
			break;
	}
	
	switch(note->note){
		case A:
			th = 11;
			k = duration * 220;
			break;
		case As:
			th = 21;
			k = duration * 240;
			break;
		case B:
			th = 10;
			k = duration * 245;
			break;
		case C:
			th = 19;
			k = duration * 262;
			break;
		case Cs:
			th = 18;
			k = duration * 277;
			break;
		case D:
			th = 17;
			k = duration * 294;
			break;
		case Ds:
			th = 16;
			k = duration * 311;
			break;
		case E:
			th = 15;
			k = duration * 330;
			break;
		case F:
			th = 28;
			k = duration * 349;
			break;
		case Fs:
			th = 14;
			k = duration * 370;
			break;
		case G:
			th = 12;
			k = duration * 392;
			break;
		case Gs:
			th = 12;
			k = duration * 415;
			break;
	}

	for (i = 0; i < k; i++) {
		SET_BIT(PORTA, 0);
		avr_wait(th);
		CLR_BIT(PORTA, 0);
		avr_wait(th);
	}
}

PlayingNote test1[] = {
	{A, Hn},
	{As, Hn},
	{F, Hn},
	{Gs, Hn},
	{Fs, Hn},
};

PlayingNote fur_elise[] = {
	{A, Qn},
	{Gs, Qn},
		
	{A, Qn},
	{Gs, Qn},
		
	{As, Qn},
	{E, Hn},
		
	{E, Qn},
	{G, Wn},
	{F, Qn},
	{D, Wn}
};

PlayingNote hot_cross_buns[] = {
	{B, Hn},
	{A, Hn},
	{G, Wn},
	{B, Hn},
	{A, Hn},
	{G, Hn},
	{G, Qn},
	{G, Qn},
	{G, Qn},
	{G, Qn},
	{A, Qn},
	{A, Qn},
	{A, Qn},
	{A, Qn},
	{B, Hn},
	{A, Hn},
	{G, Wn}
};

main() {
	// Set button input
	CLR_BIT(DDRA, 1);
	// Set LED output
	SET_BIT(DDRB, 0);
	// Set speaker output
	SET_BIT(DDRA, 0);
	// Print Song Title
	lcd_init();
	lcd_pos(0, 0);
	char hcb[15] = "Hot Cross Buns";
	lcd_puts2(hcb);
	
    while(1)
    {
		play_song(hot_cross_buns, 17);
		avr_wait(1000);
		
		/*
		// While button pushed
        if(GET_BIT(PINA, 1)) {
			SET_BIT(PORTB, 0);
			play_song(shooting_stars, 7);
		} else {
			CLR_BIT(PORTB, 0);	
		}
		*/
	}
}