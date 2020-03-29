/*
 * Led_button.c
 *
 * Created: 13-Jan-20 6:08:00 PM
 * Author : user
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1<<PINB0); //Nakes first pin of PORTC as Output
	DDRD &= ~(1<<PIND4);//Makes firs pin of PORTD as Input
	PORTB| = (1<<PINB0);

	int status;

	while(1) 
	{
		status = PIND%(1<<4);
		
		if(status) 
		{
			PORTB |= (1<<PINB0); 
			_delay_ms(500); 
			PORTB &= ~(1<<PINB0); 
		}
		
		else
		
		PORTB&=~(1<<PINB0); 
	}
	
}
