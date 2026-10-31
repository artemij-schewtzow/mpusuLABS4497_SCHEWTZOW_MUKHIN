/*
 * @file main.c
 * @brief Первая лабораторная работа, тема - регистры PORTx, DDRx, 
 * @author Шевцов А.И. Мухин А.П.
 * @date 2026-10-31
 */

#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>

//codes for segments
uint8_t segments[11] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x40};

/*
 * @brief returns value of a given digit of the given number
 * @param d - number
 * @param m - digit
 * @note function from tbook
 */
uint8_t digit(uint16_t d, uint8_t m) {
	uint8_t i = 5, a;
	while(i){ 
		a = d%10; 
		if(i-- == m) break; 
		d /= 10; 
	}
	return(a);
}

/*
 * @brief return total amount of digits of a given number
 * @param val - number
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

/*
 * @brief output '-' on all displays
 */
inline void drawError(){
	for (uint8_t i = 1 ; i <= 5 ; i++){
		PORTC = 0; 
		PORTC = segments[10]; 
		PORTA |= (1 << i);
		_delay_us(2);
		PORTA &= ~(1 << i);
	}
}
/*
 * @brief draw a number on displays
 * @param val - number
 * @param segs - amount of displays required
 */
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
			PORTC = 0; 
			PORTC = segments[digit(val, i)]; 
			PORTA |= (1 << i);
			_delay_us(2);
			PORTA &= ~(1 << i);
		}
	}
	else{
		for (uint8_t i = 6 - segs ; i <= 5 ; i++){
			PORTC = 0; 
			PORTC = segments[digit(val, i)]; 
			PORTA |= (1 << i);
			_delay_us(2);
			PORTA &= ~(1 << i);
		}
	}
}

/*
 * @brief wrapper function for draw() and drawError()
 * @param to_draw - the number to draw
 */
inline void drawNumber(int16_t to_draw){
	uint8_t num_digits = countDigits(to_draw);
	if (to_draw < 0)
		num_digits++;
	if (num_digits > 5)
		drawError();
	else
		draw(to_draw, num_digits);
}

int main(void){
	//set A and C to output
	DDRC = 0xFF;
	DDRA = (1 << PORTA1)|
	       (1 << PORTA2)|
	       (1 << PORTA3)|
	       (1 << PORTA4)|
	       (1 << PORTA5);
	//clear PORTA
	PORTA = 0;			
	for(;;){ 
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
}
