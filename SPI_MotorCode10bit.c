/*
 * SPI_MotorCode10bit.c
 *
 * Created: 10-Feb-20 8:44:53 PM
 * Author : user
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

//TRANSMITTER CODE

/*int p1,p2;
int x,y;
int x1,y2;

void SPI_MasterInit(void)
{
	//Made MOSI SS and SCK as OUTPUT
	DDRB |= (1<<DDB5) | (1<<DDB7) | (1<<DDB4);
	
	DDRB&=~(1<<DDB6);
	
	//Enabled SPI, set this as Master, set FOSC/16 as SCK frequency
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
}

void SPI_MasterTransmit(char data)
{
	//Write data to SPI data register. Writing to SPDR starts transmission
	SPDR = data;
	
	//Wait for transmission to be complete
	while(!(SPSR & (1<<SPIF)));
	
	
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

int main(void)
{
	
	SPI_MasterInit();
	
	DDRA = 0b00000000;    //ADC as input
	
	//DDRB|= (1<<PINB0);
	//DDRB|= (1<<PINB1);
	
	
	ADMUX=(1<<REFS0);      // Selecting internal reference voltage
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);   //Pre-scalar of 64
	ADMUX&=~(1<<ADLAR);			// for left adjust value
	
	
	while (1)
	{
		
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
			SPI_MasterTransmit(x1);
			//_delay_us(100);
			SPI_MasterTransmit(p2);
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
			SPI_MasterTransmit(y2);
			//_delay_us(100);
			SPI_MasterTransmit(p2);
			//_delay_us(100);
			
			//ADMUX&=0b11111000;
			
			ADMUX = 0b01000000;
			break;
			
			default:
			//Default code
			break;
			
			
		}
		
		ADCSRA|=(1<<ADSC);
		
		
		

	}
	
}*/

//___________________________________________________________________________________________________________________________

//RECEIVER CODE

int X,Y;
int x2,y2;
int result1,result2,resultf;

void SPI_SlaveInit(void)
{
	//Made MOSI SCK and SS as INPUT
	DDRB&=~((1<<DDB5)|(1<<DDB7)|(1<<DDB4));
	
	//Made MISO as OUTPUT
	DDRB|=(1<<DDB6);
	
	//Enable SPI
	SPCR |= (1<<SPE)|(1<<SPR0);
	
}

char SPI_ReceiveSlave(void)
{
	
	//SPDR = 0b00000000;
	
	//Waiting for receive
	while(!(SPSR & (1<<SPIF)));
	
	
	return SPDR;
	
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

int main(void)
{
	
	SPI_SlaveInit();
	
	DDRB|= (1<<PINB0);
	DDRB|= (1<<PINB1);
	
	PWM_init();			// Enabling PWM
	
	while(1)
	{
		_delay_us(280);
		result1 = SPI_ReceiveSlave();
		_delay_us(280);
		result2 = SPI_ReceiveSlave();
		_delay_us(280);
		resultf = (result1<<4)|result2;
		
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
	
	
	
	
	


