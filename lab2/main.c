#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>

uint8_t is_inverted = 0;
uint16_t invert_timer = 0;
const uint8_t to_drawA[16] = {0x30, 0x30, 0x30, 0x30, 0xFF, 0xFF,0x30, 0x30,0x30, 0x30,0x30, 0x30, 0xFF, 0xFF, 0x30, 0x30};
const uint8_t to_drawC[16] = {0x30, 0x30, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0xFF, 0xFF, 0x30, 0x30, 0x30, 0x30};
volatile uint8_t drawIt = 0;
ISR (TIMER3_OVF_vect){
		invert_timer++;
		if (invert_timer >= 2000){
			is_inverted ^= 1;
			invert_timer = 0;
		}
	
		if (is_inverted){
			PORTD = ( (drawIt % 16) << PORTD4);
			PORTA = ~to_drawA[drawIt % 16];
			PORTC = ~to_drawC[drawIt % 16];
			drawIt++;
		}
		else{
			PORTD = ( (drawIt % 16) << PORTD4);
			PORTA = to_drawA[drawIt % 16];
			PORTC = to_drawC[drawIt % 16];
			drawIt++;
		}
		TCNT3 = 50000;
}
int main(void){
	//init
	DDRA = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xF0;
	TCCR3B |=  (1 <<CS30); 
	ETIMSK |= (1 << TOIE3);
	TCNT3 = 50000;
	sei();
	for(;;){
		
	}
}
