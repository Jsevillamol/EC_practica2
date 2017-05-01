#include "44b.h"
#include "uart.h"


void uart0_init( void )
{
		
// COMPLETAR: inicializar la UART para que:
	// FIFOs activadas
   	// Control de flujo manual
	// normal (no infrarojos), sin paridad, 1 bit de stop, 8 bits de datos
	// 115200 baudios asumiendo 64MHz
	// Tx int/polling, Rx int/polling






}

void uart0_putchar( char ch )
{
	//COMPLETAR: espera mientras la cola FIFO de transmision este llena.  Cuando la cola FIFO de transmision no este llena escribir el caracter en el registro de transmision




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
