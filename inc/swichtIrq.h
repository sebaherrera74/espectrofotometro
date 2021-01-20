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

#ifndef _SWICHTIRQ_H_
#define _SWICHTIRQ_H_

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
  otralongitudonda,
  longitudondacero,
} swichtIrq_estadoswicht;

// Tipo de datos estructua, union o campo de bits

typedef struct {
	gpioMap_t normalabierto;
	swichtIrq_estadoswicht estadoswicht;
}swichtIrq_t;

swichtIrq_t swichtIrq;


/*=====[Prototipos de funciones publicas]====================================*/

void swichtIrq_init( swichtIrq_t *swicht,gpioMap_t normalabierto);
                    //                         GPIO6
uint8_t swichtIrqEstado(swichtIrq_t *swicht);




/*=====[Prototipos de funciones publicas de interrupcion]====================*/


/*=====[C++ fin]=============================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Evitar inclusion multiple fin]=======================================*/

#endif /* _MODULE_NAME_H_ */
