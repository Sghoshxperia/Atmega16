/*
 * SPI_OneWay_Master.c
 *
 * Created: 03-Feb-20 6:08:59 PM
 * Author : user
 */ 


/* MASTER CODE

#include <avr/io.h>
#include <util/delay.h>


void SPI_MasterInit(void)
{
	//Made MOSI SS and SCK as OUTPUT
	DDRB |= (1<<DDB5) | (1<<DDB7) | (1<<DDB4);
	
	DDRB&=~(1<<DDB6);
	
	//Enabled SPI, set this as Master, set FOSC/16 as SCK frequency
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
}

void SPI_MasterTransmit(char data)
{
	//Write data to SPI data register. Writing to SPDR starts transmission
	SPDR = data;
	
	//Wait for transmission to be complete
	while(!(SPSR & (1<<SPIF)));
	
	
}


void Blink(int n)
{
	while(n!=0)
	{
		PORTB|=(1<<PINB1);
		_delay_ms(200);
		PORTB&=~(1<<PINB1);
		_delay_ms(200);
		n--;
		
	}	
}

int main()
{
	DDRB |= (1<<PINB1);		//Led set as OUTPUT
	
	DDRB &=~(1<<PINB2);		//Switch set as INPUT and put to high
	PORTB |=(1<<PINB2);
	
	SPI_MasterInit();
	
	int status;
	
	while(1)
	{
		status = PINB&(1<<2);
		
		if(status)
		{
			PORTB|=(1<<PINB1);
			
			SPI_MasterTransmit(0b11110000);
			
		}
		else
		{
			PORTB&=~(1<<PINB1);
			
			SPI_MasterTransmit(0b00001111);
		}
		
	}
	
}*/

//SLAVE CODE

#include <avr/io.h>
#include <util/delay.h>

void SPI_SlaveInit(void)
{
	//Made MOSI SCK and SS as INPUT
	DDRB&=~((1<<DDB5)|(1<<DDB7)|(1<<DDB4));
	
	//Made MISO as OUTPUT
	DDRB|=(1<<DDB6);
	
	//Enable SPI 
	SPCR = (1<<SPE)|(1<<SPR0);
	
}

char SPI_ReceiveSlave(void)
{
	
	//SPDR = 0b00000000;
	
	//Waiting for receive
	while(!(SPSR & (1<<SPIF)));
	
	
	return SPDR;
	
}

void Blink(int n)
{
	while(n!=0)
	{
		PORTB|=(1<<PINB1);
		_delay_ms(200);
		PORTB&=~(1<<PINB1);
		_delay_ms(200);
		n--;
		
	}
}

int main()
{
	
	DDRB|=(1<<PINB1);
	
	SPI_SlaveInit();
	
	while(1)
	{
		
		if(SPI_ReceiveSlave()==0b11110000)
		PORTB|=(1<<PINB1);
		else if(SPI_ReceiveSlave()==0b00001111)
		PORTB&=~(1<<PINB1);
		
		
	}
	
}



