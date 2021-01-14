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

/*=====[Inclusion de su propia cabecera]=====================================*/

#include "sapi.h"

/*=====[Inclusiones de dependencias de funciones privadas]===================*/

#include "fsmtareaestados.h"
#include "funcionespantalla.h"
#include "debouncetecla.h"
#include "FreeRTOS.h"
#include "sem_queues_espect.h"


/*=====[Macros de definicion de constantes privadas]=========================*/

/*=====[Macros estilo funcion privadas]======================================*/



/*=====[Definiciones de tipos de datos privados]=============================*/

// Tipo de datos que renombra un tipo basico


// Tipo de datos de puntero a funcion


// Tipo de datos enumerado
// FSM state names
typedef enum{
	ESTADO_INICIAL,
	ESTADO_MENU_ENSAYOS,
	ESTADO_ELOD   ,     //Estado Ensayo de longitud de onda determinada (ELOD)
	ESTADO_EBLO   ,     //Estado Ensayo de barrido de longitud de onda  (EBLO)
	ESTADO_FINAL
} fsmtareaestados_t;

typedef enum{
	ENSAYOS,
	ENSAYO_ELOD   ,
	ENSAYO_EBLO   ,
} menuensayos_t;

typedef enum{
	ENSAYO_ELOD_INICIAL,
	ENSAYO_ELOD_CONFIRMACION   ,
	ENSAYO_ELOD_PROCESO   ,
} menuensayoselod_t;




// Tipo de datos estructua, union o campo de bits

/*=====[Definiciones de Variables globales publicas externas]================*/



/*=====[Definiciones de Variables globales publicas]=========================*/

static volatile uint16_t longitudonda=0;
char texto[7];

/*=====[Definiciones de Variables globales privadas]=========================*/
fsmtareaestados_t fsmState;
menuensayos_t   tipoensayo;
menuensayoselod_t ensayoselod;
/*=====[Prototipos de funciones privadas]====================================*/



/*=====[Implementaciones de funciones publicas]==============================*/



// FSM Error Handler Function
void fsmtareaestadosError( void )
{
	// Error handler, example, restart FSM:
	// fsmState = STATE_INIT;
}

// FSM Initialize Function
void fsmtareaestadosInit( void )
{

	InitLcd();	                //inicilizo el display Ili9341
	RotarPantalla();            //Roto pantalla a horizontal
	barraColores();

	fsmState = ESTADO_INICIAL;   // Set initial state
}

// FSM Update Sate Function
void fsmtareaestadosUpdate( void ){


	switch( fsmState ){

	case ESTADO_INICIAL:
		posicioncero();
		//Aqui verificaria si el motor esta en posicion cero lo voy a simular como si esta
		//para continuar
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
			}
			break;
		default:
			tipoensayos();
			break;
		}

		if(xSemaphoreTake(tecla_config[3].sem_tec_pulsada ,0)){  //si presiono tecla 4 return vuelvo al inicio
			fsmState=ESTADO_MENU_ENSAYOS;
			//tipoensayo=ENSAYOS;
		}

		break;

		case ESTADO_ELOD:



			switch( ensayoselod ){
			case ENSAYO_ELOD_INICIAL:
				if(xSemaphoreTake(tecla_config[0].sem_tec_pulsada ,0)){
					if(tecla_config[0].tiempo_diff > 500){              //Si presiono mas de 500 mseg muevo la longitud d eonda de a 100
						longitudonda=longitudonda+100;
					}
					longitudonda++;
				}
				if (xSemaphoreTake(tecla_config[1].sem_tec_pulsada ,0)){
					if(tecla_config[0].tiempo_diff > 500){              //Si presiono mas de 500 mseg muevo la longitud d eonda de a 100
						longitudonda=longitudonda-100;
					}
					longitudonda--;
				}

				if (longitudonda<=VALOR_MIN_LO){
					longitudonda=VALOR_MIN_LO;
				}
				if (longitudonda>VALOR_MAX_LO){
					longitudonda=VALOR_MIN_LO;
					memset (texto,'\0',7);
					cambiofondo(ILI9341_LIGHTCORAL);

				}

				sprintf(texto, "%d", longitudonda); // guardo el valor de longitud de onda seleccionado en un buffer

				seleccionlongonda(texto);
				if(xSemaphoreTake(tecla_config[2].sem_tec_pulsada ,0)){
					ensayoselod =ENSAYO_ELOD_CONFIRMACION;
					cambiofondo(ILI9341_LIGHTCORAL);
				}



				break;
			case ENSAYO_ELOD_CONFIRMACION:
				confirmacionensayo();
				if(xSemaphoreTake(tecla_config[2].sem_tec_pulsada ,0)){
					ensayoselod =ENSAYO_ELOD_PROCESO;
					}


				if (xSemaphoreTake(tecla_config[3].sem_tec_pulsada ,0)){
					ensayoselod =ENSAYO_ELOD_INICIAL;
					cambiofondo(ILI9341_LIGHTCORAL);
				}
				break;
			case ENSAYO_ELOD_PROCESO:

				xQueueSend(valorLOselec_queue, &longitudonda, portMAX_DELAY);

				//Aqui tendria que ver la forma de implementar la muestra del valor de longitud
				//de onda seleccionado y el valor medido del conversor analogico}
				//tambien el envio del valor por el puerto serie
				//Por ahora solo lo envio al estado inicial
				ensayoselod =ENSAYO_ELOD_INICIAL;
				cambiofondo(ILI9341_LIGHTCORAL);
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
				gpioToggle(LED1);
				if(xSemaphoreTake(tecla_config[3].sem_tec_pulsada ,0)){  //si presiono tecla 4 return vuelvo al inicio

					fsmState=ESTADO_FINAL;
					//tipoensayo=ENSAYOS;

				}

				break;

			case ESTADO_FINAL:
				fsmState=ESTADO_MENU_ENSAYOS;
				tipoensayo=ENSAYOS;



				/* UPDATE OUTPUTS */
				// Code for update outputs...

				/* CHECK TRANSITION CONDITIONS */
				// Code for check transitions (some ifs)...

				break;



			default:
				fsmtareaestadosError();
				break;
	}
}




/*=====[Implementaciones de funciones de interrupcion publicas]==============*/


/*=====[Implementaciones de funciones privadas]==============================*/




