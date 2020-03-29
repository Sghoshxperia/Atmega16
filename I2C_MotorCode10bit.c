/*
 * I2c_MotorCode_10bit.c
 *
 * Created: 25-Feb-20 6:07:39 PM
 * Author : user
 */ 



#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

//TRANSMITTER CODE

/*int p1,p2;
int x,y;
int x1,y2;

void I2C_Init()
{
	TWBR = 0x0C;					// Setting TWBR0 as 1
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

void I2C_Repeated_Start(char read_address) // I2C repeated start function 
{
	//uint8_t status;		// Declare variable 
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);// Enable TWI, generate start 
	while(!(TWCR&(1<<TWINT)));	// Wait until TWI finish its current job 
	//status=TWSR&0xF8;		// Read TWI status register 
	//if(status!=0x10)		// Check for repeated start transmitted 
	while((TWSR & 0xF8)!= 0x10);			// Return 0 for repeated start condition fail 
	TWDR=read_address;		// Write SLA+R in TWI data register 
	TWCR=(1<<TWEN)|(1<<TWINT);	// Enable TWI and clear interrupt flag 
	while(!(TWCR&(1<<TWINT)));	// Wait until TWI finish its current job 
	
}

void TWI_repeated_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
	while((TWSR & 0xF8)!= 0x10); // Check for the acknoledgement
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


void I2C_Stop()			// I2C stop function
{
	//PORTB = 1<<PINB0;
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);// Enable TWI, generate stop
	while(TWCR&(1<<TWSTO));	// Wait until stop condition execution
	//PORTB = 1<<PINB0;
}

unsigned int ADC_read(unsigned char chnl)
{
	chnl&=0b00000111; // select adc channel between 0 to 7
	ADMUX|=chnl;
	
	
	ADCSRA|=(1<<ADSC);   // start conversion
	while(!(ADCSRA & (1<<ADIF)));   // wait for ADIF conversion complete return
	ADCSRA|=(1<<ADIF);   // clear ADIF when conversion complete by writing 1
	
	uint8_t val = ADCL;
	uint16_t value = (ADCH<<8)|val;
	
	
	
	
	return (value); //return calculated ADC value
}



int main()
{
	I2C_Init();
	
	DDRA = 0b00000000;    //ADC as input
	
	//DDRB|= (1<<PINB0);
	//DDRB|= (1<<PINB1);
	
	
	ADMUX=(1<<REFS0);      // Selecting internal reference voltage
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);   //Pre-scalar of 64
	ADMUX&=~(1<<ADLAR);			// for left adjust value
	
	
	while (1)
	{
		
		I2C_Start();
		I2C_Write_Addr(0x20);
		
		switch(ADMUX)
		{
			
			case 0b01000000:
			
			x = ADC_read(0);
			
			p1 = (x&0b00001111);
			x = (x>>4);
			p2 = (x&0b00111111);
			//x2 = p2&0b11111110;
			
			x1 = (p1|0b00000001);
			
			//_delay_us(100);
			I2C_Write_Data(x1);
			//I2C_Stop();
			//I2C_Start();
			//I2C_Write_Addr(0x20);
			//_delay_ms(1);
			//I2C_Start();
			//_delay_us(100);
			I2C_Repeated_Start(0x20);
			//I2C_Write_Addr(0x20);
			I2C_Write_Data(p2);
			//_delay_us(100);
		
			
			//ADMUX&=0b11111000;
			
			ADMUX = 0b01000001;
			break;
			
			case 0b01000001:
			
			y = ADC_read(1);
			
			p1 = (y&0b00001111);
			y = (y>>4);
			p2 = (y&0b00111111);
			y2 = (p1&0b11111110);
			//y3 = p2&0b11111110;
			
			
			//_delay_us(100);
			I2C_Write_Data(y2);
			//I2C_Stop();
			//_delay_ms(1);
			//I2C_Start();
			//I2C_Write_Addr(0x20);
			//_delay_us(100);
			I2C_Repeated_Start(0x20);
			
			I2C_Write_Data(p2);
			//_delay_us(100);
			
			ADMUX = 0b01000000;
			break;
			
			default:
			//Default code
			break;
			
			
		}
		
		ADCSRA|=(1<<ADSC);
		
		I2C_Stop();	
		

	}
	
	
}*/

