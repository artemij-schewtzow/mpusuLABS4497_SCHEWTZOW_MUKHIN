/*
 * @file lab3 ВАРИАНТ 9 двухфазный полношаговый
 * 
 *
 */
#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>

//to power up a coilx you have to PORTC |= COILx
#define COILA (uint8_t) (1 << PORTC0)
#define COILB (uint8_t) (1 << PORTC1)
#define COILC (uint8_t) (1 << PORTC2)
#define COILD (uint8_t) (1 << PORTC3)
/*
 * @brief sends control impulse of given duration to the given coils
 * @param dur duration of the signal
 * @param coil code 
 * @note I assume that in REAL practice writing such a function would be a bad practice, since it relies on the extern things (PORTCx macros) but for this lab i think it`s good enough
 * */
void inline controlPulse(uint16_t dur, uint8_t coil){
	PORTC |= coil;
	_delay_ms(dur);
	PORTC &= ~coil;
}
/*
 * @brief speeds up to sum speed 
 * @param rpm rotates per minute till which will the rotor speed up
 */
void inline rotateSpeedDown(uint8_t rpm){
	const uint16_t Delay = 300 / rpm;
	for (uint16_t i = 1; i <= Delay; i++){
		controlPulse(i, COILA | COILB);
		controlPulse(i, COILB | COILC);
		controlPulse(i, COILC | COILD);
		controlPulse(i, COILD | COILA);
	}
}
/*
 * @brief slows down from sum speed till 0
 * @param supposed speed im rpm at the start
 */
void inline rotateSpeedUp(uint8_t rpm){
	uint16_t Delay = 300 / rpm;
	for (uint16_t i = Delay; i > 0; i--){	
		controlPulse(i, COILA | COILB);
		controlPulse(i, COILB | COILC);
		controlPulse(i, COILC | COILD);
		controlPulse(i, COILD | COILA);
	}
}


int main(void){
//init
	DDRC = 0x0F;
	
	for (;; ){
		rotateSpeedUp(100);
		_delay_ms(2000);
		rotateSpeedDown(100);
		_delay_ms(5000);
	}
}
