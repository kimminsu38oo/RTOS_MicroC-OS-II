/*
 * LED, FND, Switch + ISR를 활용한 카운터 프로그램
 * 스위치(SW1)을 누르면 FND숫자가 1 늘어나고 LED 왼쪽 shift 
 * 스위치(SW2)를 누르면 FND숫자가 1 줄어들고 LED 오른쪽 shift
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
unsigned char digit[10]={0x3f, 0x06, 0x5b,
						 0x4f, 0x66, 0x6d,
						 0x7d, 0x27, 0x7f, 0x6f};

unsigned char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};
unsigned char count = 0;

ISR(INT4_vect)
{
	count++;
}

ISR(INT5_vect)
{
	if(count > 0) {
		count--;
	}
}
void LED() {
	unsigned char j = 0x01;
	j = j<<(count)%8;
	if(j == 0) {
		j = 0x01;
	}
	PORTA = j;
}
void display_fnd() {
	int i, fnd[4];
	
	fnd[3] = (count/1000)%10; 
	fnd[2] = (count/100)%10; 
	fnd[1] = (count/10)%10;
	fnd[0] = count%10;

	for(i=0; i<4; i++) {
		PORTC = digit[fnd[i]];
		PORTG = fnd_sel[i];
		_delay_ms(2);
	}
}
int main()
{
	DDRA = 0xff; // PORTA(LED) output
	
	DDRC = 0xff; // PORTC(7-Segment) output
	DDRG = 0x0f; // PORTG(select bit) output
	
	DDRE = 0xcf; // PORTE(SWITCH) input
	EICRB = 0x0A; // 0b00001010 falling edge trigger
	EIMSK = 0x30; // enable SW1, SW2 interrupt
	SREG |= 1<<7; // enable global interrupt
	while (1)
	{
		LED();
		display_fnd();
	}
}

