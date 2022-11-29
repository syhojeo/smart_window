#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char flag;
volatile unsigned int flag1;


ISR(USART1_RX_vect)
{
	flag1=UDR1;
}
ISR(USART0_RX_vect)
{
	flag=UDR0;
}
void init_serial(void); // 시리얼통신 초기설정
void init_port(void); // 포트 활성화 및 초기화
void led_control(void); // 받아온 데이터를 이용한 led제어
void putch(char data); // 데이터 송신 준비가 되면 레지스터에 데이터값 저장
void puts(char *str); //문자열을 레지스터에 저장


void main(void){
	
	init_serial();
	init_port();
	
	
	
	
	while(1){
		led_control();
		if(!(UCSR0A & (1<<UDRE0))){
		UDR0 ="22";
		}
		_delay_ms(500);
	}
}

void init_serial(void){
	UCSR0A = 0x00; //초기화
	UCSR0B = 0x98; // 송수신 허용,버퍼인터럽트금지
	UCSR0C = 0x06; //데이터 전송비트수 8비트로 설정
	
	UBRR0H= 0x00;
	UBRR0L = 103; //Baud rate 9600
	//usart1
	UCSR1A = 0x00; //초기화
	UCSR1B = 0x98; // 송수신 허용,버퍼인터럽트금지
	UCSR1C = 0x06; //데이터 전송비트수 8비트로 설정
	
	UBRR1H= 0x00;
	UBRR1L = 103;
	SREG = 0x80;
}

void init_port(void){
	DDRB= 0xff;
	PORTB = 0x00;
}

void led_control(void){
	if(flag=='1')
	{
		PORTB=0x01;
		
	}
	if(flag=='0')
	{
		PORTB=0x00;
		
	}
}


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


//usart1
