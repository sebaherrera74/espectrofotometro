/*=====[Swicht posicion cero]=====================================================
 * Copyright 2021 Sebastian Herrera <sebaherrera@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.1
 * Fecha de creacion: 2021/01/18
 */

/*=====[Evitar inclusion multiple comienzo]==================================*/

#ifndef _SWICHTGPIO_H_
#define _SWICHTGPIO_H_

/*=====[Inclusiones de dependencias de funciones publicas]===================*/

#include "sapi.h"


/*=====[C++ comienzo]========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Macros de definicion de constantes publicas]=========================*/


/*=====[Macros estilo funcion publicas]======================================*/


/*=====[Definiciones de tipos de datos publicos]=============================*/

// Tipo de datos que renombra un tipo basico


// Tipo de datos de puntero a funcion


// Tipo de datos enumerado
typedef enum {
  abierto ,                //otralongitudonda
  cerrado                  // longitudondacero,
} swichtgpio_estado_swicht;

// Tipo de datos estructua, union o campo de bits

typedef struct {
	gpioMap_t normalabierto;
	swichtgpio_estado_swicht estadoswicht;
}swichtgpio_t;

swichtgpio_t swichtgpio;


/*=====[Prototipos de funciones publicas]====================================*/

void swichtgpio_init( swichtgpio_t *swicht,gpioMap_t normalabierto);
                    //                         GPIO6
swichtgpio_estado_swicht swichtgpioEstado(swichtgpio_t *swicht);





/*=====[Prototipos de funciones publicas de interrupcion]====================*/


/*=====[C++ fin]=============================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Evitar inclusion multiple fin]=======================================*/

#endif /* _MODULE_NAME_H_ */
