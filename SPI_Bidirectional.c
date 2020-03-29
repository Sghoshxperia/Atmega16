/*
 * SPI_Bidirectional.c
 *
 * Created: 04-Feb-20 10:46:46 PM
 * Author : user
 */ 

#include <avr/io.h>
#include <util/delay.h>


void Master_Init()
{

	//Set MOSI and SCK output, all others input
	//DDRB=0b00000000;
	DDRB&=~(1<<PINB6);//MISO
	DDRB |= (1<<PINB4);//SLAVE SELECT
	DDRB|= (1<<PINB5)|(1<<PINB7);// Enable SPI, Master, set clock rate fck/16
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void Transmit(char data)
{

	//SPDR=0b00000000;
	SPDR=data;
	while(!(SPSR&(1<<SPIF)));
}

void Slave_Init()
{
	//DDRB=0b00000000;
	DDRB &=~(1<<PINB5);
	DDRB &=~(1<<PINB7);
	DDRB &=~(1<<PINB4);
	DDRB |= (1<<PINB6);
	SPCR |= (1<<SPE);
}

char Receive()
{
	//SPDR=0b00000000;
	while(!(SPSR & (1<<SPIF)));  // Return data register
	return SPDR;
}

void Blink(int s)
{
	int i=0;
	while (i<s)
	{
		PORTB|=1<<PINB1;
		_delay_ms(100);
		PORTB&=~(1<<PINB1);
		_delay_ms(100);
		i++;
	}
}

int main(void)
{
	int toggle;
	DDRB|=1<<PINB1;
	DDRB&=~(1<<PINB2);
	PORTB|=1<<PINB2;
	int status_pin;
	int pressed;
	
	while (1)
	{
		Slave_Init();
		//Master_Init();
		_delay_ms(5);
		status_pin=PINB&(1<<2);


		if(status_pin)
		{
			
			Blink(1);
			_delay_us(100);
			Transmit(0b11100000);
			_delay_us(100);
			
		}

		else
		{
			
			Transmit(0b00000000);
			_delay_us(100);
			toggle=Receive();
			_delay_us(100);
			
			if(toggle==0b11100000)
			Blink(2);
			else if(toggle==0b00000000)
			PORTB&=~(1<<PINB1);
			
		}
		
	}

}
