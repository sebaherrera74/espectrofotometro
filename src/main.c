/*=====[Nombre del programa]===================================================
 * Copyright 2021 Sebastian Herrera <sebaherrera@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.1
 * Fecha de creacion: 2021/01/10
 * Firmware para Espectrofotometro marca shimadzu, doble haz
 */

/*=====[Inclusiones de dependencias de funciones]============================*/

#include "sapi.h"
#include "TareasEspect.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "debouncetecla.h"
#include "sem_queues_espect.h"

DEBUG_PRINT_ENABLE;

/*=====[Macros de definición de constantes privadas]=========================*/


/*=====[Definiciones de variables globales externas]=========================*/


/*=====[Definiciones de variables globales publicas]=========================*/


/*=====[Definiciones de variables globales privadas]=========================*/
void teclas_config(void);


/*=====[Funcion principal, punto de entrada al programa luegp de encender]===*/

int main (void){
	int Error_creacion_Colas=0; //Variable para verificar la crecion de colas y semaforos
	// ----- Configuraciones -------------------------
	boardInit();
	i2cInit( I2C0, 100000 );
	uartConfig( UART_USB, 115200 );
	//uartWriteString( UART_USB,"Driver de Espectrofotometro \r\n" );
	teclas_config();
	//Aqui tendria que ir el chequeo de la posicion cero del motor
	/* funcion que crea semaforos y colas a utilizar */
	Error_creacion_Colas=sem_queues_init();  //Creacion de colas y semaforos

	//CREACION DE TAREAS EN  freeRTOS

	//-------------Tarea polling de teclas --------------

	BaseType_t res1 =xTaskCreate(tarea_teclas,   // Funcion de la tarea a ejecutar
			(const char *)"Teclas polling",      // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2,          // Cantidad de stack de la tarea
			tecla_config,                        // Parametros de tarea
			tskIDLE_PRIORITY+1,                  // Prioridad de la tarea
			0);                                  // Puntero a la tarea creada en el sistema

	BaseType_t res2 =xTaskCreate(tarea_general,  // Funcion de la tarea a ejecutar
			(const char *)"Tarea gral",      // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2,      // Cantidad de stack de la tarea
			tecla_config,                    // Parametros de tarea
			tskIDLE_PRIORITY+1,              // Prioridad de la tarea
			0);                              // Puntero a la tarea creada en el sistema

	BaseType_t res3 =xTaskCreate(tarea_motorstepper,     // Funcion de la tarea a ejecutar
			(const char *)"motor paso a paso ",  // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2,          // Cantidad de stack de la tarea
			0,                                   // Parametros de tarea
			tskIDLE_PRIORITY+2,                  // Prioridad de la tarea, le doy mas prioridad
			0);                                  // Puntero a la tarea creada en el sistema

	BaseType_t res4 =xTaskCreate(tarea_barridoLO,     // Funcion de la tarea a ejecutar
			(const char *)"tarea barrido LO",  // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2,          // Cantidad de stack de la tarea
			0,                                   // Parametros de tarea
			tskIDLE_PRIORITY+2,                  // Prioridad de la tarea, le doy mas prioridad
			0);                                  // Puntero a la tarea creada en el sistema

	/*Chequeo que las tareas se crearon coreectamente, si no crearon caera el progrma
	 * en un while true */

	if(res1 == pdFAIL || res2 == pdFAIL || res3 == pdFAIL || res4 == pdFAIL){
		printf( "Error al crear las tareas." );
		while(TRUE);                              //Entro en un lazo cerrado
	}


	//Si se creron bien las colas y los semaforos lanzo el scheduler
	if (Error_creacion_Colas==0){
		vTaskStartScheduler();
	}else{
		printf("Error al iniciar el sistema !!!");
		}

	// ----- Repetir por siempre ---------------------
	while(TRUE) {
	}

	// NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
	// directamenteno sobre un microcontrolador y no es llamado por ningun
	// Sistema Operativo, como en el caso de un programa para PC.
	return 0;
}

void teclas_config(void)
{
	tecla_config[0].tecla= TEC1;
	tecla_config[0].sem_tec_pulsada	= xSemaphoreCreateBinary();

	tecla_config[1].tecla= TEC2;
	tecla_config[1].sem_tec_pulsada	= xSemaphoreCreateBinary();

	tecla_config[2].tecla= TEC3;
	tecla_config[2].sem_tec_pulsada	= xSemaphoreCreateBinary();

	tecla_config[3].tecla= TEC4;
	tecla_config[3].sem_tec_pulsada = xSemaphoreCreateBinary();
}



















