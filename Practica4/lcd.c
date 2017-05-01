
#include "44b.h"
#include "lcd.h"

void lcd_init( void )
{      
	
	// indica al controlador de LCD que el buffer LCD esta la dirección 0x0c200000 y es de tamaño 0x9600B y con reordenado de bytes;

		rLCDSADDR1 = 0x16100000; 	//direccion inicial
		rLCDSADDR2 = 0x20104B00;	// direccion final
		rLCDSADDR3 = 0x50;

		rLCDCON2  = 0x13cef; 	// resolucion de 320 x 240 pixeles
		rLCDCON3  = 0; 			// self-refresh desactivado
		rLCDCON1   = 0xc021; 	// refresco a 120 Hz

	// Parámetros de dithering: los recomendados
		rDITHMODE = 0x12210;
		rDP1_2    = 0xa5a5;
		rDP4_7    = 0xba5da65;
		rDP3_5    = 0xa5a5f;
		rDP2_3    = 0xd6b;
		rDP5_7    = 0xeb7b5ed;
		rDP3_4    = 0x7dbe;
		rDP4_5    = 0x7ebdf;
		rDP6_7    = 0x7fdfbfe;

		// Configuracion de paletas: no es necesario porque el LCD es en blanco y negro
		rREDLUT   = 0;
		rGREENLUT = 0;
		rBLUELUT  = 0;

}
