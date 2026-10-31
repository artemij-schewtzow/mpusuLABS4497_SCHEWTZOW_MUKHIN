/*
 * @file main.c
 * @brief Четвертая лабораторная работа: темы - не особо понимаю если честно, CTC режим таймера наверное 
 * @author Шевцов А.И. Мухин А.П.
 * @date 2026-10-31
 */

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
volatile uint8_t it = 0; 
volatile uint8_t rpm = 10;
//x об/мин, нужно (3.33x) импульсов/секунду, соответственно 3.33x = (11059200 /256)*OCR3A
//Probably more accurate method would be to precalculate all requires values during initialisation, so you don`t have to deal with d*vision 
#define OCR3A_VAL_ON_SPEED(x) (12960 / x)

/*
 * @brief switch coils
 */
ISR (TIMER3_COMPA_vect){
	PORTC &= ~coilsToPower[it %4];
	PORTC |= coilsToPower[(++it) %4];
	OCR3A = OCR3A_VAL_ON_SPEED(rpm);
}
/*
 * @brief control mode for the stepper motor
 */
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
