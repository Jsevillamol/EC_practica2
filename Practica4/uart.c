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


	//[7] RESERVADO
	//[6] = 0 sin infrarrojos
	//[5:3] = 000 sin paridad (0xx)
	//[2] = 0 un bit de stop
	//[1:0] = 11 8 bits de datos
	rULCON0 = (rULCON0 & 0x40) | 3;


	//[7:6] trigger level of transmit FIFO
	//[5:4] trigger level receive FIFO
	//[3] RESERVADO
	//[2] = Tx FIFO reset
	//[1] = Rx FIFO reset
	//[0] = 1 FIFO mode
	rUFCON0 = rUFCON0 | 1;


	//[7:5] RESERVADO (000)
	//[4] = 0 control de flujo manual
	//[3:1] = RESERVADO (000)
	//[0] =
	rUMCON0 = 0;

	//[2] no lo se
	//[1] mira en el UFSTAT pq esta en modo Rx..
	rUTRSTAT0 = 0;

	//(int)(64MHz/(115200 x 16) + 0.5) - 1
	rUBRDIV0 = 34;

}

void uart0_putchar( char ch )
{
	//COMPLETAR: espera mientras la cola FIFO de transmision este llena.
	//Cuando la cola FIFO de transmision no este llena escribir el caracter en el registro de transmision

	while(rUFSTAT0 & 1<<9);
	rUTXH0 = ch;


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
