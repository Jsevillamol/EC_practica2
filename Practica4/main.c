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
**      ubicadas a partir de la dirección 0x0c400000 cada 0x10000 Bytes,
**      cada una ocupa 0x9600B y se indican en el fichero load_img.text
*/

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

int nImg = 0;
void timer0_isr( void ) {
	// COMPLETAR:
	int dest = 0x0c400000 + nImg*0x10000;
	if (read_button() & 0b1) {
		putImageNoDMA(dest);
	} else {
		putImageDMA(dest);
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
    //Boton: puerto G
    portG_conf(6, INPUT);
	portG_eint_trig(6, FALLING);
	portG_conf_pup(6, ENABLE);
	//LCD: puerto D
	rPDATD   = ~0;
	rPCOND 	= 0xAAAA;
	rPUPD 	= 0xFF;
	//UART: puerto E
	rPDATE   = ~0;
	rPCONE 	= 0x255A9;
	rPUPE 	= 0x1FB;

    // Configurar el controlador de interrupciones para que genere interrupciones vectorizadas por la línea IRQ para el timer0
	ic_conf_irq(ENABLE, VEC);
	ic_conf_fiq(DISABLE);
	ic_conf_line(INT_TIMER0, IRQ);
	ic_enable(INT_TIMER0);

    // Configurar el timer0 para que genere interrupciones cada 0,1segundos
	tmr_set_prescaler(TIMER0, 255);
	tmr_set_divider(TIMER0, D1_8);
	tmr_set_count(TIMER0, 6250, 1);
	tmr_update(TIMER0);
	tmr_set_mode(TIMER0, RELOAD);
	tmr_stop(TIMER0);

	tmr_start(TIMER0);

    //Registrar la ISR
	pISR_TIMER0 = timer0_isr;


	return 0;
}


void main( void ){
unsigned int num = 0;

setup();
 
while( 1 ) {
	uart0_putint( num++ );
	uart0_putchar( '\n' );
    }
}
