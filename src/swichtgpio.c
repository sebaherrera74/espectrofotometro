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

#include "swichtgpio.h"
#include "sapi.h"
/*=====[Macros de definicion de constantes privadas]=========================*/
/*=====[Macros estilo funcion privadas]======================================*/
/*=====[Definiciones de tipos de datos privados]=============================*/
// Tipo de datos que renombra un tipo basico
// Tipo de datos de puntero a funcion
// Tipo de datos enumerado
// Tipo de datos estructua, union o campo de bits
/*=====[Definiciones de Variables globales publicas externas]================*/
/*=====[Definiciones de Variables globales publicas]=========================*/
/*=====[Definiciones de Variables globales privadas]=========================*/
/*=====[Prototipos de funciones privadas]====================================*/
//static void funPrivada(void);
/*=====[Implementaciones de funciones publicas]==============================*/

void swichtgpio_init(swichtgpio_t* swicht,gpioMap_t gpio){

	//Inicializo pines
	swicht->normalabierto=gpio;

	//configuro como entrada Pull-down
	gpioConfig(swicht->normalabierto,GPIO_INPUT_PULLUP); //configuro pin como input pull-up
	swicht->estadoswicht=abierto;                        //En estado incicial esta abierot el swicth

}

swichtgpio_estado_swicht swichtgpioEstado(swichtgpio_t* swicht){

	if (!gpioRead(swicht->normalabierto)){
		swicht->estadoswicht=cerrado;
		return swicht->estadoswicht;
	}
	else{
		swicht->estadoswicht=abierto;
		return swicht->estadoswicht;
	}
}









/*=====[Implementaciones de funciones de interrupcion publicas]==============*/
/*=====[Implementaciones de funciones privadas]==============================*/

