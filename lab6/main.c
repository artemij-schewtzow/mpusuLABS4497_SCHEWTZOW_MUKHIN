#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define GREEN (1 << PORTE4)
#define LBLUE (1 << PORTE3) | (1 << PORTE4)

void inline sound(){

}

uint16_t readAdcChannel(uint8_t channel){
	ADMUX = (1 <<REFS0) | channel;
	ADCSRA |= (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF)));
	ADCSRA |= (1 << ADIF);
	uint16_t low = ADCL;
	uint16_t high = ADCH << 8;
	return low | high;
}

int main(void){
	DDRE |= (1 << DDRE3) | (1 << DDRE4) | (1 << DDRE5);
	DDRB |= (1 << PORTB4);
	ADCSRA |= (1 << ADEN);
	ADMUX &= ~(1 << ADLAR);
	

}
