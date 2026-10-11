#include <avr/io.h>
#define F_CPU 11059200
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define CHANNEL_ADC0 0
#define CHANNEL_ADC1 (1 << MUX0)
#define CHANNEL_ADC2 (1 << MUX1)
#define CHANNEL_ADC3 (1 << MUX0) | (1 << MUX1)
#define CHANNEL_ADC4 (1 << MUX2)
#define CHANNEL_ADC5 (1 << MUX0) | (1 << MUX2)
#define CHANNEL_ADC6 (1 << MUX1) | (1 << MUX2)
#define CHANNEL_ADC7 (1 << MUX0) | (1 << MUX1) | ( 1 << MUX2 )

#define FREQ_100_HERZ_OCR30_VAL 225
#define FREQ_1000_HERZ_OCR30_VAL 21
#define ADC_VAL_TO_OCR30_VAL(x) ( (x) / 5 + 21)

volatile uint8_t OCR0_buf = 0;
uint16_t readAdcChannel(uint8_t channel){
	ADMUX = (1 <<REFS0) | channel;
	ADCSRA |= (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF)));
	ADCSRA |= (1 << ADIF);
	uint16_t low = ADCL;
	uint16_t high = (uint16_t) ADCH << 8;
	return low | high;
}

ISR (TIMER0_COMP_vect){
	OCR0 = OCR0_buf;
}
/*
 * @brief convert value from ADC to OCR30 val
 * @param val: 10-bit value that will be converted into value from 21 to 215
 */
void inline setSoundFrequency(uint16_t val){
 //0 ; 1023
 // 21 ; 225
 	OCR0_buf = ADC_VAL_TO_OCR30_VAL(val);
}
void inline setDiodBrightness(uint16_t val){
	OCR3A = val;
}

int main(void){
	DDRE |= (1 << DDRE3) | (1 << DDRE4) | (1 << DDRE5);
	DDRB |= (1 << DDRB4);
	PORTE &= ~(1 << PORTE5);
	ADCSRA |= (1 << ADEN) |
           	  (1 << ADPS2)|
      		  (1 << ADPS1)|
         	  (1 << ADPS0);	
	ADMUX &= ~(1 << ADLAR); //правое выравнивание	
	//setting up CTC on timer 0
	TCCR0 = (1 << CS01) | //256 prescaler
		 (1 << CS02) |
		 (1 << WGM01)| // CTC mode
		 (1 << COM00); // toggle OC0 
	//setting up PWM on timer 3
	TCCR3A = (1 << WGM31) | //PWM phase correct, 10-bit
		  (1 << WGM30) |
		  (1 << COM3A1); // toggle OC3A (PORTE3) 
	TCCR3B = (1 <<CS30);
	//OCR3B = 0x03FF;
	OCR3A = 0;
	OCR0 = 0;
	PORTE |= (1 << PORTE4);
	TIMSK |= (1 << OCIE0);
	sei();
	for (;;){
		uint16_t adc_val = readAdcChannel(CHANNEL_ADC3);
		setSoundFrequency(adc_val);
		setDiodBrightness(adc_val);
		_delay_ms(20);
	}

}
