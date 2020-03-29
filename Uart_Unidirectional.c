/*
 * Uart_Led_SingleDirection.c
 *
 * Created: 01-Feb-20 8:00:18 PM
 * Author : user
 */ 


#include <avr/io.h>
#include <util/delay.h>

/* RECEIVER CODE

int main(void)
{
	
	
	DDRB|=(1<<PINB0);
	
	 int ubrrvalue = 25;
	 
	 UBRRH = (unsigned char)(ubrrvalue>>8);
	 UBRRL = (unsigned char)ubrrvalue;
	 UCSRB = (1<<RXEN) | (1<<TXEN);
	 UCSRC = (1<<URSEL) | (1<<USBS) | (3<<UCSZ0);
	 
	unsigned char receiveData;
	
	while(1)
	{
		while(!(UCSRA & (1<<RXC)));
		receiveData = UDR;
		
		if(receiveData == 0b11110000)
		PORTB|=(1<<PINB0);
		else if(receiveData == 0b00001111)
		PORTB&=~(1<<PINB0);
			
	}
	
}*/


//MASTER CODE

int main(void)
{
	DDRB|=(1<<PINB1);	//Led pin set as output
	//PORTB|=(1<<PINB1);
	DDRB&=~(1<<PINB4);	//Switch pin set as input as set to high
	PORTB|=(1<<PINB4);
	
	int status = 0;
	
	int ubrrvalue = 25;
	
	//UART Details
	UBRRH = (unsigned char)(ubrrvalue>>8);
	UBRRL = (unsigned char)ubrrvalue;
	
	//enable TX and RX
	UCSRB = (1<<RXEN) | (1<<TXEN);
	
	//frame format = 8 data bits: 2 stop bits
	UCSRC = (1<<URSEL) | (1<<USBS) | (3<<UCSZ0);
	
   
    while (1) 
    {
		status = PINB&(1<<4);
		 
		if(status)		//if switch is pressed
		{
			_delay_ms(10);
			
					PORTB|=(1<<PINB1);
					
					//Wait until transmitter is ready
					
					while(!(UCSRA & (1<<UDRE)));
					UDR = 0b11110000;					
					
				}
				
		else
		
		{
			
			PORTB&=~(1<<PINB1);
			
			while(!(UCSRA & (1<<UDRE)));
			UDR = 0b00001111;
			
		}
		
    }
}



