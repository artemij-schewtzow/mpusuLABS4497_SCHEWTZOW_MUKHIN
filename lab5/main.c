
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define RS 7
#define E 6
//commands
#define CLEAR_AND_SET_CURSOR_0 		   	(0x01)
#define SET_CURSOR_0 			   	(0x02)
#define SET_CURSOR_DIRECTON_LEFT_NOSHIFT   	(0x04)
#define SET_CURSOR_DIRECTION_LEFT_SHIFT    	(0x05)
#define SET_CURSOR_DIRECTION_RIGHT_NOSHIFT 	(0x06)
#define SET_CURSOR_DIRECTION_RIGHT_SHIFT    	(0x07)
#define SET_INDICATOR_OFF_CURSOR_OFF_NOBLINK 	(0x08)
#define SET_INDICATOR_OFF_CURSOR_OFF_BLINK    	(0x09)
#define SET_INDICATOR_OF_CURSOR_ON_NOBLINK 	(0x0A)
#define SET_INDICATOR_OFF_CURSOR_ON_BLINK 	(0x0B)
#define SET_INDICATOR_ON_CURSOR_OFF_NOBLINK 	(0x0C)
#define SET_INDICATOR_ON_CURSOR_OFF_BLINK 	(0x0D)
#define SET_INDICATOR_ON_CURSOR_ON_NOBLINK 	(0x0E)
#define SET_INDICATOR_ON_CURSOR_ON_BLINK 	(0x0F)
#define MOVE_CURSOR_LEFT 			(0x10)
#define MOVE_CURSOR_RIGHT 			(0x14)
#define MOVE_SCREEN_LEFT 			(0x18)
#define MOVE_SCREEN_RIGHT 			(0x1C)
#define SET_DBUS_WIDTH_4_1_LINE_5_X_7_FONT 	(0x20)
#define SET_DBUS_WIDTH_4_2_LINE_5_X_7_FONT 	(0x28)
#define SET_DBUS_WIDTH_4_1_LINE_5_X_10_FONT 	(0x24)
#define SET_DBUS_WIDTH_4_2_LINE_5_X_10_FONT 	(0x2C)
#define SET_DBUS_WIDTH_8_1_LINE_5_X_7_FONT 	(0x30)
#define SET_DBUS_WIDTH_8_2_LINE_5_X_7_FONT 	(0x38)
#define SET_DBUS_WIDTH_8_1_LINE_5_X_10_FONT 	(0x34)
#define SET_DBUS_WIDTH_8_2_LINE_5_X_10_FONT 	(0x3C)
#define SET_SGRAM_ADRESS(ag) 			(0x40 | ag )
#define SET_DDRAM_ADRESS(ad) 			(0x80 | ad )

#define FIRST_LINE_ADRESS (0x00)
#define SECOND_LINE_ADRESS  (0x40)

#define HASH 0x23
#define STAR 0x2A

uint8_t screen_iterator = 0;
#define SCREEN_ITERATOR (SECOND_LINE_ADRESS + (screen_iterator++) % 0x28)

uint8_t tabCon[] ={0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,
0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,0x50,0x43,0x54,0xA9,
0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,
0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,
0xBC,0xBD,0x6F,0xBE,0x70,0x63,0xBF,0x79,0x5C,0x78,0xE5,0xC0,
0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7};


void lcdData(uint8_t data){
	DDRC = 0xFF;
	DDRD |= (1 << E) | (1 << RS);
	PORTD |= ( 1 << RS );
	PORTC = data;
	PORTD |= ( 1 << E );
	_delay_us(5);
	PORTD &= ~(1 << E);
	_delay_ms(20); 
}

void lcdCmd(uint8_t cmd) {
DDRC = 0xFF;
DDRD |= (1 << E) | (1 << RS);
PORTD &= ~(1<<RS);
PORTC = cmd;
PORTD |= (1 << E);
_delay_us(5);
PORTD &= ~(1 << E);
_delay_ms(5);
}


void lcdInit(void) {
DDRC = 0xFF;
lcdCmd(SET_DBUS_WIDTH_8_1_LINE_5_X_7_FONT );
lcdCmd(SET_DBUS_WIDTH_8_1_LINE_5_X_7_FONT );
lcdCmd(SET_DBUS_WIDTH_8_1_LINE_5_X_7_FONT );
lcdCmd(SET_DBUS_WIDTH_8_2_LINE_5_X_7_FONT );
lcdCmd(SET_INDICATOR_OF_CURSOR_ON_NOBLINK );
lcdCmd(SET_CURSOR_DIRECTION_RIGHT_NOSHIFT );
lcdCmd(CLEAR_AND_SET_CURSOR_0 );
}