//________________________________________________________________________________________________________________________

//RECEIVER CODE

int X,Y;
int x2,y2;
int result1,result2,resultf;

void I2C_Init(unsigned char Slave_Addr)
{
	//PORTB = 1<<PINB0;
	TWAR = Slave_Addr;
	//PORTB = 1<<PINB0;
}

uint8_t I2C_Read_Data()			//unsigned char -> void
{
	//PORTB = 1<<PINB0;
	unsigned char data;
	//uint8_t status;
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT))); //PORTB = 1<<PINB0;
	//status = TWSR&0xF8;
	//while(status!=0x80);
	while((TWSR & 0xF8)!= 0x80); //PORTB = 1<<PINB0;
	data = TWDR;
	return data;
	
	
	
}

void I2C_Match_ACK()
{
	//PORTB = 1<<PINB0;
	while((TWSR & 0xF8)!= 0x60)
	{
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		while(!(TWCR&(1<<TWINT)));
		
	}
	//PORTB = 1<<PINB0;
}

int mapped(int z,float input_l,float input_h,float output_l,float output_h)
{
	
	float mapf = (z - input_l)/(input_h-input_l)*(output_h-output_l)+output_l;
	int mapfdone = (int)mapf;
	
	
	return(mapfdone);
	
}

void PWM_init()
{
	
	TCCR0 = (1<<WGM01)|(1<<WGM00) |  (1<<COM01) |(1<<CS00);
	TCCR2 = (1<<WGM21)|(1<<WGM20) | (1<<COM21)| (1<<CS02);
	DDRB|=(1<<PINB3); //OCR0 PINB3 Led pwm led
	DDRD|=(1<<PIND7); //OCR2 PIND7 Right pwm led
	
}



