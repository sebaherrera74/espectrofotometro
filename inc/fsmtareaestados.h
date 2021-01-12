/*=====[Maquina de estados de Tarea General]=====================================================
 * Copyright 2021 Sebastian Herrera <sebaherrera152@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.1
 * Fecha de creacion: 2021/01/11
 *
 *
 */

/*=====[Evitar inclusion multiple comienzo]==================================*/

#ifndef _FSMTAREAESTADOS_H_
#define _FSMTAREAESTADOS_H_

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


// Tipo de datos estructua, union o campo de bits

/*=====[Prototipos de funciones publicas]====================================*/
void fsmtareaestadosError( void );
void fsmtareaestadosInit( void );
void fsmtareaestadosUpdate( void );


/*=====[Prototipos de funciones publicas de interrupcion]====================*/

void UART0_IRQHandler(void);

/*=====[C++ fin]=============================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Evitar inclusion multiple fin]=======================================*/

#endif /* _MODULE_NAME_H_ */
