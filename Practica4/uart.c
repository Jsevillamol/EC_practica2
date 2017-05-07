#include "44b.h"
#include "uart.h"


void uart0_init( void )
{
// COMPLETAR: inicializar la UART para que:
// FIFOs activadas //BIEN
// Control de flujo manual //SUPONGAMOS QUE BIEN
// normal (no infrarojos), sin paridad, 1 bit de stop, 8 bits de datos //BIEN
// 115200 baudios asumiendo 64MHz //BIEN
// Tx int/polling, Rx int/polling //SUPONGAMOS QUE BIEN

	rULCON0 = (rULCON0 & 0b01000000) | 3;
	rUFCON0 |= 1;
	rUMCON0 = 0;
	rUTRSTAT0 = 0;
	rUBRDIV0 = 34;
}

void uart0_putchar( char ch )
{
	//COMPLETAR: espera mientras la cola FIFO de transmision este llena.
	//Cuando la cola FIFO de transmision no este llena escribir el caracter en el registro de transmision

	while(rUFSTAT0 & 0b1<<9);
	WrUTXH0(ch);
}

void uart0_puts( char *s )
{
    while( *s )
		uart0_putchar( *s++ );
}

void uart0_putint( unsigned int i )
{
	char buf[10 + 1];    /* Array con espacio suficiente para los digitos y el '\0' */
	char *p = buf + 10;	 /* Puntero al final del array */

	*p = '\0';

	do {
    	*--p = '0' + (i % 10);
		i /= 10;
	} while( i );

	uart0_puts( p );
}
