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
#include <math.h>
/*=====[Inclusiones de dependencias de funciones privadas]===================*/

/*=====[Macros de definicion de constantes privadas]=========================*/
#define TIEMPO_ESPERA    2000
#define VOLTREF          3.3
#define RESOLUCION       1024
#define ABSORMAX         2
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
static void posicionoCeroMotor(uint32_t valorLOmaximo,uint16_t variableaux);
static void envioDatosSeriales(char *longonda,char *valormedido);

/*=====[Implementaciones de funciones publicas]==============================*/

// ------------Implementacion de Tareas----------------
//######################################################################################################################################################
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
		vTaskDelay(10/portTICK_RATE_MS);
	}
}

//######################################################################################################################################################
void tarea_general( void* taskParmPtr ){
	tTecla* config = (tTecla*) taskParmPtr;

	stepperMotorL297Init(&steppermotor,48,GPIO4,GPIO1,GPIO2,GPIO5);
	fsmtareaestadosInit();

	while(TRUE) {
		fsmtareaestadosUpdate();



		vTaskDelay(1/portTICK_RATE_MS);
	}
}

//######################################################################################################################################################
//Tarea va ser para el ensayo de longitud de onda determinada
void tarea_LOdeterminada( void* taskParmPtr ){
	tTecla* config = (tTecla*) taskParmPtr;
	static volatile uint16_t muestra[1000];
	static volatile float valorAnalogico = 0;   //Esto seria directamente la absorbancia
	static volatile float potencia = 0;
	static volatile double transmitancia=0;
	static char Buff[10];
	static char BuffTrans[10];
	static char Londa[10];
	uint32_t i,j,aux=0, total=0;
	float promedio;

	stepperMotorL297SetVelocidad(&steppermotor,velocidad_alta);
	adcConfig( ADC_ENABLE ); /* Inicializo ADC */

	while(TRUE) {
		if(xQueueReceive(valorLOselec_queue, &aux, portMAX_DELAY)){
			stepperMotorL297MoveXNanometers(&steppermotor,aux);
			//Una vez que el motor se posiciona, tomo la lectura del valor analogico

			for (i=0;i<1000;i++){
				muestra[i]=adcRead( CH1 );
			}

			for(j=0;j<1000;j++){

				total=muestra[j]+total;
			}

			promedio=total/1000;



			//Leo valor de la muestra tomado,luego de posicionar el motor
			valorAnalogico=(VOLTREF/RESOLUCION)*promedio;   //Valor analogico leido
			floatToString( valorAnalogico, Buff, 3 );      //Convierto valor analogico en float y lo guardo en un buffer
		                                                   //Esto es la absorbancia
			potencia=ABSORMAX-valorAnalogico;
			transmitancia=pow(10,potencia);
			floatToString( transmitancia, BuffTrans, 1 );
			itoa( aux,Londa,10 );
			envioDatosSeriales(Londa,Buff);
			xQueueSend(valorAnLeido,&Buff,portMAX_DELAY);
			xQueueSend(Transmitancia,&BuffTrans,portMAX_DELAY);
			promedio=0;
			total=0;

		}
		vTaskDelay(40/portTICK_RATE_MS);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------
//######################################################################################################################################################
//Tarea para el ensayo de barrido longitud de onda
void tarea_barridoLO( void* taskParmPtr ){
	tTecla* config = (tTecla*) taskParmPtr;
	static volatile uint16_t lectura = 0;
	static volatile uint16_t cantidadmuestras = 0;
	static volatile uint16_t mediciones[VALOR_MAXIMO_LO_PRUEBA]={0}; //Ojo aqui al poner el valor porque tilda el programa

	static char Buff[10];
	uint32_t aux=0;           //Valor que va ha recibir el valor de longitud maxima

	while(TRUE) {

		if(xQueueReceive(valormaximoLO_queue, &aux, portMAX_DELAY)){

			while(aux>cantidadmuestras){
				stepperMotorL297Move1nanometerCW(&steppermotor);
				lectura = adcRead( CH1 ); //Leo valor de la muestra tomado
				itoa(lectura,Buff,10 ); /* 10 significa decimal */
				//Tomo las muestras para cada longitud de onda
				mediciones[cantidadmuestras]=lectura;
				cantidadmuestras++;
				//Envio los valores medidos en formato string al puerto serie
				uartWriteString( UART_USB,Buff );
				uartWriteString( UART_USB, "\n" );

				vTaskDelay(1/portTICK_RATE_MS);
			}
			cantidadmuestras=0;

			vTaskDelay(TIEMPO_ESPERA/portTICK_RATE_MS);    //Espera un momento
			//Vuelvo a cero el motor
			posicionoCeroMotor(aux,cantidadmuestras);
			//semaforo binario que me avisa el fin del ensayo barrido
			xSemaphoreGive( sem_final_barrido );
		}
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}


/*=====[Implementaciones de funciones de interrupcion publicas]==============*/


/*=====[Implementaciones de funciones privadas]==============================*/

static void posicionoCeroMotor(uint32_t valorLOmaximo,uint16_t variableaux){
	while(valorLOmaximo>variableaux){
		stepperMotorL297Move1nanometerCCW(&steppermotor);
		variableaux++;
	}
	variableaux=0;
}

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

static void envioDatosSeriales(char *longonda,char *valormedido){
	// envio por puerto serial el valor longitud de onda posicionado
	uartWriteString( UART_USB,longonda );
	//uartWriteString( UART_USB, "," );
	uartWriteString( UART_USB, "\n" );

	// envio por puerto serial el valor leido del conversor ADC
	uartWriteString( UART_USB,valormedido );
	//uartWriteString( UART_USB, "," );
	uartWriteString( UART_USB, "\n" );
}

