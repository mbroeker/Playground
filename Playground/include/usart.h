/*
* usart.h
*
* Created: 15.02.2019 22:19:38
*  Author: Markus Bröker<broeker.markus@googlemail.com>
*/

#ifndef USART_H_
#define USART_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 38400
#endif

#define FREQ F_CPU/16/BAUD

#define BUF_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void USART_Init(unsigned int);
int USART_Printf(const char *format, ...);
int USART_ReadLine(char *buffer, int bufSize);
int USART_ReadBytes(char *buffer, int bufSize);

#endif /* USART_H_ */