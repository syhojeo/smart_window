#include <avr/io.h>
#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>

#include "lcd.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;


ISR(TIMER0_OVF_vect); //timer interrupt routine
unsigned long millis(); // from booting  to current time
void init_dust(); //timer set


int main(){
	LCD_DATA_DIR = 0xff;
	LCD_CTRL_DIR = 0xff;
	
	init_dust(); //timerset
	init_lcd(); //lcd set
	char y[] = "you. r";
	/*unsigned long x = millis();
	
	char buf[100];
	
	sprintf(buf,"%d",(int)x);
	
	writestring_lcd(0,0,buf);*/
	writestring_lcd(0,0,y);
	writestring_lcd(0,1,y);
	while(1);
}


ISR(TIMER0_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}


unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;
}

void init_dust()
{
	sei();
	sbi(TCCR0, CS02);
	sbi(TIMSK, TOIE0); //timer interrupt enable
}
