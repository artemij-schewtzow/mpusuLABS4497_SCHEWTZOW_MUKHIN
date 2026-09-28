
#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

volatile uint8_t pressCntr = 0;
volatile uint8_t sendFlag = 0;

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
		draw(to_draw, num_digits);
	}	
}

void uartInit(void) {
	UCSR0B |= (1 << TXEN0); // включение передатчика
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
//для значения 19200, UBRR0 = 36 = 0x24
	UBRR0H = 0;
	UBRR0L = 0x24;
}
//[]----------------------------------------------------[]
//| Назначение: передача символа с МК на ПК |
//| ожидание выставления бита UDRE0, |
//| после чего кладём посылку в UDR0 |
//[]----------------------------------------------------[]
void uartTransmitByte(char byte) {
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = byte;
}

void inline uartSendStr(char* str){
	while (*str){
		uartTransmitByte(*str);
		str++;
	}
}



ISR (INT2_vect){
	EIMSK &= ~(1 << INT2);
	
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	TIMSK |= (1 << TOIE0);
}

ISR (TIMER0_OVF_vect){
	TIMSK &= ~(1 <<TOIE0);
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
	if (PIND & ( 1 << PIND2 )){
		pressCntr++;
		sendFlag = 1;
	}
	EIMSK |= (1 << INT2);
	
}

int main(void){

	DDRC = 0xFF;
	DDRA = (1 << DDRA1) | (1 << DDRA2) | (1 << DDRA3) | (1 << DDRA4) | (1 << DDRA5);
	//clear PORTA
	PORTA = 0;			

	EICRA |= (2 << ISC20); // call interrupt on falling front
	EIMSK |= (1 << INT2); // allow interrupts on INT2
	sei();

	uartInit();
	for (;;){
		for (uint16_t i = 500 ; i < 1100; i+=20){
			if (sendFlag){
				cli();
				uint8_t val = pressCntr;
				sendFlag = 0;
				sei();
				uint8_t digits = countDigits(val);
				char buf[3];
				for (uint8_t j = 0; j < digits; j++ ){
					buf[j]  = '0' + digit(val, 6 - digits + j);
				}
				for (uint8_t j = digits; j < 3; j++){
					buf[j] = ' ';
				}

				char str[32] = "Число нажатий: ";
				str[27] = buf[0]; str[28] = buf[1]; str[29] = buf[2]; str[30] = '\n'; str[31] = '\0';
				uartSendStr(str);
			}
			drawNumber(i);
			_delay_ms(100);
		}
	}

}
