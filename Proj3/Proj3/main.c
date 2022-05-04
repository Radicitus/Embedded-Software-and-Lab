#include "avr.h"

typedef enum {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
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
	
	double f = (2^( note->note / 12) * 220);
	
	float p = 1 / f;
	float k = note->duration / p;
	float th = p /2;
	
	for (i = 0; i < k; i++) {
		SET_BIT(PORTA, 0);
		avr_wait(th);
		CLR_BIT(PORTA, 0);
		avr_wait(th);
	}
}


PlayingNote shooting_stars[] = {
	{5, Wn},
	{5, Hn},
	{7, Hn},
	{2, Qn},
	{11, Qn}
};

int main(void)
{
	// Set button input
	CLR_BIT(DDRA, 1);
	// Set LED output
	SET_BIT(DDRB, 0);
	// Set speaker output
	SET_BIT(DDRA, 0);
	
    while(1)
    {
		// While button pushed
        if(!GET_BIT(PINA, 1)) {
			SET_BIT(PORTB, 0);
		} else {
			CLR_BIT(PORTB, 0);	
		}
    }
}