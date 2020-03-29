
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned int ADC_read(unsigned char chnl)
{
	chnl&=0b00000111; // select adc channel between 0 to 7
	ADMUX|=chnl;    
	    
	ADCSRA|=(1<<ADSC);   // start conversion
	while(!(ADCSRA & (1<<ADIF)));   // wait for ADIF conversion complete return
	ADCSRA|=(1<<ADIF);   // clear ADIF when conversion complete by writing 1
	
	uint8_t val = ADCL;
	uint16_t value = (ADCH<<8)|val;
	
	return (value); //return calculated ADC value
}


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
	
	ADMUX=(1<<REFS0);                        // Selecting internal reference voltage
	ADCSRA|=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);
	ADMUX&=~(1<<ADLAR);                     // Enable ADC also set Prescaler as 64
	
	int i = 0;
	int j = 0; // define an integer to save adc read value
	
	while (1)
	{
		
		/*i = ADC_read(0);                        //ADC dual channel without switch case
			OCR0 = i/4; 
			if(OCR0<8)
			OCR0 = 0;
			
			ADMUX&=0b11111000;
			_delay_us(10);
		                     
		j = ADC_read(1);
			OCR2 = j/4;
			if(OCR2<8)
			OCR2 = 0;
			
			ADMUX&=0b11111000;
			_delay_us(10);*/
		
		
		switch(ADMUX)                            //ADC dual channel with switch case
		{
			
			case 0b01000000:
			
			i = ADC_read(0);
			OCR0 = i/4;
			
			ADMUX = 0b01000001;
			break;
			
			case 0b01000001:
			
			j = ADC_read(1);
			OCR2 = j/4;
			
			ADMUX = 0b01000000;
			break;
			
			default:
			//Default code
			break;
			
			
		}
		
		ADCSRA|=(1<<ADSC);
		
	}
}
