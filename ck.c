/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Professional
Automatic Program Generator
© Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 5/22/2023
Author  : NeVaDa
Company : 
Comments: 


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega16.h>
#include <string.h>
#include <delay.h>

#ifndef RXB8
#define RXB8 1
#endif

#ifndef TXB8
#define TXB8 0
#endif

#ifndef UPE
#define UPE 2
#endif

#ifndef DOR
#define DOR 3
#endif

#ifndef FE
#define FE 4
#endif

#ifndef UDRE
#define UDRE 5
#endif

#ifndef RXC
#define RXC 7
#endif

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

// USART Receiver buffer
#define RX_BUFFER_SIZE 8
char rx_buffer[RX_BUFFER_SIZE];

#if RX_BUFFER_SIZE <= 256
unsigned char rx_wr_index = 0;
unsigned char rx_rd_index,rx_counter;
#else
unsigned int rx_wr_index,rx_rd_index,rx_counter;
#endif
int LED[8];
 //NGAT NGOAI
interrupt [EXT_INT0] void ext_int0_isr(void)
{
DDRB=0X00;
DDRC=0X00;  
}
interrupt [EXT_INT1] void ext_int1_isr(void)
{ 

DDRB=0XFF;
DDRC=0XFF;
}
//

 void uart_char_send(unsigned char chr){
    while (!(UCSRA & (1<<UDRE))) {}; //Wait for empty transmit buffer - UDRE=1  
            UDR=chr;
}
//Send a string to the USART
void uart_string_send(unsigned char *txt){
    unsigned char n, i;            
    n=strlen(txt); // Count the number of characters
          for (i=0; i<n; i++){
            uart_char_send(txt[i]); //Send data
            }
}


// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSRA;
data=UDR;

rx_buffer[rx_wr_index]=data;
rx_wr_index += 1;
if (data == '?') rx_wr_index=0;
   // xu ly
  
   if(strcmp(rx_buffer, "a0?") == 0)
   {     
     PORTB.0 = 1;   
   }
   else if(strcmp(rx_buffer, "a1?") == 0)
   {  
     PORTB.0 = 0;
   }
   else if(strcmp(rx_buffer, "b0?") == 0)
   {     
      PORTB.1=1;
   }
   if(strcmp(rx_buffer, "b1?") == 0)
   {  
     PORTB.1 = 0;
   }
   else if(strcmp(rx_buffer, "c0?") == 0)
   {     
     PORTB.2=1;   
   }
   else if(strcmp(rx_buffer, "c1?") == 0)
   {  
     PORTB.2 = 0;
   }
   else if(strcmp(rx_buffer, "d0?") == 0)
   {     
     PORTB.3=1;   
   }
   else if(strcmp(rx_buffer, "d1?") == 0)
   {  
     PORTB.3 = 0;
   }
   else if(strcmp(rx_buffer, "e0?") == 0)
   {     
     PORTB.4=1;  
   }
   else if(strcmp(rx_buffer, "e1?") == 0)
   {  
     PORTB.4=0;
   }
   else if(strcmp(rx_buffer, "f0?") == 0)
   {     
     PORTB.5=1;
   }
   else if(strcmp(rx_buffer, "f1?") == 0)
   {  
     PORTB.5 =1;
   }
   else if(strcmp(rx_buffer, "g0?") == 0)
   {     
     PORTB.6 = 1;   
   }
   else if(strcmp(rx_buffer, "g1?") == 0)
   {  
     PORTB.6 = 0;
   }
   else if(strcmp(rx_buffer, "h0?") == 0)
   {     
     PORTB.7 =  1;   
   }
   else if(strcmp(rx_buffer, "h1?") == 0)
   {  
     PORTB.7 = 0;
   } 
 else if(strcmp(rx_buffer, "sp?") == 0)
   { 
     DDRB=0X00;
     DDRC=0X00;    
   }
   else if(strcmp(rx_buffer, "st?") == 0)
   {  
    DDRB=0Xff;
    DDRC=0Xff;
   }
   
}


#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter==0);
data=rx_buffer[rx_rd_index++];
#if RX_BUFFER_SIZE != 256
if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
#endif
#asm("cli")
--rx_counter;
#asm("sei")
return data;
}
#pragma used-
#endif

// Standard Input/Output functions
#include <stdio.h>

#define ADC_VREF_TYPE 0xE0

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCH;
}
// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
PORTC.1=~PORTC.1;
TCNT1H=0X85;   
TCNT1L=0XED;
}

//
unsigned int ADC_val;
unsigned char str[10];

//////////////////////////////////////////////////
void main(void)
{
DDRB = 0x00;
DDRC=0X00;
//Extenal Interrupt
GICR|=(1<<INT1) | (1<<INT0) | (0<<INT2);
MCUCR=(1<<ISC11) | (0<<ISC10) | (1<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(1<<INTF1) | (1<<INTF0) | (0<<INTF2);

//Timer
TCCR1B=0x04;
TIMSK=0X04;
TCNT1H=0X85;
TCNT1L=0XED;
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=0x00;
UCSRB=0x98;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x33;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: AREF pin
// ADC Auto Trigger Source: ADC Stopped
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x83;

// Global enable interrupts
#asm("sei")

while (1)
      {
      ADC_val = read_adc(1);
      sprintf(str, "%u\n", ADC_val);
      uart_string_send(str);     
      delay_ms(1000);
      }
}
