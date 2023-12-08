/*
 * project.c
 *
 *  Created on: 15 Sept 2023
 *      Author: Zeyad El-banna
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
unsigned char sec_1 = 0;
unsigned char min_1 = 0;
unsigned char hour_1 = 0;
unsigned char sec_2 = 0;
unsigned char min_2 = 0;
unsigned char hour_2 = 0;
void Timer1_init(void);
void int0_init(void);
void int1_init(void);
void int2_init(void);
int main (void)
{
    DDRC |= 0x0F;
    PORTC &= 0xF0;
    DDRA |= 0x3F;
    PORTA &= 0xC0;
    DDRD &= ~(1<<PD2) & ~(1<<PD3);
    DDRB &= ~(1<<PB2);
    PORTD |= (1<<PD2);
    PORTB |= (1<<PB2);
    Timer1_init();
    int0_init();
    int1_init();
    int2_init();
	while(1)
	{
		PORTA = 0x01 ;
		PORTC = ( PORTC & 0xF0 ) | sec_1 ;
		_delay_ms(3);
		PORTA = 0x02 ;
		PORTC = ( PORTC & 0xF0 ) | sec_2 ;
		_delay_ms(3);
		PORTA = 0x04 ;
		PORTC = ( PORTC & 0xF0 ) | min_1 ;
		_delay_ms(3);
		PORTA = 0x08;
		PORTC = ( PORTC & 0xF0 ) | min_2 ;
		_delay_ms(3);
		PORTA = 0x10 ;
		PORTC = ( PORTC & 0xF0 ) | hour_1 ;
		_delay_ms(3);
		PORTA = 0x20;
		PORTC = ( PORTC & 0xF0 ) | hour_2 ;
		_delay_ms(3);
 	}
	return 0;
}
void Timer1_init(void)
{
	TCCR1A = (1<<FOC1A) | (1<<WGM12);
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS11);
	TCNT1 = 0;
	OCR1A = 15625 ;
	TIMSK |= (1<<OCIE1A) ;
	SREG |= (1<<7);
}
ISR (TIMER1_COMPA_vect)
{
	sec_1 ++ ;
	if(sec_1 > 9)
	{
		sec_1 = 0;
		sec_2 ++ ;
		if(sec_2 == 6)
		{
			sec_2 = 0 ;
			sec_1 = 0;
			min_1 ++;
		}
	}
	if(min_1 > 9)
	{
		min_1 = 0;
		min_2 ++ ;
        if(min_2 == 6)
        {
        	min_1 = 0 ;
        	min_2 = 0;
        	hour_1 ++ ;
        	if(hour_2 == 2 && hour_1 == 4)
        			{
        				hour_1 = 0;
        				hour_2 = 0;
        			}

        }

	}
	if(hour_1 > 9)
	{
		hour_1 = 0;
		hour_2 ++ ;
	}
}
void int0_init(void)
{
	SREG |= (1<<7);
	MCUCR |= (1<<ISC01) ;
	GICR |= (1<<INT0);
}
void int1_init(void)
{
	SREG |= (1<<7);
	MCUCR |= (1<<ISC10) | (1<<ISC11);
	GICR |= (1<<INT1);
}
void int2_init(void)
{
	SREG |= (1<<7);
	MCUCSR &= ~(1<<ISC2);
	GICR |= (1<<INT2);
}
ISR(INT0_vect)
{
	sec_1 = 0;
	sec_2 = 0;
	min_1 = 0;
	min_2 = 0;
	hour_1 = 0;
	hour_2 = 0;
}
ISR(INT1_vect)
{
	TCCR1B &= ~(1<<CS10) & ~(1<<CS11) & ~(1<<CS12);
}
ISR(INT2_vect)
{
	TCCR1B |= (1<<CS10) | (1<<CS11);
}
