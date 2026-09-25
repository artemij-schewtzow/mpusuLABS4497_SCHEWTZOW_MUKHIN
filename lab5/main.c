//TODO FIX INT PARSERS
//REALISE THE SCROLL CORRECTLY

#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>

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

uint8_t tabCon[] ={0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,
0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,0x50,0x43,0x54,0xA9,
0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0x62,0xAF,0xB0,0xB1,
0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,
0xBC,0xBD,0x6F,0xBE,0x70,0x63,0xBF,0x79,0x5C,0x78,0xE5,0xC0,
0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7};

const int8_t uArr[5] = {1, -6, -13, -20, -27};
const int16_t inline f(const int8_t u,const int16_t y){
	return (7 * y + 3 * u - 2);
}


const int16_t inline yCalc(const int16_t y, const uint8_t uIndex){
	return (y + 3* f(uArr[uIndex], y));
}

void solve(int16_t arr[6]){
	arr[0] = 0;
	for (uint8_t i = 1; i < 6; i++){
		arr[i] = yCalc(arr[i - 1], uArr[ i - 1 ]);
	}
}

void lcdCmd(uint8_t cmd) {
//[]--------------------------------------------------[]
//| Назначение: запись кодов в регистр команд ЖКИ
//| Входные параметры: cmd - код команды
//[]--------------------------------------------------[]
DDRC = 0xFF;
// все разряды PORTC на выход
DDRD |= ((1<<E)|(1<<RS));// разряды PORTD на выход
PORTD &= ~(1<<RS);
// выбор регистра команд RS=0
PORTC = cmd;
// записать команду в порт PORTC
PORTD |= (1<<E); // \ сформировать на
_delay_us(5);
// | выводе E строб 1-0
PORTD &= ~(1<<E); // / передачи команды
_delay_ms(100); // задержка для завершения записи
}	

void lcdData(uint8_t data){
	DDRC = 0xFF;
	DDRD |= (1 << E) | (1 << RS);
	PORTD |= ( 1 << RS );
	PORTC = data;
	PORTD |= ( 1 << E );
	_delay_us(5);
	PORTD &= ~(1 << E);
	_delay_ms(100); 
}

void lcdInit(void) {
//[]--------------------------------------------------[]
//| Назначение: инициализация ЖКИ
//[]--------------------------------------------------[]
DDRC = 0xFF;
// все разряды PORTC на выход
DDRD |= ((1<<E)|(1<<RS)); // разряды PORTD на выход
_delay_ms (100); // задержка для установления напряжения питания
lcdCmd (0x30);
// \ вывод
lcdCmd (0x30);
// | трех
lcdCmd (0x30);
// / команд 0x30
lcdCmd (0x38);
// 8 разр.шина, 2 строки, 5 × 7 точек
lcdCmd (0x0E);
// включить ЖКИ и курсор, без мерцания
lcdCmd (0x06);
// инкремент курсора, без сдвига экрана
lcdCmd (0x01);
// очистить экран, курсор в начало
}

const uint8_t code(const uint8_t symb) {
//[]------------------------------------------------[]
//| Назначение: перекодировка символов кириллицы|
//| Входные параметры: symb – символ ASCII

//| Функция возвращает код отображения символа|
//[]------------------------------------------------[]
return (symb >= 192 ? tabCon[symb-192]: symb);
}

void writeSymbol(const uint8_t symb, const uint8_t adress){
	lcdCmd(SET_DDRAM_ADRESS(adress));
	lcdData(code(symb));
}
void writeString(const char* str, const uint8_t len, uint8_t start_adress){
	for (uint8_t i = 0; i < len; i++){
		writeSymbol(str[i], (start_adress + i) % 0x64);
	}
}
uint8_t cntDigits(int16_t val){
	if (val == 0) return 1;
	uint8_t ans = 0;
	if (val < 0) ans++;
	while (val != 0){
		val/=10;
		ans++;
	}
	return ans;
}
void convertIntToString(int16_t val, char buf[8]){
	if (val == 0 ){
		buf[0] = '0';
		return;
	}
	int16_t temp = val;
	uint8_t cntDigits = 0;
	while (temp != 0){
		temp/=10;
		cntDigits++;
	}
	if (val < 0){
		cntDigits++;
		buf[0] = '-';
		while(cntDigits > 1){
			buf[--cntDigits] = '0' - (val % 10);
			val/=10;
		}
	}
	else{
		while(cntDigits > 0){
			buf[--cntDigits] = '0' + (val % 10);
			val/=10;
		}
	}
}

/*// pile of junk
void inline insertStr(uint8_t buffer[128], uint8_t starting_pos, const char* str){
	while (*str){
		buffer[starting_pos++] = *str++;
	}
}
void buildOutputString(uint8_t buffer[128], int16_t valArr[6]){
	uint8_t it = 1;
	uint8_t i = 1;
	while (it < 127){
		insertStr(buffer, it - 1, "Результат: Y(");
		it += 17;
		insertStr(buffer, (it - 1), convertIntToString(i));
		it += cntDigits(i) + 1;
		insertStr(buffer, it -1, ") = ");
		it += 5;
		insertStr(buffer, (it - 1), convertIntToString(valArr[i++]));
		it += cntDigits(valArr[i -1]) + 1;
		insertStr(buffer, it - 1, " ");
		it += 2;
	}

}*/
int main(void){
	//solve this thing
	lcdInit();
	
	int16_t yArr[6];
	solve(yArr);
	
	for (;;){
		/*
		 *Okay do I reslly want an infinite scrolling string, the best I`ve came up for rn is to make just first 16 blank, so I`ll have fine start
		 I am not sure that it is possible to make smooth change from the end of a buffer to the start, now I`m thinking it kinda is just the same as scroll for each individual
		 string, so maybe I`ll just do
		 Paste the string in a buffer
		 Scroll 
		 clear the screen 
		 repeat BUT THAT IS NO WA
		 * */
		_delay_ms(2000);
		writeString("Результат: Y(", 12, SECOND_LINE_ADRESS);
		for (uint8_t i = 1; i < 6; i++){
			//print the output
			char buf[8];
			convertIntToString(i, buf);
			uint8_t len = cntDigits(i);
			writeString(buf, len, SECOND_LINE_ADRESS + 12);
			writeString(") = ", 4, SECOND_LINE_ADRESS + 12 + len );
			convertIntToString(yArr[i], buf);
			uint8_t len2 = cntDigits(yArr[i]);
			writeString(buf, len2, SECOND_LINE_ADRESS + 12 + len + 4 + len2);
			//|xxxxxxxxxxxxxxxx
			//|Результат: Y(1) = X
			for (uint8_t j = 0; j < 11; j++){
				lcdCmd(MOVE_SCREEN_RIGHT);
				_delay_ms(1000);
			}
			_delay_ms(3000);
			for (uint8_t j = 0; j < 11; j++){
				lcdCmd(MOVE_SCREEN_LEFT);
				_delay_ms(1000);
			}
			lcdCmd(SET_CURSOR_0);
		}
	}
}







