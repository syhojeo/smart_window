#include <avr/io.h>

#include <avr/interrupt.h>





volatile short int step = 0;

volatile short int comm = 0;

volatile char* str;

volatile short int initialized = 0;

volatile short int printing = 0;





// 8-bit mode command

volatile short int init_command1[] = {

	0x32, 0x30,

	0x32, 0x30,

	0x32, 0x30,

	0x22, 0x20   // change to 4-bit mode

};



// 4-bit mode command

volatile short int init_command2[] = {

	0x22, 0x20, 0x82, 0x80,  // function set(4-bit, 2 line, 5x7 dot)

	0x02, 0x00, 0xC2, 0xC0,  // display control(display ON, cursor OFF)

	0x02, 0x00, 0x62, 0x60,  // entry mode set(increment, not shift)

	0x02, 0x00, 0x12, 0x10,  // clear display

	0x00, 0x00   // Delay

};







// Function prototype

void CLCD_command();

void CLCD_data();

void CLCD_initialize();

void CLCD_string(short int command, char *string);







ISR(TIMER0_OVF_vect) // Interrupt service routine

{

	PORTB = 0x01;    // Debugging signal

	if (!initialized)

	{

		CLCD_initialize();

	}

	else if(comm)

	{

		CLCD_command();

	}

	else if(printing)

	{

		CLCD_data();

	}

	PORTB = 0x00;    // Debugging signal

}







int main(void)

{

	char line1[] = " 2004 Character LCD";

	char line2[] = "   AVR ATmega128A";

	char line3[] = "  Timer 0 overflow";

	char line4[] = "     interrupt";

	

	// RS: PA0, RW: GND, E: PA1, D4: PA4, D5: PA5, D6: PA6, D7: PA7

	

	DDRA = 0xF3;   // PA[7:4], PA[1:0] output

	DDRB = 0x01;   // PB0 outout

	TCCR0 = 0x07;  // Normal mode, prescaler = 1024

	TIMSK = 0x01;  // overflow interrupt enable

	sei();

	

	// Timer interrupt period: 1024*256/8MHz = 32.768ms

	

	// Timer interrupt period changes during intialization: 32*256/8MHz = 1.024ms

	

	

	CLCD_string(0x80, line1);

	CLCD_string(0x80 + 0x40, line2);

	CLCD_string(0x80 + 0x14, line3);

	CLCD_string(0x80 + 0x54, line4);

	

	

	while(1)

	{

	}

}







void CLCD_initialize() // Initialize character(text) LCD module

{

	if (step < 8)  // 8-bit mode

	{

		PORTA = init_command1[step];

		step++;

	}

	else if (step < 25)  // 4-bit mode

	{

		PORTA = init_command2[step - 8];

		step++;

	}

	else

	{

		initialized = 1;

		step = 0;

	}

	

	if (step == 9)

	{

		TCCR0 = 0x03;  // Timer interrupt period: 32*256/8MHz = 1.024ms

	}

}







void CLCD_command()

{

	// RS: PA0, RW: GND, E: PA1, D4: PA4, D5: PA5, D6: PA6, D7: PA7

	

	switch (step)

	{

		case 0: PORTA = (comm&0xF0) | 0x02;

		break;

		case 1: PORTA = comm&0xF0;

		break;

		case 2: PORTA = ((comm&0x0F)<<4) | 0x02;

		break;

		case 3: PORTA = (comm&0x0F)<<4;

		break;

	}

	

	if (step < 3)

	{

		step++;

	}

	else

	{

		comm = 0;

		step = 0;

	}

	

}







void CLCD_data()

{

	// RS: PA0, RW: GND, E: PA1, D4: PA4, D5: PA5, D6: PA6, D7: PA7

	

	switch (step)

	{

		case 0: PORTA = (*(str + printing - 1)&0xF0) | 0x03;

		break;

		case 1: PORTA = (*(str + printing - 1)&0xF0) | 0x01;

		break;

		case 2: PORTA = ((*(str + printing - 1)&0x0F)<<4) | 0x03;

		break;

		case 3: PORTA = ((*(str + printing - 1)&0x0F)<<4) | 0x01;

		break;

	}

	

	if (step < 3)

	{

		step++;

	}

	else  // check for the next character

	{

		if (*(str + printing) != '\0')

		{

			printing++;   // next character

		}

		else

		{

			printing = 0; // stop

		}

		step = 0;

	}

}







void CLCD_string(short int command, char *string) // display a string on LCD

{

	while(printing != 0);

	comm = command;

	str = string;

	printing = 1;  // Start signal

}
