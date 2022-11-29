#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "millis.h"


void main(void){
	DDRE = 0xff;
	DDRF = 0xff;
	
	init_lcd();
	init_dust();
	
	writestring_lcd (0,0,"welcome to robot");
	writestring_lcd (0,1, "you r vip");
	while(1);
}