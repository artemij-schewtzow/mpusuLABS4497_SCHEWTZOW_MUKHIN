/*
 *@file Lab2 
 *@brief Prints САУ letter by letter each time inverting the letter
 * 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>



//To set a column x as an active you need to set DDRD = COLx

#define COL1  (0x00 << PORTD4)
#define COL2  (0x01 << PORTD4)
#define COL3  (0x02 << PORTD4)
#define COL4  (0x03 << PORTD4)
#define COL5  (0x04 << PORTD4)
#define COL6  (0x05 << PORTD4)
#define COL7  (0x06 << PORTD4)
#define COL8  (0x07 << PORTD4)
#define COL9  (0x08 << PORTD4)
#define COL10 (0x09 << PORTD4)
#define COL11 (0x0A << PORTD4)
#define COL12 (0x0B << PORTD4)
#define COL13 (0x0C << PORTD4)
#define COL14 (0x0D << PORTD4)
#define COL15 (0x0E << PORTD4)
#define COL16 (0x0F << PORTD4)

uint8_t is_inverted = 0;
uint16_t invert_timer = 0;
void inline draw(){
	PORTD = COL1;
	PORTA = 0x30;
	PORTC = 0x30;
	
	PORTD = COL2;
	PORTA = 0x30;
	PORTC = 0x30;
	
	PORTD = COL3;
	PORTA = 0x30;
	PORTC = 0xFF;
	
	PORTD = COL4;
	PORTA = 0x30;
	PORTC = 0xFF;
	
	PORTD = COL5;
	PORTA = 0xFF;
	PORTC = 0x30;
	
	PORTD = COL6;
	PORTA = 0xFF;
	PORTC = 0x30;
	
	PORTD = COL7;
	PORTA = 0x30;
	PORTC = 0x30;
	
	PORTD = COL8;
	PORTA = 0x30;
	PORTC = 0x30;
	
	PORTD = COL9;
	PORTA = 0x30;
	PORTC = 0x30;
	
	PORTD = COL10;
	PORTA = 0x30;
	PORTC = 0x30;
	
	PORTD = COL11;
	PORTA = 0x30;
	PORTC = 0xFF;
	
	PORTD = COL12;
	PORTA = 0x30;
	PORTC = 0xFF;
	
	PORTD = COL13;
	PORTA = 0xFF;
	PORTC = 0x30;
	
	PORTD = COL14;
	PORTA = 0xFF;
	PORTC = 0x30;
	
	PORTD = COL15;
	PORTA = 0x30;
	PORTC = 0x30;
	
	PORTD = COL16;
	PORTA = 0x30;
	PORTC = 0x30;
	
}
void inline drawInverted(){
	PORTD = COL1;
	PORTA = ~0x30;
	PORTC = ~0x30;
	
	PORTD = COL2;
	PORTA = ~0x30;
	PORTC = ~0x30;
	
	PORTD = COL3;
	PORTA = ~0x30;
	PORTC = ~0xFF;
	
	PORTD = COL4;
	PORTA = ~0x30;
	PORTC = ~0xFF;
	
	PORTD = COL5;
	PORTA = ~0xFF;
	PORTC = ~0x30;
	
	PORTD = COL6;
	PORTA = ~0xFF;
	PORTC = ~0x30;
	
	PORTD = COL7;
	PORTA = ~0x30;
	PORTC = ~0x30;
	
	PORTD = COL8;
	PORTA = ~0x30;
	PORTC = ~0x30;
	
	PORTD = COL9;
	PORTA = ~0x30;
	PORTC = ~0x30;
	
	PORTD = COL10;
	PORTA = ~0x30;
	PORTC = ~0x30;
	
	PORTD = COL11;
	PORTA = ~0x30;
	PORTC = ~0xFF;
	
	PORTD = COL12;
	PORTA = ~0x30;
	PORTC = ~0xFF;
	
	PORTD = COL13;
	PORTA = ~0xFF;
	PORTC = ~0x30;
	
	PORTD = COL14;
	PORTA = ~0xFF;
	PORTC = ~0x30;
	
	PORTD = COL15;
	PORTA = ~0x30;
	PORTC = ~0x30;
	
	PORTD = COL16;
	PORTA = ~0x30;
	PORTC = ~0x30;
}
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
