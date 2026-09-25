/*
 * GccApplication3.c
 *
 * Created: 14.09.2026 10:44:16
 * Author : student
 */ 

#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>
//ctrl c + ctrl v сметодички
uint8_t digit(uint16_t d, uint8_t m) {
	uint8_t i = 5, a;
	while(i){ // цикл по разрядам числа
		a = d%10; // выделяем очередной разряд
		if(i-- == m) break; // выделен ли желаемый разряд
		d /= 10; // уменьшаем число в 10 раз
	}
	return(a);
}
//init segments
uint8_t segments[11] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x40};
/* 
segments[0] = 0x3F;
segments[1] = 0x06;
segments[2] = 0x5B;
segments[3] = 0x4F;
segments[4] = 0x66;
segments[5] = 0x6D;
segments[6] = 0x7D;
segments[7] = 0x07;
segments[8] = 0xFF;
segments[9] = 0xEF;
segments[10] = -;
*/
inline uint8_t countDigits(int16_t val){
	if (val == 0) 
		return 1;
	uint8_t ret = 0;
	while (val != 0){
		val /= 10;
		ret++;
	}
	return ret;
}

inline void drawError(){
	for (uint8_t i = 1 ; i <= 5 ; i++){
		PORTC = 0; //clear PORTC
		PORTC = segments[10]; // put a digit to draw in portc
		PORTA |= (1 << i);
		_delay_us(2);
		PORTA &= ~(1 << i);
	}
}//-456 4

inline void draw(int16_t val, uint8_t segs){
	for (uint8_t i = 1; i <= 5 - segs; i++){
		PORTC = 0;
		PORTA |= (1 << i);
		_delay_us(2);
		PORTA &= ~(1 << i);
	}
	if (val < 0){
	//draw minus sign
		PORTC = 0;
		PORTC = segments[10];
		PORTA |= (1 << (6 - segs));
		_delay_us(2);
		PORTA &= ~(1 << (6 - segs));
	//draw the number itself	
		val = -val;
		for (uint8_t i = 7 - segs ; i <= 5 ; i++){
			PORTC = 0; //clear PORTC
			PORTC = segments[digit(val, i)]; // put a digit to draw in portc
			PORTA |= (1 << i);
			_delay_us(2);
			PORTA &= ~(1 << i);
		}
	}
	else{
		for (uint8_t i = 6 - segs ; i <= 5 ; i++){
			PORTC = 0; //clear PORTC
			PORTC = segments[digit(val, i)]; // put a digit to draw in portc
			PORTA |= (1 << i);
			_delay_us(2);
			PORTA &= ~(1 << i);
		}
	}
}

inline void drawNumber(int16_t to_draw){
	uint8_t num_digits = countDigits(to_draw);
	if (to_draw < 0)
		num_digits++;
		
	if (num_digits > 5){
			drawError();
	}
	else{
		/*
			if (to_draw < 0)
				drawNegative(to_draw, num_digits);
			else
				drawPositive(to_draw, num_digits);*/
		draw(to_draw, num_digits);
	}
		/*
	for (uint8_t i = 5 - num_digits ; i <= 5 ; i++){
		PORTC = 0; //clear PORTC
		PORTC = segments[digit(to_draw, i)]; // put a digit to draw in portc
		PORTA |= (1 << i);
		_delay_us(2);
		PORTA &= ~(1 << i);
	}*/
	
}
int main(void)
{
	//set A and C to output
    DDRC = 0xFF;
	DDRA = (1 << PORTA1) | (1 << PORTA2) | (1 << PORTA3) | (1 << PORTA4) | (1 << PORTA5);
	//clear PORTA
	PORTA = 0;			
    while (1) 
    { 
		PORTA = 0;
		drawNumber(17345);
		_delay_ms(3000);
		PORTA = 0;
		drawNumber(906);
		_delay_ms(3000);
		PORTA = 0;
		drawNumber(0);
		_delay_ms(3000);
		PORTA = 0;
		drawNumber(-32000);
		_delay_ms(3000);
		PORTA = 0;
		drawNumber(-456);
		_delay_ms(3000);
		PORTA = 0;
		drawNumber(-1234);
		_delay_ms(3000);
		PORTA = 0;
		
    }
	
	//return(0);
}

