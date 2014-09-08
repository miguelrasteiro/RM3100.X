/* 
 * File:   uart.h
 * Author: MiguelRasteiro
 *
 * Created on 25 de Agosto de 2014, 11:57
 */
#include <plib.h>

#ifndef UART_H
#define	UART_H


#define UART_MODULE_ID UART1 // PIM is connected to Explorer through UART2 module

void SendDataBuffer(const char *buffer, UINT32 size);
UINT32 GetMenuChoice(void);

#endif	/* UART_H */

