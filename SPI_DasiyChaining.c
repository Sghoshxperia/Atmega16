/*
 * DaisyChain_SPI.c
 *
 * Created: 27-Feb-20 9:30:37 AM
 * Author : user
 */ 

/*
 * DaisyChain_SPI.c
 *
 * Created: 27-Feb-20 10:46:46 PM
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

char Receiver()
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
		PORTB|=1<<PINB0;
		_delay_ms(100);
		PORTB&=~(1<<PINB0);
		_delay_ms(100);
		i++;
	}
}

int main(void)
{
	int toggle;
	DDRB|=1<<PINB0;
	
	DDRB&=~(1<<PINB2);			//Switch for SLAVE 1
	PORTB|=(1<<PINB2);
	
	DDRD&=~(1<<PIND2);			//Switch for SLAVE 2
	PORTD|=(1<<PIND2);
	 
	int status_pin1;			//status for SLAVE 1
	int status_pin2;			//status for SLAVE 2
	
	
	while (1)
	{
		//Slave_Init();
		Master_Init();
		
		_delay_ms(5);
		status_pin1=PINB&(1<<2);
		
		//_delay_ms(5);
		status_pin2=PIND&(1<<2);
		

		
		if(status_pin1)							//SLAVE 1 
		{
			
			Blink(1);
			_delay_us(100);
			Transmit(0b11110000);
			_delay_us(100);
			
		}
		
		else if(status_pin2)					//SLAVE 2
		{
			
			Blink(1);
			_delay_us(100);
			Transmit(0b00001111);
			_delay_us(100);
			
			
		}

		else                                            //NO SWITCH IS PRESSED
		{
			
			Transmit(0b10101010);
			_delay_us(100);
			toggle=Receiver();
			_delay_us(100);
			
			if(toggle==0b11000011)
			Blink(2);
			if(toggle==0b00111100)
			Blink(2);
			else
			PORTB&=~(1<<PINB0);
			

		}
		

	}

}


/*SLAVE1 CODE

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

char Receiver()
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
		PORTB|=1<<PINB0;
		_delay_ms(100);
		PORTB&=~(1<<PINB0);
		_delay_ms(100);
		i++;
	}
}

int main(void)
{
	int toggle;
	DDRB|=1<<PINB0;
	
	DDRB&=~(1<<PINB2);			//Switch for SLAVE 1
	PORTB|=(1<<PINB2);
	
	

	
	int status_pin;			
				
	
	
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
			Transmit(0b11000011);
			_delay_us(100);
			
		}
		

		else                                            //NO SWITCH IS PRESSED
		{
			
			Transmit(0b10101010);
			_delay_us(100);
			toggle=Receiver();
			_delay_us(100);
			
			if(toggle==0b11110000)
			Blink(2);
			else if(toggle==0b00001111)
			Transmit(0b00001111);
			else
			PORTB&=~(1<<PINB0);
			

		}
		

	}

}*/



/*SLAVE2 CODE

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

char Receiver()
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
		PORTB|=1<<PINB0;
		_delay_ms(100);
		PORTB&=~(1<<PINB0);
		_delay_ms(100);
		i++;
	}
}

int main(void)
{
	int toggle;
	DDRB|=1<<PINB0;
	
	DDRB&=~(1<<PINB2);			//Switch for SLAVE 1
	PORTB|=(1<<PINB2);
	
	

	
	int status_pin;			
				
	
	
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
			Transmit(0b00111100);
			_delay_us(100);
			
		}
		

		else                                            //NO SWITCH IS PRESSED
		{
			
			Transmit(0b10101010);
			_delay_us(100);
			toggle=Receiver();
			_delay_us(100);
			
			if(toggle==0b00001111)
			Blink(2);
			
			else
			PORTB&=~(1<<PINB0);
			

		}
		

	}

}*/





