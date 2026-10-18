
#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define COILA (1 << PORTC0)
#define COILB (1 << PORTC1)
#define COILC (1 << PORTC2)
#define COILD (1 << PORTC3)

const uint8_t coilsToPower[4] = {COILA | COILB, COILB | COILC, COILC | COILD, COILD| COILA};
volatile uint8_t it = 0; // так как 128 делится на 4 то по идее проблем из-за overflow не должно быть (127) % 4 = 3, 0 % 4 = 0
volatile uint8_t rpm = 10;
//x об/мин, нужно (3.33x) импульсов/секунду, соответственно 3.33x = (11059200 /256)*OCR3A
#define OCR3A_VAL_ON_SPEED(x) (12960 / x)
ISR (TIMER3_COMPA_vect){
	PORTC &= ~coilsToPower[it %4];
	PORTC |= coilsToPower[(++it) %4];
	OCR3A = OCR3A_VAL_ON_SPEED(rpm);
}

void controlMode(){
	while(rpm < 100){
		rpm++;
		_delay_ms(100);
	}
	_delay_ms(30000);
	while(rpm > 10){
		rpm--;
		_delay_ms(100);
	}
	_delay_ms(30000);

}

int main(void){
	DDRC = 0x0F;
	TCCR3B |= (1 <<WGM32);
	TCCR3B |= (1 << CS32);
	OCR3A = 1296;
	ETIMSK |= (1 << OCIE3A);
	sei();
	for(;;){
		controlMode();
	}

}
