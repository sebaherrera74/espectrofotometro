/*=====[Maquina de estados de Tarea General]=====================================================
 * Copyright 2021 Sebastian Herrera <sebaherrera152@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.1
 * Fecha de creacion: 2021/01/11
 */

/*=====[Inclusion de su propia cabecera]=====================================*/

#include "sapi.h"

/*=====[Inclusiones de dependencias de funciones privadas]===================*/

#include "fsmtareaestados.h"
#include "funcionespantalla.h"
#include "debouncetecla.h"
#include "FreeRTOS.h"
#include "sem_queues_espect.h"
#include "swichtgpio.h"
#include "steppermotor_l297.h"

/*=====[Macros de definicion de constantes privadas]=========================*/

#define TIEMPO_PULSACION 500
#define CAMBIOESCALA     100

/*=====[Macros estilo funcion privadas]======================================*/

/*=====[Definiciones de tipos de datos privados]=============================*/
// Tipo de datos que renombra un tipo basico

// Tipo de datos de puntero a funcion

// Tipo de datos enumerado
// Tipos de estados del display
typedef enum{
	ESTADO_INICIAL,
	ESTADO_MENU_ENSAYOS,
	ESTADO_ELOD   ,     //Estado Ensayo de longitud de onda determinada (ELOD)
	ESTADO_EBLO   ,     //Estado Ensayo de barrido de longitud de onda  (EBLO)
	ESTADO_FINAL
} fsmtareaestados_t;

//Ensayos posibles
typedef enum{
	ENSAYOS,
	ENSAYO_ELOD   ,
	ENSAYO_EBLO   ,
} menuensayos_t;

//Estadosposibles en el caso del ensayo de longitud de onda determinada
typedef enum{
	ENSAYO_ELOD_INICIAL,
	ENSAYO_ELOD_CONFIRMACION   ,
	ENSAYO_ELOD_PROCESO   ,
	ENSAYO_ELOD_MUESTRAVALOR
} menuensayoselod_t;

//Estadosposibles en el caso del ensayo de barrido longitud de onda
typedef enum{
	ENSAYO_EBLO_INICIAL,
	ENSAYO_EBLO_CONFIRMACION   ,
	ENSAYO_EBLO_PROCESO   ,
	ENSAYO_EBLO_FINAL
} menuensayoseblo_t;

// Tipo de datos estructua, union o campo de bits

/*=====[Definiciones de Variables globales publicas externas]================*/

/*=====[Definiciones de Variables globales publicas]=========================*/

static uint16_t longitudonda=0;
char longonda[7];
char valorAnleido[10];

char Transmit[10];


/*=====[Definiciones de Variables globales privadas]=========================*/
fsmtareaestados_t fsmState;
menuensayos_t   tipoensayo;
menuensayoselod_t ensayoselod;
menuensayoseblo_t ensayoeblo;

/*=====[Prototipos de funciones privadas]====================================*/

/*=====[Implementaciones de funciones publicas]==============================*/

// FSM Error Handler Function
void fsmtareaestadosError( void )
{
	// Error handler, example, restart FSM:
	fsmState = ESTADO_INICIAL;   // Set initial state
}

// FSM Initialize Function
void fsmtareaestadosInit( void )
{
	InitLcd();	                //inicilizo el display Ili9341
	RotarPantalla();            //Roto pantalla a horizontal
	barraColores();
	swichtgpio_init(&swichtgpio,GPIO8); //inicializo el swicht conectado el na a GPIO08
	stepperMotorL297ResetPosiciones(&steppermotor);
	fsmState = ESTADO_INICIAL;   // Set initial state
}

