/*
* usart.c
*
* Created: 15.02.2019 22:19:24
*  Author: Markus Bröker<broeker.markus@googlemail.com>
*/

#include <usart.h>

/************************************************************************/
/* SETUP USART | 115200 | 8N1                                           */
/************************************************************************/
void USART_Init(unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
	
	/*
	UCSR0C         mode            parity       stop       size*     polarity
	+-------------------+---------------+-------+-----------------+---------+
	| UMSEL01 : UMSEL00 | UPM01 : UPM00 | USBS0 | UCSZ01 : UCSZ00 |  UPOL0  |
	+-------------------+---------------+-------+-----------------+---------+
	UPM0[1:0]   none(00) even(10) odd(01)
	USBS0       1bit(0)   2bit(1)
	UCSZ0[2:0]  5bit(000) 6bit(001) 7bit(010) 8bit(011) 9bit(111)
	*/
	
	/* Frame Format: 1 stop bit | 8 data */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
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
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

/************************************************************************/
/* Retrieve a string                                                    */
/************************************************************************/
static char* USART_Receive_String(void)
{
	static char buffer[BUF_SIZE];
	int i = 0;
	
	char c = 0;
	while (c != '\n' || (i > sizeof(buffer) - 3)) {
		c = USART_Receive_Character();
		buffer[i++] = c;
	}
	
	buffer[i++] = '\r';
	buffer[i++] = '\n';
	buffer[i++] = '\0';
	
	return buffer;
}

/************************************************************************/
/* Write a line of text                                                 */
/************************************************************************/
static int USART_WriteLine(char *s)
{
	int len = 0;
	while (*s != 0x0) {
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
char *SerialGetString ()
{
	return USART_Receive_String();
}