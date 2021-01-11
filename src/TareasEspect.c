/*=====[Nombre del modulo]=====================================================
 * Copyright 2021 Sebastian Herrera <sebaherrera152@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.1
 * Fecha de creacion: 2021/01/11
 */

/*=====[Inclusion de su propia cabecera]=====================================*/
#include "TareasEspect.h"
#include "sapi.h"
#include "debouncetecla.h"
#include "task.h"
/*=====[Inclusiones de dependencias de funciones privadas]===================*/




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



/*=====[Implementaciones de funciones publicas]==============================*/

// ------------Implementacion de Tareas----------------

// Implementacion de la tarea polling teclas
void tarea_teclas( void* taskParmPtr ){
	/*taskENTER_CRITICAL();
	printf("\r\n %s \r\n",pcTaskGetTaskName(NULL));
	fflush( stdout );
	taskEXIT_CRITICAL();*/

	tTecla* config = (tTecla*) taskParmPtr;

	//inicializacion teclas
	int i = 0;
	for (i = 0; i < 4; i++)
		mefbotonInit(&config[i]);

	// Tarea periodica cada 10 ms
	portTickType xPeriodicity =  40/ portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE) {
		for (i = 0; i < 4; i++)
			actualizacionTecla(&config[i]);  //update de tareas teclas 1 ,2
		vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
	}
}

/*=====[Implementaciones de funciones de interrupcion publicas]==============*/

void UART0_IRQHandler(void)
{
	// ...
}

/*=====[Implementaciones de funciones privadas]==============================*/

static void funPrivada(void)
{
	// ...
}

