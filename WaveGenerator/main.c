/*
* Wave Generator for
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
#include <cdc_device.h>
#include <tools.h>
#include <sinus_wave.h>

/************************************************************************/
/* Main Module                                                          */
/************************************************************************/
int main(void)
{
	// Initialize USART(38400|8|N|1)
	USART_Init(FREQ);
	
	// DIGITAL LED ON PB5 as INPUT
	Set_Bit(DDRB, 5);
	
	SerialPrintf("WAVE GENERATOR READY\n");

	while (1)
	{
		// Generate a Sinus Wave
		for (uint8_t x = 0; x < 255; x++) {
			uint8_t value = sinusWave(x);
			
			if (value > 128) {
				Set_Bit(PORTB, DDB5);
				} else {
				Toggle_Bit(PORTB, DDB5);
			}
			
			_delay_us(10);
		}
		
		_delay_ms(10);
	}

	return EXIT_SUCCESS;
}
