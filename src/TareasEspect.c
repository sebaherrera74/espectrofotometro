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
#include "fsmtareaestados.h"
#include "steppermotor_l297.h"
#include "sem_queues_espect.h"
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
static char* itoa(int value, char* result, int base);


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
	for (i = 0; i < 4; i++){
		mefbotonInit(&config[i]);
	}


	portTickType xPeriodicity =  10/ portTICK_RATE_MS;
	portTickType xLastWakeTime = xTaskGetTickCount();

	// ---------- REPETIR POR SIEMPRE --------------------------
	while(TRUE) {
		for (i = 0; i < 4; i++){
			actualizacionTecla(&config[i]);  //update de tareas teclas 1 ,2
		}

		vTaskDelay(1/portTICK_RATE_MS);
		//vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
	}
}



void tarea_general( void* taskParmPtr ){
	tTecla* config = (tTecla*) taskParmPtr;

	fsmtareaestadosInit();

	while(TRUE) {
		fsmtareaestadosUpdate();
		vTaskDelay(40/portTICK_RATE_MS);
		}
}

void tarea_motorstepper( void* taskParmPtr ){
	tTecla* config = (tTecla*) taskParmPtr;

	uint32_t aux=0;

	stepperMotorL297Init(&steppermotor,48,GPIO4,GPIO7,GPIO8,GPIO5);
	stepperMotorL297SetVelocidad(&steppermotor,velocidad_media);



	while(TRUE) {


		if(xQueueReceive(valorLOselec_queue, &aux, portMAX_DELAY)){
			stepperMotorL297MoveXNanometers(&steppermotor,aux);
			//semaforo que avisa que el motor ya se posiciono en la longitud
			//de onda
			xSemaphoreGive( sem_motorposicionadoLOD );


		}


		vTaskDelay(40/portTICK_RATE_MS);
		}
}

void tarea_lecturaADC( void* taskParmPtr ){
	tTecla* config = (tTecla*) taskParmPtr;


	static volatile char Buff[10];
	static volatile uint16_t muestra = 0;
	uint32_t aux=0;
    adcConfig( ADC_ENABLE ); /* Inicializo ADC */



	while(TRUE) {

		if(xSemaphoreTake( sem_motorposicionadoLOD,portMAX_DELAY)){
		muestra = adcRead( CH1 ); //Leo valor de la muestra tomado

		itoa( muestra,Buff,10 ); /* 10 significa decimal */

		//mando por una cola el valor leido del conversor ADC
		xQueueSend(valorAnLeido,&Buff,portMAX_DELAY);


		}


		vTaskDelay(40/portTICK_RATE_MS);
	}
}


/*=====[Implementaciones de funciones de interrupcion publicas]==============*/


/*=====[Implementaciones de funciones privadas]==============================*/
static char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}

