#include "44b.h"
#include "uart.h"

void uart0_init( void )
{// COMPLETAR: inicializar la UART para que:
	rUFCON0 |= 0x1;// FIFOs activadas
	rUMCON0 &= ~(1 << 4);// Control de flujo manual
	rULCON0 &= ~(1 << 6);
	rULCON0 &= ~(1 << 2);
	rULCON0 &= ~(1 << 3);
	rULCON0 |= 11;// normal (no infrarojos), sin paridad, 1 bit de stop, 8 bits de datos
	rUBRDIV0 = 34;// 115200 baudios asumiendo 64MHz
	rUCON0 |= 0x1;
	rUCON0 &= ~(1 << 1);
	rUCON0 |= 0x2;
	rUCON0 &= ~(1 << 3);//Tx int/polling, Rx int/polling
}

void uart0_putchar( char ch )
{//COMPLETAR: espera mientras la cola FIFO de transmision este llena.
 //Cuando la cola FIFO de transmision no este llena escribir el caracter en el registro de transmision
	while(rUFSTAT0 & (0x1 << 9)){}
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
