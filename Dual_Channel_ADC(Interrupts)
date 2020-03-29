#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


/*unsigned int ADC_read(unsigned char chnl)
{
	chnl&=0b00000111; // select adc channel between 0 to 7
	ADMUX|=chnl;
	
	ADCSRA|=(1<<ADSC);   // start conversion
	while(!(ADCSRA & (1<<ADIF)));   // wait for ADIF conversion complete return
	ADCSRA|=(1<<ADIF);   // clear ADIF when conversion complete by writing 1
	
	uint8_t val = ADCL;
	uint16_t value = (ADCH<<8)|val;
	
	return (value); //return calculated ADC value
}*/

 unsigned char i = 0;
 unsigned char j = 0;

void PWM_init()
{
	
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	TCCR2 = (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<CS20);
	DDRB|=(1<<PINB3);
	DDRD|=(1<<PIND7);
	
}


int main(void)
{
	

	DDRA = 0b00000000;
	
	PWM_init();
	
	ADMUX=(1<<REFS0);                                 // Selecting internal reference voltage
	ADCSRA|=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);
	ADMUX&=~(1<<ADLAR);
	ADCSRA |= 1<<ADIE;
	

	sei();

	ADCSRA |= 1<<ADSC;

	while (1)
	{
		
		OCR0 = i;
    
		OCR2 = j;
    
	}
}


ISR(ADC_vect)
{
	
	uint8_t theLow = ADCL;
	uint16_t theHigh = ADCH<<8 | theLow;	

	switch (ADMUX)
	{
		case 0b01000000:
		
		if((theHigh/4)<33)
		i = 0;
		else
		i = theHigh/4;
		
		ADMUX = 0b01000001;
		break;
		
		case 0b01000001:
		
		if((theHigh/4)<33)
		j=0;
		else
		j = theHigh/4;
				
		ADMUX = 0b01000000;
		break;
		
		default:
		//Default code
		break;
	} 
	
	ADCSRA |= 1<<ADSC;
}
