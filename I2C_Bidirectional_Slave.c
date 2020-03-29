/*
 * I2C_Slave_TwoWay.c
 *
 * Created: 24-Feb-20 4:09:20 PM
 * Author : user
 */ 

//SLAVE I2C

#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>


void Blink(int n)
{
	while(n!=0)
	{
		PORTB|=(1<<PINB0);
		_delay_ms(200);
		PORTB&=~(1<<PINB0);
		_delay_ms(200);
		n--;
		
	}
}

void I2C_Init(unsigned char Slave_Addr)
{
	
	TWAR = Slave_Addr;

}

void I2C_Read_Data()			//unsigned char -> void
{

	unsigned char data;
	//uint8_t status;
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT))); 
	//status = TWSR&0xF8;
	//while(status!=0x80);
	while((TWSR & 0xF8)!= 0x80); 
	data = TWDR;
	//return data;
	
	if(data == 0b00001111)
	Blink(4);
	else
	PORTB&=~(1<<PINB0); 
	
}

void TWI_write_slave(unsigned char write_data) // Function to write data
{
	TWDR= write_data;           // Fill TWDR register whith the data to be sent
	TWCR= (1<<TWEN)|(1<<TWINT);   // Enable TWI, Clear TWI interrupt flag
	while((TWSR & 0xF8) != 0xC0); // Wait for the acknowledgement
}

void TWI_match_write_slave(void) //Function to match the slave address and slave dirction bit(write)
{
	while((TWSR & 0xF8)!= 0xA8) // Loop till correct acknoledgement have been received
	{
		// Get acknowlegement, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
	}
}


void I2C_Match_ACK()
{
	
	while((TWSR & 0xF8)!= 0x60)
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		while(!(TWCR&(1<<TWINT)));
		
	}

}

int main()
{
	DDRB&=~(1<<PINB2);
	PORTB|=(1<<PINB2);
	
	DDRB|=(1<<PINB0);		//Led is set as OUTPUT	
	I2C_Init(0x20);
	
	int button;
	
	while(1)
	{
		button = PINB&(1<<2);
		
		if(button)
		{
			Blink(2);
			TWI_match_write_slave();
			TWI_write_slave(0b11100000);
			
		}
		
		
		I2C_Match_ACK();
		
		I2C_Read_Data();
		
	}
	
	
}
