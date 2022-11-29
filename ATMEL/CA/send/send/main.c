#include <avr/io.h>
#define F_CPU 16000000UL 
#include <util/delay.h>

void putch(char data){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void puts(char *str){
	while(*str!=0){
		putch(*str);
		str++;
	}
}

void init(void){
	UCSR0A = 0x00;
	UCSR0B = 0b00011000;
	UCSR0C = 0b00000110;
	UBRR0H = 0x00;
	UBRR0L = 0x67;
}

void main(){
	char Hello[] = "20";
	
	
	init();
	
	while(1){
		
		puts(Hello);
		_delay_ms(3000);
	}
}