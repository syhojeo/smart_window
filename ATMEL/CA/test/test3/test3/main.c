#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>


#define LCD_DATA_DIR DDRF
#define LCD_DATA_PORT PORTF
#define LCD_CTRL_DIR DDRC
#define LCD_CTRL_PORT PORTC

#define LCD_RS_HIGH (LCD_CTRL_PORT |= 0x01)
#define LCD_RS_LOW (LCD_CTRL_PORT &= 0xfe)
#define LCD_E_HIGH (LCD_CTRL_PORT |= 0x02)
#define LCD_E_LOW (LCD_CTRL_PORT &= 0xfd)

void delay(int t){
	for(int i=0; i<t; i++){
		_delay_ms(1);
	}
}

void E_pulse(void){
	LCD_E_HIGH;
	delay(1);
	LCD_E_LOW;
}

void command_set(char s){
	LCD_RS_LOW;
	LCD_DATA_PORT =s;
	E_pulse();
}

void data_set(char s){
	LCD_RS_HIGH;
	LCD_DATA_PORT = s;
	E_pulse();
}

void cursor_at(char x, char y){
	switch(y){
		case 0 : y = 0x80 ;break;
		case 1: y= 0xc0; break;
		case 2: y = 0x94; break;
		case 3: y = 0xd4; break;
	}
	y = y+x;
	command_set(y);
}

void writestring_lcd(char x ,char y, char *str){
	cursor_at(x,y);
	while(*str){
		data_set(*str++);
	}
}

void init_lcd(void){
	command_set (0x38);
	command_set(0x0f);
	command_set(0x06);
}

void main(){

	DDRE = 0xff;
	DDRF = 0xff;
	
	init_lcd();
	writestring_lcd(0,0,"Welcome to Robot");
	writestring_lcd(0,1,"You r Vip");
	while(1);
}