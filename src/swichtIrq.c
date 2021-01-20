/*=====[Swicht posicion cero]=====================================================
 * Copyright 2021 Sebastian Herrera <sebaherrera@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.1
 * Fecha de creacion: 2021/01/18
 */

/*=====[Inclusion de su propia cabecera]=====================================*/

#include "swichtIrq.h"
#include "chip.h"       // <= Biblioteca LPCOpen, capa chip (del fabricante NXP)
/*=====[Inclusiones de dependencias de funciones privadas]===================*/

#include "sapi.h"


/*=====[Macros de definicion de constantes privadas]=========================*/
// { {6,11}, FUNC0, {3, 7} },   // 23   DIO23   CON2_38   GPIO7

//{ {6,10}, FUNC0, {3, 6} },   //  29   CON2_35   GPIO6

// GPIO6 { {PinNamePortN ,PinNamePinN}, PinFUNC, {GpioPortN, GpioPinN} }
#define GPIO6_GPIO_PORT  3
#define GPIO6_GPIO_PIN   6

// Interrupt
#define PININT6_INDEX         6                  // PININT index used for GPIO mapping
#define PININT6_IRQ_HANDLER   GPIO6_IRQHandler   // GPIO interrupt IRQ function name
#define PININT6_NVIC_NAME     PIN_INT6_IRQn      // GPIO interrupt NVIC interrupt name

/*------------------------------------------------------------*/


// GPIO7 { {PinNamePortN ,PinNamePinN}, PinFUNC, {GpioPortN, GpioPinN} }
#define GPIO7_GPIO_PORT  3
#define GPIO7_GPIO_PIN   7

// Interrupt
#define PININT7_INDEX         7                  // PININT index used for GPIO mapping
#define PININT7_IRQ_HANDLER   GPIO7_IRQHandler   // GPIO interrupt IRQ function name
#define PININT7_NVIC_NAME     PIN_INT7_IRQn      // GPIO interrupt NVIC interrupt name




/*=====[Macros estilo funcion privadas]======================================*/



/*=====[Definiciones de tipos de datos privados]=============================*/

// Tipo de datos que renombra un tipo basico


// Tipo de datos de puntero a funcion


// Tipo de datos enumerado

// Tipo de datos estructua, union o campo de bits

/*=====[Definiciones de Variables globales publicas externas]================*/



/*=====[Definiciones de Variables globales publicas]=========================*/

uint8_t swicht_cero = 0;

/*=====[Definiciones de Variables globales privadas]=========================*/



/*=====[Prototipos de funciones privadas]====================================*/

void PININT7_IRQ_HANDLER(void);
void PININT6_IRQ_HANDLER(void);

//static void funPrivada(void);

/*=====[Implementaciones de funciones publicas]==============================*/

void swichtIrq_init(swichtIrq_t *swicht,gpioMap_t normalabierto){

	//Inicializo pines
	swicht->normalabierto=normalabierto;

	//configuro como entrada Pull-down
	gpioConfig(swicht->normalabierto,GPIO_INPUT_PULLUP);


	//Inicializo interrupciones de gpio 6y 7

	// ---> Comienzo de funciones LPCOpen para configurar la interrupcion-GPIO6

	// Configure interrupt channel for the GPIO pin in SysCon block
	Chip_SCU_GPIOIntPinSel( PININT6_INDEX, GPIO6_GPIO_PORT, GPIO6_GPIO_PIN);

	// Configure channel interrupt as edge sensitive and falling edge interrupt
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT6_INDEX));


	//
	Chip_PININT_SetPinModeLevel(LPC_GPIO_PIN_INT, PININTCH(PININT6_INDEX));

	/*como esta en modo pullup el pin esta siempre en alto
	 * y de esta manera cuando el pin baja a cero
	 */
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(PININT6_INDEX));

	// Enable interrupt in the NVIC
	NVIC_ClearPendingIRQ( PININT6_NVIC_NAME);
	NVIC_EnableIRQ( PININT6_NVIC_NAME);


	/*	// ---> Comienzo de funciones LPCOpen para configurar la interrupcion-GPIO7

	// Configure interrupt channel for the GPIO pin in SysCon block
	Chip_SCU_GPIOIntPinSel( PININT7_INDEX, GPIO7_GPIO_PORT, GPIO7_GPIO_PIN);

	// Configure channel interrupt as edge sensitive and falling edge interrupt
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT7_INDEX));
	Chip_PININT_SetPinModeLevel(LPC_GPIO_PIN_INT, PININTCH(PININT7_INDEX));

	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(PININT7_INDEX));
	Chip_PININT_ClearRiseStates(LPC_GPIO_PIN_INT, PININTCH(PININT7_INDEX));
	// Enable interrupt in the NVIC
	NVIC_ClearPendingIRQ( PININT7_NVIC_NAME);
	NVIC_EnableIRQ( PININT7_NVIC_NAME);*/
}

uint8_t swichtIrqEstado(swichtIrq_t *swicht){

	if (swicht_cero==0){
		swicht->estadoswicht=otralongitudonda;
	}

	else{
		swicht->estadoswicht=longitudondacero;
	    //swicht_cero=0;
	}

	switch (swicht->estadoswicht){
	case otralongitudonda:

		return 0;
		break;

	case longitudondacero:
		swicht_cero=0;
		return 1;
		break;
	default:
	break;
	}
}









/*=====[Implementaciones de funciones de interrupcion publicas]==============*/
/*=====[Implementaciones de funciones privadas]==============================*/
void PININT6_IRQ_HANDLER(void) {
	// Se da aviso que se trato la interrupcion
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT6_INDEX));
	gpioToggle(LEDB);
	// Se realiza alguna accion.
	swicht_cero=1;                //Aviso que hay una interrupcion del swicht

}

/*void PININT7_IRQ_HANDLER(void) {

		// Se da aviso que se trato la interrupcion
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT7_INDEX));

	// Se realiza alguna accion.
	gpioToggle(LEDB);

	}*/