uint8_t code(uint8_t symb){
	return (symb >= 192)?tabCon[symb - 192] : symb;
}

void writeSymbol(const uint8_t symb, const uint8_t adress){
	lcdCmd(SET_DDRAM_ADRESS(adress));
	lcdCmd(MOVE_CURSOR_RIGHT);
	lcdData(code(symb));
}
void writeString(const char* str, const uint8_t len, uint8_t start_adress){
	for (uint8_t i = 0; i < len; i++){
		writeSymbol(str[i], start_adress + i);
	}
}


const uint8_t rows[4] = {1 << PINA0, 1 << PINA1, 1 << PINA2, 1 << PINA3};
const uint8_t cols[3] = { 1 << PORTD0, 1 << PORTD1, 1 << PORTD2 };
const uint8_t map[4][3] = { {3,2,1}, 
	   		{6, 5, 4}, 
			{9,8,7}, 
			{HASH, 0, STAR} };

uint8_t getKey(){
	for(;;){
		for (uint8_t i = 0; i < 3; i++){
			PORTD = (PORTD & (~0x07)) | cols[i];
			if (!(PINA & ((1 << PINA0)| (1 << PINA1) | (1 << PINA2) | (1 << PINA3)) ))
				continue;
			else{
				_delay_ms(50);	
				if (!(PINA & ((1 << PINA0)| (1 << PINA1) | (1 << PINA2) | (1 << PINA3)) ))
					continue;
				for (uint8_t j = 0; j < 4; j++){
					if (PINA & rows[j])
						return map[j][i];
				}	
			}
		}
	}
	return 127;
}
uint16_t stringToInt(char* str, int8_t len){
	uint16_t ans = 0;
	uint16_t digit = 1;
	for (int8_t i = len; i >= 0; i--){
		ans += digit * (str[i] - '0');
		digit *= 10;
	}
	return ans;
}

/*
 * @brief function that guaranties correct reading of inputted number, every succesfuly read key is outputted on the screen
 * @param buf: buffer in which number will be saved as string WITHOUT NULL TERMINATOR IT CAN`T BEHAVE AS NORMAL C STRING 
 * @param terminator: charachter which upon being read stops waiting for input, it is not written into buf
 * @param start_adress: adress of the screen from which printing will start
 * @note function will work UNTIL terminator charachter is written in
 */
uint16_t getNumber(char buf[5], uint8_t terminator){
	uint8_t ch;
	uint8_t it = 0;
	do{
		ch = getKey();
		if (ch == terminator)
			break;
		if (ch == STAR || ch == HASH)
			continue;
		else{
			buf[it] = '0' + ch;
			writeSymbol(buf[it], SCREEN_ITERATOR);
			it++;	
		}
	}while(it < 5);
	//wait till user inputs terminator charachter
	while (ch != terminator) ch = getKey();

	uint16_t ans = stringToInt(buf, it -1 );
	return ans;
}

uint8_t digit(uint16_t val){
	if (val == 0) return 1;
	uint8_t ans = 0;
	while (val != 0){
		val/=10;
		ans++;
	}
	return ans;
}
//I mean explicytly passing digits is probably slop code, bu IN THIS EXACT LAB I DONOT WANT TO CALUCLATE DIGITS TWICE
void intToString(char* buf, uint16_t val, uint8_t dig){
	for (uint8_t i = 1; i <= dig; i++){
		buf[dig - i] ='0' + ( val %10);
		val/= 10;
	}
}
int main(void){
	DDRD |= (1 << PORTD0) | (1 << PORTD1) | (1 << PORTD2);
	DDRA &= ~((1 <<PORTA0) | (1 << PORTA1) | (1 << PORTA2) | (1 << PORTA3));
	DDRC = 0xFF;
	lcdInit();
	uint16_t num;
	char numStr[5];
	uint8_t dig;
	for(;;){
		num = getNumber(numStr, STAR);
		writeSymbol(STAR, SCREEN_ITERATOR);
		num *= getNumber (numStr, HASH);
		writeSymbol('=', SCREEN_ITERATOR);
		dig = digit(num);
		intToString(numStr, num, dig); 
		if (screen_iterator + dig > 16){
			lcdCmd(CLEAR_AND_SET_CURSOR_0);
			writeString(numStr, dig, SECOND_LINE_ADRESS);
		}
		else{
			writeString(numStr, dig, SCREEN_ITERATOR);
		}
		//input HASH twice to calculate next number
		num = getNumber(numStr, HASH);
		num = getNumber(numStr, HASH);
		lcdCmd(CLEAR_AND_SET_CURSOR_0);
		screen_iterator = 0;
	}
}