// FSM Update Sate Function
void fsmtareaestadosUpdate( void ){

	uint32_t valor_maximo=VALOR_MAXIMO_LO_PRUEBA;//pongo un valor chico para probar
	//uint32_t valor_minimo= VALOR_MIN_LO ;  //pongo un valor chico para probar
	static volatile uint16_t valor = 0;
	static volatile float valorAn = 0;
	static volatile float arsorb=0;            //absorbancia
	static volatile float transmit=0;   //transmitancia


		static char Buff[10];
	switch( fsmState ){

	case ESTADO_INICIAL:
		posicioncero();
		while(!swichtgpioEstado(&swichtgpio)){		//veo la posicion del swicht sino esta en cero giro el motor
			stepperMotorL297Move1nanometerCCW(&steppermotor);
		}
		cambiofondo(ILI9341_LIGHTCORAL);
		fsmState=ESTADO_MENU_ENSAYOS;
		tipoensayo=ENSAYOS;

		break;

	case ESTADO_MENU_ENSAYOS:
		if(xSemaphoreTake(tecla_config[0].sem_tec_pulsada ,0)){
			tipoensayo=ENSAYO_ELOD;
		}
		if (xSemaphoreTake(tecla_config[1].sem_tec_pulsada ,0)){
			tipoensayo=ENSAYO_EBLO;
		}
		valor = adcRead( CH1 );
        valorAn=(3.3/1024)*valor;

        floatToString( valorAn, Buff, 3 );
		//itoa( valor,Buff,10 ); /* 10 significa decimal */


        muestravaloranalogico(Buff);




		switch( tipoensayo ){

		case ENSAYOS:
			tipoensayos();
		break;
		case ENSAYO_ELOD:
			ensayolongitudonda();
			if(xSemaphoreTake(tecla_config[2].sem_tec_pulsada ,0)){
				fsmState=ESTADO_ELOD;
				ensayoselod=ENSAYO_ELOD_INICIAL;
				cambiofondo(ILI9341_LIGHTCORAL);
			}
		break;
		case ENSAYO_EBLO:
			ensayobarrido();
			if(xSemaphoreTake(tecla_config[2].sem_tec_pulsada ,0)){
				fsmState=ESTADO_EBLO;
				ensayoeblo=ENSAYO_EBLO_INICIAL;
			}
		break;
		default:
			tipoensayos();
		break;
		}
		break;
		case ESTADO_ELOD:
			switch( ensayoselod ){
			case ENSAYO_ELOD_INICIAL:
				if(xSemaphoreTake(tecla_config[0].sem_tec_pulsada ,0)){
					if(tecla_config[0].tiempo_diff > TIEMPO_PULSACION/portTICK_RATE_MS){              //Si presiono mas de 500 mseg muevo la longitud d eonda de a 100
						longitudonda=longitudonda+CAMBIOESCALA;
					}
					longitudonda++;
				}
				if (xSemaphoreTake(tecla_config[1].sem_tec_pulsada ,0)){
					if(tecla_config[1].tiempo_diff > TIEMPO_PULSACION/portTICK_RATE_MS){              //Si presiono mas de 500 mseg muevo la longitud d eonda de a 100
						longitudonda=longitudonda-CAMBIOESCALA;
					}
					longitudonda--;
				}
				if (longitudonda<=VALOR_MIN_LO){
					longitudonda=VALOR_MIN_LO;
				}
				if (longitudonda>VALOR_MAX_LO){
					longitudonda=VALOR_MIN_LO;
					cambiofondo(ILI9341_LIGHTCORAL);
				}
				memset (longonda,'\0',7);
				sprintf(longonda, "%d", longitudonda); // guardo el valor de longitud de onda seleccionado en un buffer
				seleccionlongonda(longonda);
				if(xSemaphoreTake(tecla_config[2].sem_tec_pulsada ,0)){
					ensayoselod =ENSAYO_ELOD_CONFIRMACION;
					cambiofondo(ILI9341_LIGHTCORAL);
				}
				break;
			case ENSAYO_ELOD_CONFIRMACION:
				confirmacionensayo();
				if(xSemaphoreTake(tecla_config[2].sem_tec_pulsada ,0)){
					ensayoselod =ENSAYO_ELOD_PROCESO;
					cambiofondo(ILI9341_LIGHTCORAL);
				}
				if (xSemaphoreTake(tecla_config[3].sem_tec_pulsada ,0)){
					ensayoselod =ENSAYO_ELOD_INICIAL;
					cambiofondo(ILI9341_LIGHTCORAL);
				}
				break;
			case ENSAYO_ELOD_PROCESO:
				ensayoenproceso();
				xQueueSend(valorLOselec_queue, &longitudonda, portMAX_DELAY);
				ensayoselod =ENSAYO_ELOD_MUESTRAVALOR;
				xQueueReset (valorLOselec_queue);
				cambiofondo(ILI9341_LIGHTCORAL);
				break;
			case ENSAYO_ELOD_MUESTRAVALOR:
				//muestro el valor de la medicion realizada, en Volts
				//Recibo de la cola el valor analogico leido
				xQueueReceive(valorAnLeido, &valorAnleido, 1);
				xQueueReceive(Transmitancia, &Transmit, 1);
				//Muestro valor de longitud de onda posicionado

				valorlongondaselecc(longonda,valorAnleido,Transmit);


				break;
			default:
				break;
			}

			if(xSemaphoreTake(tecla_config[3].sem_tec_pulsada ,0)){  //si presiono tecla 4 return vuelvo al inicio
				cambiofondo(ILI9341_LIGHTCORAL);
				fsmState=ESTADO_FINAL;
			}
			break;

			case ESTADO_EBLO:
				/*tengo que chequear la posicion del motor, porque puedo haber estado haciendo
				 * otro ensayo y el motor no quedo en la posicion cero
				 *Una vez que el motor llegue a cero comenzar el ensayo desde cero a 1050 nanometros
				 */
				switch( ensayoeblo ){
				case ENSAYO_EBLO_INICIAL:
					/*AQUI CHEQUEO POSICIONCERO SI PASA VOY A CONFIRMACION
					 * porque puede haber estado haciendo otro ensayo y no me quedo en cero
					 * 							 */
					if(longitudonda|=0 ){
						cambiofondo(ILI9341_LIGHTCORAL);
						posicioncero();
						while((longitudonda|=0)){
							stepperMotorL297Move1nanometerCCW(&steppermotor);
							longitudonda--;
						}
						//tendria que avisar que la posicion deseada y/o actual queda en cero
						//REvisar esto
						stepperMotorL297ResetPosiciones(&steppermotor);

					}
					ensayoeblo=ENSAYO_EBLO_CONFIRMACION;
					cambiofondo(ILI9341_LIGHTCORAL);
					break;
				case ENSAYO_EBLO_CONFIRMACION:

					//poner mensajes en diaplay de confirmacion o salida
					longitudonda=0;
					confirmacionensayoEblo();
					if(xSemaphoreTake(tecla_config[2].sem_tec_pulsada ,0)){
						ensayoeblo =ENSAYO_EBLO_PROCESO;
						cambiofondo(ILI9341_LIGHTCORAL);
					}
					if (xSemaphoreTake(tecla_config[3].sem_tec_pulsada ,0)){
						ensayoeblo =ENSAYO_EBLO_FINAL;
						cambiofondo(ILI9341_LIGHTCORAL);
					}
					break;

				case ENSAYO_EBLO_PROCESO:
					ensayoenproceso_2();
					xQueueSend(valormaximoLO_queue, &valor_maximo, portMAX_DELAY);  //Aqui mando el barrido, con el valor maximo

					if (xSemaphoreTake(sem_final_barrido,portMAX_DELAY)){
						ensayoeblo =ENSAYO_EBLO_INICIAL;
						longitudonda=0;
					}
					//Podria poner algun mensaje para avisar que el motor
					//termino el barrido y esta volviendo a cero
					cambiofondo(ILI9341_LIGHTCORAL);
					break;
				case ENSAYO_EBLO_FINAL:
					fsmState=ESTADO_MENU_ENSAYOS;
					tipoensayo=ENSAYOS;
					break;
				default:
					break;
				}
				break;

				case ESTADO_FINAL:
					fsmState=ESTADO_MENU_ENSAYOS;
					tipoensayo=ENSAYOS;

					break;
				default:
					fsmtareaestadosError();
					break;
	}
}




/*=====[Implementaciones de funciones de interrupcion publicas]==============*/


/*=====[Implementaciones de funciones privadas]==============================*/




