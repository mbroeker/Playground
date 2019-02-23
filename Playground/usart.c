/*
* usart.c
*
* Created: 15.02.2019 22:19:24
*  Author: Markus Bröker<broeker.markus@googlemail.com>
*/

#include <usart.h>
#include <ctype.h>

/************************************************************************/
/* SETUP USART | 38400 max for rx and tx | 8N1                          */
/************************************************************************/
void USART_Init(unsigned int ubrr)
{
	// Set Baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	UCSR0C = 0
	| (1<<UCSZ01)|(1<<UCSZ00)        // 8
	| (0<<UPM00)|(0<<UPM01) 		 // N
	| (0<<USBS0)                     // 1
	;
}

/************************************************************************/
/* Send a Single Character                                              */
/************************************************************************/
static void USART_Transmit_Character(unsigned char c)
{
	/* Wait for empty transmit buffer */
	while ( !(UCSR0A & (1<<UDRE0)))
	;
	/* Put data into buffer, sends the data */
	UDR0 = c;
}

/************************************************************************/
/* Retrieve a Single Character                                          */
/************************************************************************/
static unsigned char USART_Receive_Character(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)))
	;
	/* Get and return received data from buffer */
	return UDR0;
}

/************************************************************************/
/* Write a line of text                                                 */
/************************************************************************/
static int USART_WriteLine(char *s)
{
	int len = 0;
	while (*s != '\0') {
		USART_Transmit_Character(*s);
		s++;
		len++;
	}
	
	return len;
}

/************************************************************************/
/* Printf for Serial Console                                            */
/************************************************************************/
int SerialPrintf (const char *format, ...)
{
	char buffer[BUF_SIZE];
	
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, BUF_SIZE, format, args);
	va_end(args);
	
	return USART_WriteLine(buffer);
}

/************************************************************************/
/* GetString for Serial Console                                         */
/************************************************************************/
int SerialGetString (char *buffer, int bufSize)
{
	int i = 0;
	unsigned char c = '\0';
	while (c != '\n' && i < bufSize - 1) {
		c = USART_Receive_Character();
		if (isascii(c)) {
			buffer[i++] = c;
		}
	}
	
	buffer[i] = '\0';
	
	return i;
}