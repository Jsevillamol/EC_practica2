
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

	ZDISRC0 = (0b10<<30)|(0b01 << 28)|imgDir;
	ZDCSRC0 = ZDISRC0;

	ZDIDES0 = (0b10<<30)|(0b01 << 28)|0x0c200000;
	ZDCDES0 = ZDIDES0;
	
	//Dudas en 30:31, 24:25, 0:19
	ZDICNT0 = (0b00 << 30)|(0b10 << 28)|(0b01 << 26)|(0b00 << 24)|
			 (0b00 << 22)|(0b0  << 21)|(0b1  << 20)|(0x9600 )
	ZDCCNT0 = ZDICNT0;

}