int main()
{
	
	I2C_Init(0x20);
	
	DDRB|= (1<<PINB0);
	DDRB|= (1<<PINB1);
	
	PWM_init();
	
	while(1)
	{
		I2C_Match_ACK();
		//_delay_us(435);
		result1 = I2C_Read_Data();
		_delay_ms(3);
		I2C_Match_ACK();
		//_delay_us(435);
		result2 = I2C_Read_Data();
		//_delay_us(400);
		resultf = (result2<<4)|result1;
		
		//_delay_us(100);
		
		
		if((resultf&0b00000001)==1)			//X values
		{
			
			//x2 = SPI_ReceiveSlave();
			//_delay_us(100);
			//X = (x2<<4)|result;
			
			X = resultf;
			X = mapped(X,0,1023,-1023,1023);
			X = X/4;
			
		}
		else if((resultf&0b00000001)==0)	//Y values
		{
			//y2 = SPI_ReceiveSlave();
			//_delay_us(100);
			//Y = (y2<<4)|result;
			
			Y = resultf;
			Y = mapped(Y,0,1023,-1023,1023);
			Y = Y/4;
			
		}
		
		
		float theta = atan2f(Y,X);
		
		int remap = abs(abs(X)-abs(Y));
		
		
		
		
		
		
		if(abs(X)<60&&abs(Y)<60)
		{
			OCR0 = 0;
			OCR2 = 0;
			PORTB&=~(1<<PINB0);
			PORTB&=~(1<<PINB1);
			_delay_us(10);
		}
		
		else if(theta>0.70f&&theta<0.87f)		
		{
			
			OCR2 = 0;
			PORTB&=~(1<<PINB1);
			OCR0 = abs(X);
			//PORTB|=(1<<PINB0);
			//_delay_us(5);
			
		}
		
		else if(theta>2.23f&&theta<2.47f)
		{
			OCR0 = 0;
			PORTB&=~(1<<PINB0);
			OCR2 = abs(X);
			//PORTB|=(1<<PINB1);
			//_delay_us(5);
		}
		
		else if(theta>-2.40f&&theta<-2.30f)
		{
			OCR2 = 0;
			PORTB&=~(1<<PINB0);
			OCR0 = abs(X);
			//PORTB|=(1<<PINB0);
			//_delay_us(5);
		}
		
		else if(theta>-0.90f&&theta<-0.60f)
		{
			OCR0 = 0;
			PORTB&=~(1<<PINB1);
			OCR2 = abs(X);
			//PORTB|=(1<<PINB1);
		}
		
		else if(theta>0.0f&&theta<0.79f)     
		{
			
			//_delay_us(30);
			
			//analogWrite(LeftF,X_val);
			OCR0 = X;
			PORTB |= (1<<PINB0);
			
			//analogWrite(RightB,remap);
			OCR2 = remap;
			PORTB&=~(1<<PINB1);
			
			//_delay_us(5);
			
		}


		else if(theta>0.79f&&theta<1.57f)     
		{
			
			//_delay_us(30);
			
			//analogWrite(LeftF,Y_val);
			OCR0 = Y;
			PORTB|=(1<<PINB0);
			
			//analogWrite(RightF,remap);
			if(theta>0.70f&&theta<0.87f)
			OCR2 = 0;
			else
			OCR2 = remap;
			PORTB|=(1<<PINB1);
			
			//_delay_us(5);

			
			
		}

		else if(theta>1.57f&&theta<2.36f)   
		{
			
			//_delay_us(30);
			
			//analogWrite(LeftF,remap);					
			
			if(theta>2.23f&&theta<2.47f)
			OCR0 = 0;
			else
			OCR0 = remap;
			
			PORTB|=(1<<PINB0);
			
			//analogWrite(RightF,Y_val);
			OCR2 = Y;
			PORTB|=(1<<PINB1);
			
			//_delay_us(5);
			
			
		}

		else if(theta>2.36f&&theta<3.14f)     
		{
			//_delay_us(30);
			
			//analogWrite(LeftB,remap);
			
			if(theta>=2.23f&&theta<=2.47f)
			OCR0 = 0;
			else
			OCR0 = remap;
			
			PORTB&=~(1<<PINB0);
			
			//analogWrite(RightF,abs(X_val));
			OCR2 = abs(X);
			PORTB|=(1<<PINB1);
			

			//_delay_us(5);
			
			
		}

		else if(theta>-3.14f&&theta<=-2.36f)     
		{
			//_delay_us(30);
			
			//analogWrite(LeftB,abs(X_val));
			OCR0 = abs(X);
			PORTB&=~(1<<PINB0);
			
			//analogWrite(RightF,remap);
			
			if(theta>-2.40f&&theta<-2.30f)
			OCR2 = 0;
			else
			OCR2 = remap;
			PORTB|=(1<<PINB1);
			
			
			//_delay_us(5);
			
			
		}

		else if(theta>-2.36f&&theta<-1.57f)     
		{
			
			//	_delay_us(30);
			
			//analogWrite(LeftB,abs(Y_val));
			OCR0 = abs(Y);
			PORTB&=~(1<<PINB0);
			
			
			//analogWrite(RightB,remap);
			OCR2 = remap;
			PORTB&=~(1<<PINB1);

			
			
			//_delay_us(5);
			
			
		}

		else if(theta>-1.57f&&theta<-0.79f)     
		{
			
			//_delay_us(30);
			
			//analogWrite(LeftB,remap);
			
			//if(remap<40)
			//OCR0 = 0;
			//else
			OCR0 = remap;
			
			
			PORTB&=~(1<<PINB0);
			
			//analogWrite(RightB,abs(Y_val));
			OCR2 = abs(Y);
			PORTB&=~(1<<PINB1);

			//_delay_us(5);
			
		}

		else if(theta>-0.79f&&theta<0.0f)     
		{
			
			//responsible for led off at diagonal
			//_delay_us(30);
			
			//analogWrite(LeftF,remap);
			
			if(theta>-0.90f&&theta<-0.60f)
			OCR0 = 0;
			else
			OCR0 = remap;
			
			
			PORTB|=(1<<PINB0);
			
			//analogWrite(RightB,X_val);
			OCR2 = X;
			PORTB&=~(1<<PINB1);

			//_delay_us(5);
			
			
		}
		
	}
}
	
