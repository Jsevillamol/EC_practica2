#include "44b.h"
#include "dma.h"

void zdma0_init( void )
{
	// Habilita peticion de DMA externa, sin comandos sw
	// Inicializa direcion fuente, destino y cuenta a cero.
	
	rZDCON0  = 0; // Habilita peticion de DMA externa, sin comandos sw
	rZDISRC0 = 0; //Dir inicio
	rZDIDES0 = 0; //Dir destino
	rZDICNT0 = 0; //Cuenta 0
}


void putImageDMA( unsigned int imgDir )
{

// COMPLETAR: Programar una transferencia DMA que copie desde la direccion origen hasta la direccion destino.
//	Para ello hay que configurar lo siguiente:
// La direccion de inicio es imgDir
// La direccion destino es la direccion del buffer de video
// Los datos son de 32b (tamaño word)
// Ambas direcciones con modo post-incrementada
// El modo de DMA es: whole service, unit transfer, polling mode, no autoreload, enable DMA
// start DMA	

	rZDISRC0 = 0b1001<<28 | imgDir;
	rZDIDES0 = 0b1001<<28 | 0x0c200000;
	rZDICNT0 = 0b1001<<26 | 1<<20 | 0x9600;
	rZDCON0 = (rZDCON0 & 0xFFFFFFFD) | 1;

}
