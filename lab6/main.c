/*
 * @file main.c
 * @brief Шестая лабораторная работа: темы - АЦП, ШИМ 
 * @author Шевцов А.И. Мухин А.П.
 * @date 2026-10-31
 */

#include <avr/io.h>
#define F_CPU 11059200UL
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

#define ADP_MODE_COLOUR 0
#define ADP_MODE_BRIGHTNESS 1

/*
 * @brief LED value, with 10 bit RGB and brightness values
 */
typedef struct{
	uint16_t R;
	uint16_t G;
	uint16_t B;
	uint16_t brightness;
	} LED;

LED led;
volatile uint8_t adp_mode = ADP_MODE_COLOUR;
volatile uint8_t OCR0_buf = 0;

/*
 * @brief get a 10-bit value from adc channel
 * @param channel - adc channel
 */
uint16_t readAdcChannel(uint8_t channel){
	ADMUX = (1 <<REFS0) | channel;
	ADCSRA |= (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF)));
	ADCSRA |= (1 << ADIF);
	uint16_t low = ADCL;
	uint16_t high = (uint16_t) ADCH << 8;
	return low | high;
}

/*
 * @brief change frequency of the sound
 */
ISR (TIMER0_COMP_vect){
	OCR0 = OCR0_buf;
}
/*
 * @brief enable timer 2, permit interrupts on overflow
 */
ISR (INT2_vect){
	EIMSK &= ~(1 << INT2);	
	TCCR2 |= (1 << CS22) | (1 << CS20);
	TIMSK |= (1 << TOIE2);
}

/*
 * @brief check for button debounce, change control mode of adp
 */
ISR (TIMER2_OVF_vect){
	TCNT2 = 0;
	TIMSK &= ~(1 <<TOIE2);
	TCCR2 &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
	if (!(PIND & ( 1 << PIND2 ))){
		if (adp_mode == ADP_MODE_COLOUR)
			adp_mode = ADP_MODE_BRIGHTNESS;
		else 
			adp_mode = ADP_MODE_COLOUR;
	}
	EIMSK |= (1 << INT2);
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

/*
 * @brief update colour and brightness on LED
 * @note since my variant required Green to Light Blue transition, code for red colour is not written
 */
void updateLED(){
	OCR3A = (uint16_t) led.B * ( (double) led.brightness / 0x03FF );
	OCR3B = (uint16_t) led.G  * ( (double) led.brightness / 0x03FF );
	//OCR3C = led.R;
}

/*
 * @brief toggle LED parameters based on value from ADC
 * @param adc_val - value from ADC
 */
void inline toggleLED(uint16_t adc_val){
	if (adp_mode == ADP_MODE_BRIGHTNESS){
		led.brightness = adc_val;
		updateLED();
	}
	else{
		led.B = adc_val;
		updateLED();
	}
}

int main(void){
	DDRE |= (1 << DDRE3) | (1 << DDRE4) | (1 << DDRE5);
	DDRB |= (1 << DDRB4);
	PORTE &= ~(1 << PORTE5);
	PORTD &= ~(1 << PORTD2);
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
		  (1 << COM3A1)| //toggle OCR3A (PORTE3)
		  (1 << COM3B1); //toggle OCR3B (PORTE4) 
		  
	TCCR3B = (1 <<CS30);
	OCR3B = 0x03FF;
	OCR3A = 0;
	OCR0 = 0;
	TIMSK |= (1 << OCIE0);

	EICRA = (2 << ISC20); // call interrupt on falling edge
	EIMSK = (1 << INT2); // allow interrupts on INT2
	led.R = 0;
	led.G = 0x03FF;
	led.B = 0;
	led.brightness = 128;
	sei();
	for (;;){		
		uint16_t adc_val = readAdcChannel(CHANNEL_ADC3);
		setSoundFrequency(adc_val);
		toggleLED(adc_val);
		_delay_ms(20);
	}
}
