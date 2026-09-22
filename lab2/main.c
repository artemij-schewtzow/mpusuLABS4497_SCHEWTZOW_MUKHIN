/*
 *@file Lab2 
 *@brief Prints САУ letter by letter each time inverting the letter
 * 
 */
#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>



//To set a column x as an active you need to set DDRD = COLx
#define COL1 (uint8_t) (1 << PORTD4)
#define COL2 (uint8_t) (2 << PORTD4)
#define COL3 (uint8_t) (3 << PORTD4)
#define COL4 (uint8_t) (4 << PORTD4)
#define COL5 (uint8_t) (5 << PORTD4)
#define COL6 (uint8_t) (6 << PORTD4)
#define COL7 (uint8_t) (7 << PORTD4)
#define COL8 (uint8_t) (8 << PORTD4)

/*@brief inverse whatever is on display
 * @note I hope it IS done via ^= 0xFF because I`m too lazy to test or even ask ChatGPT about it
 */
void inline inverseScreen(){
	_delay_us(2);
	DDRD = COL1;
	DDRA ^= 0xFF;
	_delay_us(2);
	DDRD = COL2;
	DDRA ^= 0xFF;
	_delay_us(2);
	DDRD = COL3;
	DDRA ^= 0xFF;
	_delay_us(2);
	DDRD = COL4;
	DDRA ^= 0xFF;
	_delay_us(2);
	DDRD = COL5;
	DDRA ^= 0xFF;
	_delay_us(2);
	DDRD = COL6;
	DDRA ^= 0xFF;
	_delay_us(2);
	DDRD = COL7;
	DDRA ^= 0xFF;
	_delay_us(2);
	DDRD = COL8;
	DDRA ^= 0xFF;
	_delay_us(2);
}

/*
 *@brief Draw letter C on the screen
 * ++++++++
 * +.......
 * +.......
 * +.......
 * +.......
 * +.......
 * +.......
 * ++++++++
 * @note I am not sure that delays are needed, but since there are transistors in the scheme I suppose they do
 * */
void inline drawLetC(){
	_delay_us(2);
	DDRD = COL1;
	DDRA = 0xFF;
	_delay_us(2);
	DDRD = COL2;
	DDRA = 0x81;
	_delay_us(2);
	DDRD = COL3;
	DDRA = 0x81;
	_delay_us(2);
	DDRD = COL4;
	DDRA = 0x81;
	_delay_us(2);
	DDRD = COL5;
	DDRA = 0x81;
	_delay_us(2);
	DDRD = COL6;
	DDRA = 0x81;
	_delay_us(2);
	DDRD = COL7;
	DDRA = 0x81;
	_delay_us(2);
	DDRD = COL8;
	DDRA = 0x81;
	_delay_us(2);
}

/*
 *@brief Draw letter A on the screen
 * ...++...
 * ..++++..
 * .+....+.
 * +......+
 * +......+
 * ++++++++
 * +......+
 * +......+
 *
 * */
void inline drawletA(){
	_delay_us(2);
	DDRD = COL1;
	DDRA = 0x1F;
	_delay_us(2);
	DDRD = COL2;
	DDRA = 0x24;
	_delay_us(2);
	DDRD = COL3;
	DDRA = 0x44;
	_delay_us(2);
	DDRD = COL4;
	DDRA = 0xC4;
	_delay_us(2);
	DDRD = COL5;
	DDRA = 0xC4;
	_delay_us(2);
	DDRD = COL6;
	DDRA = 0x44;
	_delay_us(2);
	DDRD = COL7;
	DDRA = 0x24;
	_delay_us(2);
	DDRD = COL8;
	DDRA = 0x1F;
	_delay_us(2);
}

/*
 *@brief Draw letter Y on the screen
 * +......+
 * +.....+.
 * .+...+..
 * ..+.+...
 * ...+....
 * ..+.....
 * .+......
 * +....... 
 * */
void inline drawletY(){

	_delay_us(2);
	DDRD = COL1;
	DDRA = 0xC1;
	_delay_us(2);
	DDRD = COL2;
	DDRA = 0x22;
	_delay_us(2);
	DDRD = COL3;
	DDRA = 0x14;
	_delay_us(2);
	DDRD = COL4;
	DDRA = 0x8;
	_delay_us(2);
	DDRD = COL5;
	DDRA = 0x10;
	_delay_us(2);
	DDRD = COL6;
	DDRA = 0x20;
	_delay_us(2);
	DDRD = COL7;
	DDRA = 0x40;
	_delay_us(2);
	DDRD = COL8;
	DDRA = 0x80;
	_delay_us(2);
}


int main(void){
	//init
	DDRA = 0xFF;
	DDRD = 0xF0;

	for(;;){
		drawLetC();
		_delay_ms(3000);
		inverseScreen();
		_delay_ms(3000);
		drawletA();
		_delay_ms(3000);
		inverseScreen();
		_delay_ms(3000);
		drawletY();
		_delay_ms(3000);
		inverseScreen();
		_delay_ms(3000);
	}

}
