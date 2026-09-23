
#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>

#define SOUND_PERIOD_US (1110)
#define LED_BLUE (1 << PORTE3)
#define LED_PURPLE (1 << PORTE3)| (1 << PORTE5)
#define DOT_LENGTH_MS 500
#define DASH_LENGTH_MS 1500
#define SYMBOL_INTERVAL_LENGTH_MS 500
#define LETTER_INTERVAL_LENGTH_MS 1000

void inline sound(uint16_t length_ms){
	for (uint16_t i = 0 ; i < length_ms ; i++ ){
		PORTB |= (1 << PORTB4);
		_delay_us( SOUND_PERIOD_US >> 1);
		PORTB &= ~ (1 << PORTB4);
		_delay_us(SOUND_PERIOD_US >> 1);
	}
}
void inline dot(){
	PORTE |= LED_PURPLE;
	sound(DOT_LENGTH_MS);
	PORTE &= ~LED_PURPLE;
}

void inline dash(){
	PORTE |= LED_BLUE;
	sound( DASH_LENGTH_MS );
	PORTE &= ~LED_BLUE;
}
// I am NOT going to write @brief for them all, they all just signal the letter
void inline sigLetV(){
	dot();
	_delay_ms( SYMBOL_INTERVAL_LENGTH_MS );
	dot();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dash();
}
void inline sigLetE(){
	dot();
}
void inline sigLetI(){
	dot();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dot();
}
void inline sigLetM(){
	dash();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dash();
}
void inline sigLetN(){
	dash();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dot();
}
void inline sigLetO(){
	dash();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dash();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dash();
}
void inline sigLetU(){
	dot();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dot();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dash();
}
void inline sigLetH(){
	dot();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dot();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dot();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dot();
}
void inline sigLetTZ(){
	dash();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dot();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dash();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dot();
}
void sigLetSCH(){
	dash();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dash();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dash();
	_delay_ms(SYMBOL_INTERVAL_LENGTH_MS);
	dash();
}

void inline sigSurnameSchewtzow(){
		sigLetSCH();
		_delay_ms(LETTER_INTERVAL_LENGTH_MS);
		sigLetE();
		_delay_ms(LETTER_INTERVAL_LENGTH_MS);
		sigLetV();
		_delay_ms(LETTER_INTERVAL_LENGTH_MS);
		sigLetTZ();
		_delay_ms(LETTER_INTERVAL_LENGTH_MS);
		sigLetO();
		_delay_ms(LETTER_INTERVAL_LENGTH_MS);
		sigLetV();
		_delay_ms(LETTER_INTERVAL_LENGTH_MS);
}

void inline sigSurnameMuhin(){
	sigLetM();
	_delay_ms(LETTER_INTERVAL_LENGTH_MS);
	sigLetU();
	_delay_ms(LETTER_INTERVAL_LENGTH_MS);
	sigLetH();
	_delay_ms(LETTER_INTERVAL_LENGTH_MS);
	sigLetI();
	_delay_ms(LETTER_INTERVAL_LENGTH_MS);
	sigLetN();
	_delay_ms(LETTER_INTERVAL_LENGTH_MS);
}

int main(void){
//init
	DDRB = (1 << DDRB4);
	DDRE = (1 << DDRE3) | (1 << DDRE4) | (1 << DDRE5);	
	for ( ; ; ){
		_delay_ms(2000);
		sigSurnameSchewtzow();
		_delay_ms(1000);
		sigSurnameMuhin();
		_delay_ms(2000);
	}
}
