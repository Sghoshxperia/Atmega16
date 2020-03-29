/*
 * PWM.c
 *
 * Created: 18-Jan-20 10:38:46 AM
 * Author : user
 */ 

#include <avr/io.h>
#include <util/delay.h>

/*void PWM_init()
{
	
	TCCR0 = (1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS01);
	DDRB |= 1<<PINB3;
			
}*/

int main(void)
{
	int duty;
	TCCR0 = (1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS01);
	DDRB |= 1<<PINB3;
	
	//PWM_init();
	
	
 
    while (1) 
    {
		
		for(duty=0;duty<255;duty++)
		{
			OCR0 = duty;
			_delay_ms(5);
		}
		
		for(duty=255;duty>1;duty--)
		{
			OCR0 = duty;
			_delay_ms(5);
		}
		
    }
}

