/*
 * Led_Button2.c
 *
 * Created: 17-Jan-20 8:11:31 PM
 * Author : user
 */ 

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	
	DDRB |=1<<PINB0;
	
	DDRB &= ~(1<<PINB2);
	PORTB|=1<<PINB2;
	
	
	int old_status;
	int new_status;
	
    old_status = PINB&(1<<2);
	
	while (1) 
	
    {
		new_status = PINB&(1<<2);
		
		
		if(new_status)
		{
			_delay_ms(10);

			if(old_status!=new_status)
			{
				
				PORTB^=1<<PINB0;
				old_status=new_status;
			}
			
		}
		else
		{
			_delay_ms(10);
			old_status=new_status;
		}
		
	}
}
			
