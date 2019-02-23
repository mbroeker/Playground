/*
* Playground
*	Arduino UNO R3 (ATmega328P) with Atmel Studio 7.0
*   USB ISP-Programmer for ATMEL(Tremex, STK500)
*	Hantek 6022BE Oscilloscope
*	Hantek 6022BL Logic Analyzer
*   Voltcraft VC 444
*
*
* Created: 14.02.2019 22:16:43
* Author : Markus Bröker<broeker.markus@googlemail.com>
*/

#include <usart.h>
#include <tools.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMEOUT 120

/************************************************************************/
/* Timer0 | NO PWM_POSSIBLE                                             */
/************************************************************************/
void Counter_TimSK0_Init()
{
	// Prescaler F_CPU / 256 = 64k	for Timer 0
	TCCR0A = 0;
	TCCR0A |= (1 << WGM01);
	
	TCCR0B = 0;
	TCCR0B |= (1 << CS02);
	
	// Counter from 0 - 255
	OCR0A = 0xff;
	
	// Enable Overflow Capture Interrupt on TIM0
	TIMSK0 |= (1 << OCIE0A);
}

/************************************************************************/
/* Timer1 | PWM_POSSIBLE                                                */
/************************************************************************/
void Counter_TimSK1_Init()
{
	// Prescaler F_CPU / 256 = 64k	for Timer 1
	TCCR1A = 0;
	TCCR1A |= (1 << WGM11);
	
	TCCR1B = 0;
	TCCR1B |= (1 << CS12);
	
	// Counter from 0 - 128
	OCR1A = 128;
	
	// Enable Overflow Capture Interrupt on TIM1
	TIMSK1 |= (1 << OCIE1A);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
ISR (TIMER0_COMPA_vect)
{
	static int counter = 0;
	static int iteration = 0;
	
	if (counter > TIMEOUT) {
		iteration++;
		
		if ((iteration % 20) == 0) {
			USART_Printf("ROUND %d\n", iteration);
		}
		
		// Toggle PIN for Hantek 6022BE
		Toggle_Bit(PORTB, 5);
		
		counter = 0;
	}
	
	counter++;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
ISR (TIMER1_COMPA_vect)
{
	static int counter = 0;
	
	if (counter > TIMEOUT) {
		counter = 0;
	}
	
	counter++;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int main(void)
{
	USART_Init(FREQ);
	
	// Init GPIO LED
	Set_Bit(DDRB, DDB5);
	
	cli();
	Counter_TimSK0_Init();
	Counter_TimSK1_Init();
	sei();
	
	char buffer[BUF_SIZE];

	USART_Printf("LISTENING ON SERIAL CONSOLE WITH 38400 Bd|8|N|1\n");
	
	int len = 0;
	while (1) {
		len = USART_ReadLine((char*)&buffer, BUF_SIZE);
		
		if (len > 0) {
			USART_Printf("RECEIVED: %s", buffer);
			*buffer = 0;
		}
	}

	return EXIT_SUCCESS;
}
