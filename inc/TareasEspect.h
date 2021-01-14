/*=====[Nombre del modulo]=====================================================
 * Copyright 2021 Sebastian Herrera <sebaherrera152@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.1
 * Fecha de creacion: 2021/01/11
 */

/*=====[Evitar inclusion multiple comienzo]==================================*/

#ifndef _TAREASESPECT_H_
#define _TAREASESPECT_H_

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


/*=====[Prototipos de funciones publicas]====================================*/

void tarea_teclas( void* taskParmPtr);
void tarea_general( void* taskParmPtr);
void tarea_motorstepper( void* taskParmPtr);


//bool_t rtcInit( rtc_t* rtc );

/*=====[Prototipos de funciones publicas de interrupcion]====================*/



/*=====[C++ fin]=============================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Evitar inclusion multiple fin]=======================================*/

#endif /* _MODULE_NAME_H_ */
