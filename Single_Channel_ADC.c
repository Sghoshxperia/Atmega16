
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned int ADC_read(unsigned char chnl)
{
	chnl= chnl & 0b00000111;     //select adc channel between 0 to 7
	ADMUX|=chnl;                 //channel A0 selected


	ADCSRA|=(1<<ADSC);   		// start conversion
	while(!(ADCSRA & (1<<ADIF)));   // wait for ADIF conversion complete return
	ADCSRA|=(1<<ADIF); 
	
	 
	uint8_t val = ADCL;
	uint16_t value=(ADCH<<8)|val ;
	
	  
	return (value); //return calculated ADC value
}


void PWM_init()
{
	
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);  
	DDRB|=(1<<PINB3);
	
}


int main(void)
{
	DDRA = 0b00000000;
	
	PWM_init();
	
	ADMUX=(1<<REFS0);      			    // Selecting internal reference voltage
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);     // Enable ADC also set Prescaler as 64
	ADMUX&=~(1<<ADLAR);
	
	int i = 0; // define an integer to save adc read value
	
	
	while (1)
	{
		
		i = ADC_read(0);

		OCR0 = i/4;
		
		//_delay_ms(15);
		      
	}
}
