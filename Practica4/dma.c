
#include "44b.h"
#include "dma.h"

void zdma0_init( void )
{
	// Habilita peticion de DMA externa, sin comandos sw
	// Inicializa direcion fuente, destino y cuenta a cero.
	
	rZDCON0  = 0;
	rZDISRC0 = 0;
	rZDIDES0 = 0;
	rZDICNT0 = 0;
}


void putImageDMA( unsigned int imgDir )
{

// COMPLETAR: Programar una transferencia DMA que copie desde la direccion origen hasta la direccion destino. Para ello hay que configurar lo siguiente:
// La direccion de inicio es imgDir
// La direccion destino es la direccion del buffer de video
// Los datos son de 32b (tamaño word)
// Ambas direcciones con modo post-incrementada
// El modo de DMA es: whole service, unit transfer, polling mode, no autoreload, enable DMA
// start DMA	






}
