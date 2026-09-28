
#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define COILA (1 << PORTC0)
#define COILB (1 << PORTC1)
#define COILC (1 << PORTC2)
#define COILD (1 << PORTC3)

#define MIN_SPEED_CONTROL_IMPULSE(x)\
	do{\
		PORTC |= x;\
		_delay_ms(30);\
		PORTC &= ~x;\
	}while(0);\

#define MAX_SPEED_CONTROL_IMPULSE(x)\
	do{\
		PORTC |= x;\
		_delay_ms(3);\
		PORTC &= ~x;\
	}while(0);\

const uint8_t coilsToPower[4] = {COILA | COILB, COILB | COILC, COILC | COILD, COILD| COILA};
volatile uint8_t it = 0; // так как 128 делится на 4 то по идее проблем из-за overflow не должно быть (127) % 4 = 3, 0 % 4 = 0

ISR (TIMER3_COMPA_vect){
	PORTC &= ~coilsToPower[it %4];
	PORTC |= coilsToPower[(++it) %4];
}

inline void increaseToMaxSpeed(){
	if (OCR3A == 130) return;
	cli();
	OCR3A = OCR3A - 1;
	sei();
	_delay_ms(10);
}

inline void decreaseToMinimalSpeed(){
	if (OCR3A == 1296) return;
	cli();
	OCR3A = OCR3A + 1;
	sei();
	_delay_ms(10);
}

void controlMode(){
	while(OCR3A != 130){
		increaseToMaxSpeed();
	}
	_delay_ms(10000);
	while(OCR3A != 1296){
		decreaseToMinimalSpeed();
	}
	_delay_ms(10000);
}

int main(void){
	DDRC = 0x0F;
	TCCR3B |= (1 <<WGM32);
	TCCR3B |= (1 << CS32);/*
	OCR3AH = (uint8_t) (1296 >> 8);
      	OCR3AL = (uint8_t) (1296 & 0xFF);
	*/
	OCR3A = 1296;
	ETIMSK |= (1 << OCIE3A);
	sei();
	//мин скорость 10 оборотов в минуту длина импульса - 0.03 
	//макс скорость 100 оборотов в минуту длина импульса - 0.003
	//
	for(;;){
		controlMode();
	}

}
