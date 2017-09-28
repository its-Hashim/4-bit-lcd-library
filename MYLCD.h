#ifndef MYLCD
#define MYLCD

#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>

#define rs PD3
#define rw PD2
#define en PD1

void printWord(char *word);
void lcd_init();
void sendCommand(char);
void printCharecter(char);
void sendCommadByNibbles(char);
void sendDataByNibbles(char);
void clearFirstLine(void);
void clearSecondLine(void);
void clear(void);
void setCursor(uint8_t x,  uint8_t y);
void printInt(int);
char doubleCheck(double);
void printDouble(double k);

void lcd_init() // fuction for intialize
{
	sendCommand(0x02); // to initialize LCD in 4-bit mode. or to go to home
	sendCommand(0x28); //to initialize LCD in 2 lines, 5X7 dots and 4bit mode.
	sendCommand(0x0C);//display on cursor off
	sendCommand(0x06);//entry mode
	//sendCommand(0x0F);//display on cursor blink
	sendCommand(0x80);
}

void sendCommand(char cmd_value)
{
	char cmd_value1;
	
	cmd_value1 = cmd_value & 0xF0; //mask lower nibble because PA4-PA7 pins are used.
	sendCommadByNibbles(cmd_value1); // send to LCD
	
	cmd_value1 = ((cmd_value<<4) & 0xF0); //shift 4-bit and mask
	sendCommadByNibbles(cmd_value1); // send to LCD
}

void printCharecter(char data_value)
{
	char data_value1;
	
	data_value1=data_value&0xF0;
	sendDataByNibbles(data_value1);
	
	data_value1=((data_value<<4)&0xF0);
	sendDataByNibbles(data_value1);
}

void sendCommadByNibbles(char cmdout)
{
	PORTB=cmdout;
	PORTD&=~(1<<rs);
	PORTD&=~(1<<rw);
	PORTD|=(1<<en);
	_delay_ms(1);
	PORTD&=~(1<<en);
}

void sendDataByNibbles(char dataout)
{
	PORTB=dataout;
	PORTD|=(1<<rs);
	PORTD&=~(1<<rw);
	PORTD|=(1<<en);
	_delay_ms(1);
	PORTD&=~(1<<en);
}

void printWord(char *word)
{
	
	while(*word != '\0')
	{
		printCharecter(*word++);
	}
	_delay_ms(2);
}

void setCursor(uint8_t x,  uint8_t y)
{
	char logicForYaxis[2]={0,64};
	x-=1;
	y-=1;
	sendCommand(0x80 + logicForYaxis[y] + x);
}

void clear()
{
	setCursor(1,1);
	printWord("                ");
	setCursor(1,2);
	printWord("                ");
	setCursor(1,1);
}

void clearSecondLine()
{
	setCursor(1,2);
	printWord("                ");
	setCursor(1,2);
}

void clearFirstLine()
{
	setCursor(1,1);
	printWord("                ");
	setCursor(1,1);
}

void printInt(int k)
{
	char str[10];
	itoa(k,str,10);
	printWord(str);
}

char doubleCheck(double k)
{
	char l=0;
	if(k<0)
	{
		k=k*-1;
		l+=1;
	}
	if(k<10)
	l+=4;
	else if(k<100)
	l+=5;
	else if(k<1000)
	l+=6;
	else if(k<10000)
	l+=7;
	else if(k<100000)
	l+=8;
	else if(k<1000000)
	l+=9;
	else if(k<10000000)
	l+=10;
	else
	l+=0;
	return l;
}

void printDouble(double k)
{
	char str[10];
	char l;
	l=doubleCheck(k);
	if(l==0)
	{
		printWord("Double error");
		_delay_ms(10000);
	}
	else
	{
		dtostrf(k,l,2,str);
		printWord(str);
	}
}

#endif