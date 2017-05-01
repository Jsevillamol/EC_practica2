/*-------------------------------------------------------------------
**
**  Fichero:
**    utils.c
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automática
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Contiene las implementaciones de funciones auxiliares
**
**  Notas de diseño:
**    Equivale al fichero homónimo usado en Fundamentos de Computadores 
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "utils.h"
#define MCLK 64000000

//--------------------------------SYSTEM---------------------------------//
static int delayLoopCount=400;

void Delay(int time)
// time=0: adjust the Delay function by WatchDog timer.//
// time>0: the number of loop time//
// 100us resolution.//
{
	int i,adjust=0;
	if(time==0)
	{
		time=200;
		adjust=1;
		delayLoopCount=400;
		rWTCON=((MCLK/1000000-1)<<8)|(2<<3);	// 1M/64,Watch-dog,nRESET,interrupt disable//
		rWTDAT=0xffff;
		rWTCNT=0xffff;
		rWTCON=((MCLK/1000000-1)<<8)|(2<<3)|(1<<5); // 1M/64,Watch-dog enable,nRESET,interrupt disable //
	}
	for(;time>0;time--)
		for(i=0;i<delayLoopCount;i++);
	if(adjust==1)
	{
		rWTCON=((MCLK/1000000-1)<<8)|(2<<3);
		i=0xffff-rWTCNT;   //  1count/16us?????????//
		delayLoopCount=8000000/(i*64);	//400*100/(i*64/200)   //
	}
}

void sys_init( void )
{
	rWTCON  = 0;		// deshabilita el watchdog
	rINTMSK = ~0;		// enmascara todas las interrupciones

	// Configuracion del gestor de reloj
	rLOCKTIME = 0xfff;	// estabilizacion del PLL = 512 us
	rPLLCON   = 0x38021;	// MCLK = 64MHz
	rCLKSLOW  = 0x8;		// MCLK_SLOW = 500 KHz
	rCLKCON   = 0x7ff8;	// modo NORMAL y reloj distribuido a todos los controladores

	// Configuracion del arbitro de bus
	// prioridad fija por defecto LCD > ZDMA > BDMA > IRQ
     rSBUSCON = 0x8000001b;

	// Configuracion de cache
	rSYSCFG = 0;				// deshabilitada

	// Configuracion del controlador de interrupciones
	rI_PMST = 0x1f1b;	// prioridades fijas por defecto
	rI_PSLV = 0x1b1b1b1b;

	rEXTINTPND = ~0;		// borra las interrupciones externas pendientes por linea EINT7..4
    rI_ISPC = ~0;			// borra todas las interrupciones pendientes
}


