/*
 * I2C_Unidirectional.c
 *
 * Created: 20-Feb-20 6:14:56 PM
 * Author : user
 */ 



#include <avr/io.h>
#define F_CPU 1000000UL

//MASTER I2C

//I2C Initialize
void I2C_Init()			
{
	TWBR = 0b00000001;					// Setting TWBR0 as 1
	TWSR &=~((1<<TWPS1)|(1<<TWPS0));	// Setting Pre-scalar 1
	TWCR = (1<<TWEN);
	 	
}


void I2C_Start()		// I2C start function 
{
			
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT); // Enable TWI, generate START 
	while(!(TWCR&(1<<TWINT)));	// Wait until TWI finish its current job 
			                           // Read TWI status register 
	
	while((TWSR & 0xF8)!= 0x08);	
			
}



void I2C_Write_Addr(unsigned char Addr)		// I2C write function 
{
	                		
	
	TWDR=Addr;			// Copy data in TWI data register 
	TWCR=(1<<TWEN)|(1<<TWINT);	// Enable TWI and clear interrupt flag 
	while(!(TWCR&(1<<TWINT)));	// Wait until TWI finish its current job 
	                 		              // Read TWI status register 
		
	while((TWSR & 0xF8)!=0x18);		
		
}

void I2C_Write_Data(unsigned char data)
{
	 
	TWDR = data;				// Copy data in TWI data register 
	TWCR=(1<<TWEN)|(1<<TWINT);	// Enable TWI and clear interrupt flag 
	while(!(TWCR&(1<<TWINT)));	// Wait until TWI finish its current job 	
	
	while((TWSR & 0xF8)!=0x28);
	
	
}


void I2C_Stop()			// I2C stop function 
{
	
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);// Enable TWI, generate stop 
	while(TWCR&(1<<TWSTO));	// Wait until stop condition execution 
	
}


int main(void)
{
	
	DDRB&=~(1<<PINB2);		//Switch made as INPUT
	PORTB|=(1<<PINB2);		//Switch is pulled to HIGH by default


	DDRB|=(1<<PINB0);	   //Led set as OUTPUT
	
	I2C_Init();
	
	int button;
	
			
	while(1)
	{
		I2C_Start(); 
		I2C_Write_Addr(0x20); 
		button = PINB&(1<<2);
		
		if(button)
		{
			PORTB|=(1<<PINB0);
			I2C_Write_Data(0b00001111);
		}
		
		else
		{
			PORTB&=~(1<<PINB0);
			I2C_Write_Data(0b11110000);
		}
		
		I2C_Stop(); 
		
	}
    
}


//____________________________________________________________________________________________________________


//SLAVE I2C

/*void I2C_Init(unsigned char Slave_Addr)
{
	
	TWAR = Slave_Addr;
	
}

void I2C_Read_Data()			
{
	
	unsigned char data;
	
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT))); 
	
	while((TWSR & 0xF8)!= 0x80); 
	data = TWDR;
	
	
	if(data == 0b00001111)
	PORTB = 1<<PINB0;
	else
	PORTB&=~(1<<PINB0); 
	
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
	DDRB|=(1<<PINB0);		//Led is set as OUTPUT	
	I2C_Init(0x20);
	
	while(1)
	{
		
		I2C_Match_ACK();
		
		
		I2C_Read_Data();
		
	}
	
	
}*/
