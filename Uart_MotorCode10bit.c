/*
 * MotorCode_Uart10bit.c
 *
 * Created: 05-Feb-20 6:29:27 PM
 * Author : user
 */ 


#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>


// TRANSMITTER CODE

/*int p1,p2;
void TransmitUART(unsigned char data)
{
	
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
	
}


int x;
int y;
int X_val;
int Y_val;


int x1,x2;
int y2,y3;


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
	
	//UART initialization
  
  int ubrrvalue = 25;
	
	UBRRH = (unsigned char)(ubrrvalue>>8);
	UBRRL = (unsigned char)ubrrvalue;
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRC = (1<<URSEL) | (1<<USBS) | (3<<UCSZ0);
	
	
	
	DDRA = 0b00000000;    //ADC as input
	
	DDRB|= (1<<PINB0);
	DDRB|= (1<<PINB1);
	
	
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
			
			TransmitUART(x1);
			TransmitUART(p2);
			
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
			
			TransmitUART(y2);
			TransmitUART(p2);
			
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

//____________________________________________________________________________________________________________________

//RECEIVER CODE

int result;
int X;
int Y;
int x2;
int y2;


unsigned char ReceiveUART(void)
{
	while(!(UCSRA & (1<<RXC)));
	return UDR;
	
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
	
	DDRB|= (1<<PINB0);
	DDRB|= (1<<PINB1);
	
	PWM_init();			// Enabling PWM
	
	//UART initialization
	
	int ubrrvalue = 25;
	
	UBRRH = (unsigned char)(ubrrvalue>>8);
	UBRRL = (unsigned char)ubrrvalue;
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRC = (1<<URSEL) | (1<<USBS) | (3<<UCSZ0);
	
	
	while(1)
	{
		result = ReceiveUART();	
		
		
		if((result&0b00000001)==0b00000001)			//X values
		{
			x2 = ReceiveUART();
			X = (x2<<4)|result;
			
			X = mapped(X,0,1023,-1023,1023);
			X = X/4;
			
		}
		else if((result&0b00000001)==0b00000000)	//Y values
		{
			y2 = ReceiveUART();
			Y = (y2<<4)|result;
			
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

