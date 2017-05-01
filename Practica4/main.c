/*-------------------------------------------------------------------
**
**  Fichero:
**    main.c
**
**-----------------------------------------------------------------*/


#include "44b.h"
#include <stdio.h>
#include "dma.h"
#include "uart.h"
#include "lcd.h"
#include "utils.h"
#include "intcontroller.h"
#include "timer.h"
#include "gpio.h"
#include "button.h"

/* Notas de diseño:
**    - El buffer de memoria del LCD esta en 0x0c200000 (es programable) y es de tamaño 0x9600B
**    - El LCD tiene una resolucion de 320x240px y en memoria cada byte contiene 2pixeles
**    - Las 36 imagenes a visualizar consecutivamente en el LCD están
**      ubicadas a partir de la dirección 0x0c400000 cada 0x10000B,
**      cada una ocupa 0x9600B y se indican en el fichero load_img.text
*/

int nImg = 0;

void timer0_isr( void ) __attribute__ ((interrupt ("IRQ")));

void putImageNoDMA( unsigned int imgDir )
{
	unsigned int i;
	unsigned char *src, *dst;

	src = (unsigned char *) imgDir;
	dst = (unsigned char *) 0x0c200000;

	for( i=0; i<0x9600; i++ )
		dst[i] = src[i];
}


void timer0_isr( void )
{
	// COMPLETAR:
	unsigned int whicheint = rEXTINTPND;
	unsigned int buttons = (whicheint >> 2) & 0x3;

	int imgDir = 0x0c400000 + nImg*0x10000B;

	if (buttons & BUT2) {
		//transferencia sin DMA
		putImageNoDMA(imgDir);
	} else {
		//transferencia DMA
		putImageDMA(imgDir);
	}

	nImg = (nImg + 1) % 36;

	ic_cleanflag(INT_TIMER0);
}


int setup(void)
{
	// Inicializa el sistema, la UART0, el LCD y el DMA
    sys_init();
    uart0_init();
    lcd_init();
    zdma0_init();

    // COMPLETAR:
    //Configurar los puertos de la GPIO que sean necesarios






    // Configurar el controlador de interrupciones para que genere interrupciones vectorizadas por la línea IRQ para el timer0




    // Configurar el timer0 para que genere interrupciones cada 0,1segundos




    //Registrar la ISR



	return 0;
}



void main( void )
{
unsigned int num = 0;

setup();
 
while( 1 )
    {
	uart0_putint( num++ );
	uart0_putchar( '\n' );
    }

}
