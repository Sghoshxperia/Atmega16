/*
 * delay_timer.c
 *
 * Created: 18-Jan-20 12:08:34 PM
 * Author : user
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

void delay(int x)
{
	//n=256-n;
	
	TCCR0 = (1<<CS02)|(1<<CS00);
	TCNT0 = 256-x;
	//TIFR&=~(1<<TOV0);
	
	while((TIFR&(1<<TOV0))==0);
	
		//TOV0 = 0;	
		TCCR0 = 0;
		//TCNT0 = 0;
		TIFR = 0b00000001;
}

void actualdelay(int n)
{
	
	int temp = n/250;
	int temp2 = n%(temp*250);
	
	for(int i=0;i<temp;i++)
	{
		delay(250);
	}
	delay(temp2);	
}


int main(void)
{
	
	DDRB |= (1<<PINB3);
	PORTB |= (1<<PINB3);
    
    while (1) 
    {
		PORTB^=(1<<PINB3);
		//_delay_ms(500);
		//delay();
		actualdelay(500);
		
    }
}

