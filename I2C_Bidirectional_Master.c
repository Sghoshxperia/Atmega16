/*
 * I2C_Master_TwoWay.c
 *
 * Created: 24-Feb-20 2:57:07 PM
 * Author : user
 */ 


#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>

//MASTER I2C

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

//I2C Initialize

void I2C_Init()			
{
	TWBR = 0b00000001;					// Setting TWBR0 as 1
	TWSR &=~((1<<TWPS1)|(1<<TWPS0));	// Setting Pre-scalar 1
	TWCR = (1<<TWEN);
	 	
}


void I2C_Start()		// I2C start function 
{
	//uint8_t status;		// Declare variable 
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT); // Enable TWI, generate START 
	while(!(TWCR&(1<<TWINT)));	// Wait until TWI finish its current job 
	//status=TWSR & 0xF8;		// Read TWI status register 
	//while(status!=0x08);
	while((TWSR & 0xF8)!= 0x08);	
			
}



void I2C_Write_Addr(unsigned char Addr)		// I2C write function 
{
	//uint8_t status;		// Declare variable 
	
	TWDR=Addr;			// Copy data in TWI data register 
	TWCR=(1<<TWEN)|(1<<TWINT);	// Enable TWI and clear interrupt flag 
	while(!(TWCR&(1<<TWINT)));	// Wait until TWI finish its current job 
	//status=TWSR&0xF8;		// Read TWI status register 
	//while(status!=0x18);	
	while((TWSR & 0xF8)!=0x18);		
	//PORTB=(1<<PINB0);	
}

void I2C_Write_Data(unsigned char data)
{
	//PORTB=1<<PINB0;
	//uint8_t status; 
	
	TWDR = data;				// Copy data in TWI data register 
	TWCR=(1<<TWEN)|(1<<TWINT);	// Enable TWI and clear interrupt flag 
	while(!(TWCR&(1<<TWINT)));	// Wait until TWI finish its current job 	
	//status=TWSR&0xF8;
	//while(status!=0x28);
	while((TWSR & 0xF8)!=0x28);
	//PORTB = 1<<PINB0;
	
}

void TWI_read_address(unsigned char data)
{
	TWDR=data; // Address and read instruction
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received
	while((TWSR & 0xF8)!= 0x40);  // Check for the acknoledgement
}

void TWI_read_data(void)
{
	unsigned char recv_data;
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x58); // Check for the acknoledgement
	recv_data=TWDR;
	if(recv_data == 0b11100000)
	Blink(4);
}


void I2C_Stop()			// I2C stop function 
{
	//PORTB = 1<<PINB0;
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);// Enable TWI, generate stop 
	while(TWCR&(1<<TWSTO));	// Wait until stop condition execution 
	//PORTB = 1<<PINB0;
}

	


int main(void)
{
    
	
	
	DDRB&=~(1<<PINB2);		//Switch made as INPUT
	PORTB|=(1<<PINB2);		//Switch is pulled to HIGH by default


	DDRB|=(1<<PINB0);	   //Led set as OUTPUT
	
	I2C_Init();
	
	int button;
	
	//PORTB = 1<<PINB0;
			
	while(1)
	{
		I2C_Start(); 
		I2C_Write_Addr(0x20); 
		button = PINB&(1<<2);
		
		if(button)
		{
			//PORTB|=(1<<PINB0);
			//I2C_Write_Addr(0x20);
			I2C_Write_Data(0b00001111);
			Blink(2);
		}
		
		else
		{
			PORTB&=~(1<<PINB0);
			I2C_Write_Data(0b11110000);
		}
		
		I2C_Stop(); 
		
		_delay_ms(10);
		
		I2C_Start();
		TWI_read_address(0x21);
		TWI_read_data();
		I2C_Stop();
		
		
	}
		
		
    
}

