#include "avr.h"

typedef enum {
	A, As, B, C, D, Ds, E, F, Fs, G, Gs
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
	for (i = 0; i < k; i++) {
		SET_BIT(PORTA, 0);
		avr_wait()
	}
}


PlayingNote shooting_stars[] = {
	{D, Wn},
	{D, Hn},
	{E, Hn},
	{B, Qn},
	{Gs, Qn}
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