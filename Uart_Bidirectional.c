/*
 * Uart_bidirectional.c
 *
 * Created: 28-Jan-20 7:10:29 PM
 * Author : user
 */ 

#include <avr/io.h>
#include <util/delay.h>


unsigned char toggle;

void TransmitUART(unsigned char data)
{
	
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
	
}

void Blink(int n)
{
	
	
	while(n!=0){
	PORTB|=(1<<PINB1);
	_delay_ms(200);
	PORTB&=~(1<<PINB1);
	_delay_ms(200);
	n--;
	
	}
	
	
}

void ReceiveUART(void)
{
	while(!(UCSRA & (1<<RXC)));
	toggle = UDR;
	if (toggle==5)
	Blink(5);
	
}



int main(void)
{
	DDRB|=(1<<PINB1);	//Led pin set as output
	
	DDRB&=~(1<<PINB4);	//Switch pin set as input as set to high
	PORTB|=(1<<PINB4);
	
	
	int status = 0;		//Switch status
	
	//UART Details
	
	int ubrrvalue = 25;
	UBRRH = (unsigned char)(ubrrvalue>>8);
	UBRRL = (unsigned char)ubrrvalue;
	
	
	//enable TX and RX
	UCSRB = (1<<RXEN) | (1<<TXEN);
	
	//frame format = 8 data bits: 2 stop bits
	UCSRC = (1<<URSEL) | (1<<USBS) | (3<<UCSZ0);
	

   
    while (1) 
    {
		
		status = PINB&(1<<4);
		 
		if(status)		
		{
					
					Blink(2);
					PORTB&=~(1<<PINB1);
					
					TransmitUART(5);	
				
		}
			
		
		else
		
		  {
				TransmitUART(2);

				PORTB&=~(1<<PINB1);
			}
				
			ReceiveUART();
			
			
			
		}
	}
		
		
		
		
		
    


