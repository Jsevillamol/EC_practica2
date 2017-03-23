#include <stdio.h>
#include "44b.h"
#include "button.h"
#include "leds.h"
#include "utils.h"
#include "D8Led.h"
#include "intcontroller.h"
#include "timer.h"
#include "gpio.h"

struct RLstat {
	int moving;
	int speed;
	int direction;
	int position;
};

static struct RLstat RL = {
	.moving = 0,
	.speed = 5,
	.direction = 0,
	.position = 0,
};

void irq_ISR(void) __attribute__ ((interrupt ("IRQ")));

void timer_ISR(void)
{
	//COMPLETAR: tomar el código de avance de posición del led rotante de la
	//primera parte

	if (RL.moving) {
		// COMPLETAR: debemos hacer avanzar el led rotante una posición en
		// la dirección indicada por el campo direction de la variable RL.
		// La posición actual está representada en el campo position.
		// Recordar que queremos un movimiento circular, representado por
		// las 6 primeras posiciones en el array Segmentes del display de 8
		// segmentos, por lo que position debe estar siempre entre 0 y 5.
		if (RL.direction == 0)
			RL.position =  (RL.position + 1) % 6;
		else
			RL.position =  (RL.position==0) ? 5 : (RL.position - 1);
		D8Led_segment(RL.position);
	}
}

void button_ISR(void)
{
	unsigned int whicheint = rEXTINTPND;
	unsigned int buttons = (whicheint >> 2) & 0x3;

	//COMPLETAR: usar el código de la primera parte parte de atención a los
	//pulsadores

	//unsigned int buttons = read_button();

	if (buttons & BUT1) {
		// COMPLETAR: utilizando la interfaz para los leds definida en leds.h
		// hay que conmutar el led1
		// También hay que conmutar la dirección del movimiento del led rotante
		// representado por el campo direction de la variable RL
		led1_switch();
		RL.direction = (RL.direction == 1) ?0 : 1;
	}

	if (buttons & BUT2) {
		// COMPLETAR: utilizando la interfaz para los leds definida en leds.h
		// hay que conmutar el led2
		// También hay que conmutar el estado de movimiento del led rotante
		// representado por el campo moving de la variable RL, y en caso de
		// ponerlo en marcha debemos reiniciar el campo iter al valor del campo
		// speed.
		led2_switch();
		if (RL.moving == 0){
			RL.moving = 1;
			tmr_start(TIMER0);
		}
		else{
			RL.moving = 0;
		}
	}

	// eliminamos rebotes
	Delay(2000);
	// borramos el flag en extintpnd
	rEXTINTPND |= 0x3; //COMPLETAR: debemos borrar las peticiones de interrupción en
		               //EXTINTPND escribiendo un 1 en los flags que queremos borrar (los
				       //correspondientes a los pulsadores pulsados)
}

void irq_ISR(void)
{
	int bit = rI_ISPR;

	//COMPLETAR: debemos ver si la interrupción que debemos atender (bit) es la
	//de la línea INT_TIMER0 and INT_EINT4567. Si es la del timer se invocará la
	//función timer_ISR y después se borrará el flag de interrupción utilizando
	//el interfaz definido en intcontroller.h. Si es la de EINT4567 se invocará
	//la función button_ISR y se borrará el flag correspondiente utilizando el
	//mismo interfaz.

	if (bit & (1<<13)){
		timer_ISR();
		ic_cleanflag(INT_TIMER0);
	} else if (bit & (1<<21)){
		button_ISR();
		ic_cleanflag(INT_EINT4567);
	}

}

int setup(void)
{
	leds_init();
	D8Led_init();
	D8Led_segment(RL.position);

	/* Port G: configuración para generación de interrupciones externas */

	//COMPLETAR: utilizando el interfaz para el puerto G definido en gpio.h
	//configurar los pines 6 y 7 del puerto G para poder generar interrupciones
	//externas por flanco de bajada por ellos y activar las correspondientes
	//resistencias de pull-up.

	portG_conf(6, EINT);
	portG_eint_trig(6, FALLING);
	portG_conf_pup(6, ENABLE);
	portG_conf(7, EINT);
	portG_eint_trig(7, FALLING);
	portG_conf_pup(7, ENABLE);

	/********************************************************************/

	/* Configuración del timer */

	//COMPLETAR: utilizando el interfaz para los timers definido en timer.h
	//configurar el timer 0: 
	//      valor de prescalado a 255
	//      valor del divisor 1/8
	//      valor de cuenta 62500 y cualquier valor de comparación entre 1 y 62499
	//      actualizar el contador con estos valores (update)
	//      poner el contador en modo RELOAD
	//      dejar el contador parado

	tmr_set_prescaler(0, 255);
	tmr_set_divider(0, 2);
	tmr_set_count(TIMER0, 62500, 1);
	tmr_update(TIMER0);
	tmr_set_mode(TIMER0, RELOAD);
	tmr_stop(TIMER0);

	if (RL.moving)
		tmr_start(TIMER0);

	/***************************/

	// Registramos la ISR
	pISR_IRQ = irq_ISR;

	//COMPLETAR: registrar irq_ISR como rutina de tratamiento de
	//interrupción para la linea IRQ

	/* Configuración del controlador de interrupciones
	 * Habilitamos la línea IRQ, en modo no vectorizado
	 * y registramos una ISR para la línea IRQ
	 * Configuramos el timer 0 en modo IRQ y habilitamos
	 * esta línea
	 * Configuramos la línea EINT4567 en modo IRQ y la
	 * habilitamos
	 */

	ic_init();
	//COMPLETAR: utilizando el interfaz definido en intcontroller.h
	//		habilitar la línea IRQ en modo no vectorizado
	//		deshabilitar la línea FIQ
	//		configurar la línea INT_TIMER0 en modo IRQ
	//		configurar la línea INT_EINT4567 en modo IRQ
	//		habilitar la línea INT_TIMER0
	//		habilitar la línea INT_EINT4567

	ic_conf_irq(ENABLE, NOVEC);
	ic_conf_fiq(DISABLE);
	ic_conf_line(INT_TIMER0, IRQ);
	ic_conf_line(INT_EINT4567, IRQ);
	ic_enable(INT_TIMER0);
	ic_enable(INT_EINT4567);

	/***************************************************/

	Delay(0);
	return 0;
}

int loop(void)
{
	return 0;
}


int main(void)
{
	setup();

	while (1) {
		loop();
	}
}
