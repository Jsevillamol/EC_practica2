/*-----------------------------------------------------------------
**
**  Fichero:
**    init.asm  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Arranca un programa en C
**
**  Notas de dise�o:
**
**---------------------------------------------------------------*/

    .extern main
    .extern timer_ISR
    .extern ic_cleanflag
    .extern button_ISR
	.extern ic_cleanflag
	.global start
	.global irq_ISR

	/*
	** Modos de operaci�n
	*/
	.equ MODEMASK, 0x1f		/* Para selecci�n de M[4:0] del CPSR */
	.equ USRMODE,  0x10
	.equ FIQMODE,  0x11
	.equ IRQMODE,  0x12
	.equ SVCMODE,  0x13
	.equ ABTMODE,  0x17
	.equ UNDMODE,  0x1b
	.equ SYSMODE,  0x1f

	/*
	** Direcciones de las bases de las pilas del sistema 
	*/
	.equ USRSTACK, 0xc7ff000   	
	.equ SVCSTACK, 0xc7ff100
	.equ UNDSTACK, 0xc7ff200
	.equ ABTSTACK, 0xc7ff300
	.equ IRQSTACK, 0xc7ff400
	.equ FIQSTACK, 0xc7ff500

	/*
	** Registro de m�scara de interrupci�n
	*/
	.equ rINTMSK,    0x1e0000c
	.equ rI_ISPC,    0x1e00024
	.equ rF_ISPC,    0x1e0003c
	.equ rEXTINTPND, 0x1d20054

	//anadimos un alias del registo I_ISPR
	.equ rI_ISPR, 0x1e00020
	.equ thirteen, 0x2000;
	.equ twentyone, 0x200000;

start:

	/* Pasa a modo supervisor */
    mrs	r0, cpsr
    bic	r0, r0, #MODEMASK
    orr	r1, r0, #SVCMODE
    msr	cpsr_c, r1 

	/* Inicialización de la sección bss a 0, estándar C */
    ldr	    r3, =Image_ZI_Base
	ldr	    r1, =Image_ZI_Limit	/* Top of zero init segment */
    mov	    r2, #0
L0:
    cmp	    r3, r1	    		/* Zero init */
    strcc   r2, [r3], #4
    bcc	    L0
	/****************************************************/

	/* Desde modo SVC inicializa los SP de todos los modos de ejecuci�n privilegiados */
    bl InitStacks

	ldr r0, =rEXTINTPND
	ldr r1, =0xff
	str r1, [r0]
	ldr r0, =rI_ISPC
	ldr r1, =0x1fffffff
	str r1, [r0]
	ldr r0, =rF_ISPC
	ldr r1, =0x1fffffff
	str r1, [r0]

	/* Enmascara interrupciones */
	ldr r0, =rINTMSK
	ldr r1, =0x1fffffff
    str r1, [r0]

	/* Habilita linea IRQ y FIQ del CPSR */
	mrs r0, cpsr
	bic r0, r0, #0xC0
	msr cpsr_c, r0
	
	/* Desde modo SVC cambia a modo USR e inicializa el SP_usr */
	mrs r0, cpsr
	bic r0, r0, #MODEMASK
	orr r1, r0, #USRMODE  
	msr cpsr_c, r1
	ldr sp, =USRSTACK

    mov fp, #0

    bl main

End:
    B End

InitStacks:
	mrs r0, cpsr
	bic r0, r0, #MODEMASK

	orr r1, r0, #UNDMODE  /* desde modo SVC cambia a modo UND e inicializa el SP_und */
	msr cpsr_c, r1    
	ldr sp, =UNDSTACK

	orr r1, r0, #ABTMODE  /* desde modo UND cambia a modo ABT e inicializa el SP_abt */
	msr cpsr_c, r1 
	ldr sp, =ABTSTACK

	orr r1, r0, #IRQMODE  /* desde modo ABT cambia a modo IRQ e inicializa el SP_abt */
	msr cpsr_c, r1
	ldr sp, =IRQSTACK

	orr r1, r0, #FIQMODE  /* desde modo IRQ cambia a modo FIQ e inicializa el SP_fiq */
	msr cpsr_c, r1
	ldr sp, =FIQSTACK

	orr r1, r0, #SVCMODE  /* desde modo FIQ cambia a modo SVC e inicializa el SP_svc */
	msr cpsr_c, r1
	ldr sp, =SVCSTACK
	
    mov pc, lr

/*
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
*/

irq_ISR:
	//Prologo
	push {r0-r10,fp,ip,lr}
	add fp, sp, #(4*13-4)

	//Cuerpo
	ldr r1, =rI_ISPR
	ldr r0, [r1]

	and r2, r0, #thirteen
	cmp r2, #thirteen
	beq timer

	and r2, r0, #twentyone
	cmp r2, #twentyone
	beq button

	b else

timer:
	bl timer_ISR
	mov r0, #13
	bl ic_cleanflag
button:
	bl button_ISR
	mov r0, #21
	bl ic_cleanflag
else:

	//Epilogo
	sub sp, fp, #(4*13-4)
	pop {r0-r10,fp,ip,lr}
	subs pc, lr, #4

	.end

