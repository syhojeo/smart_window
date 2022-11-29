#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"


void main(void){
	DDRE = 0xff;
	DDRF = 0xff;
	
	init_lcd();
	writestring_lcd (0,0,"welcome to robot");
	writestring_lcd (0,1, "you r vip");
	while(1);
}