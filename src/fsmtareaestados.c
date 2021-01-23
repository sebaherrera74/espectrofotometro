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
#include "swichtIrq.h"
#include "steppermotor_l297.h"


/*=====[Macros de definicion de constantes privadas]=========================*/

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
char texto[7];

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
	// fsmState = STATE_INIT;
}

// FSM Initialize Function
void fsmtareaestadosInit( void )
{

	InitLcd();	                //inicilizo el display Ili9341
	RotarPantalla();            //Roto pantalla a horizontal
	barraColores();
	swichtIrq_init(&swichtIrq,GPIO6); //inicializo el swicht conectado el na a GPIO06

	fsmState = ESTADO_INICIAL;   // Set initial state
}

// FSM Update Sate Function
void fsmtareaestadosUpdate( void ){

	uint32_t valor_maximo=100;//pongo un valor chico para probar
	uint32_t valor_minimo=0;//pongo un valor chico para probar

	switch( fsmState ){

	case ESTADO_INICIAL:
		posicioncero();

		while(!swichtIrqEstado(&swichtIrq)){			//veo la posicion del swicht sino esta en cero giro el motor
        /* que conviene aqui llamar directamente a la funcion de girra el motor
         * de la libreria de sttepermotor.h o un semaforo que vaya a la tarea?
         */

			// xSemaphoreGive(sem_posicioncero);No conviene ponerlo en la tarea
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
				ensayoselod =ENSAYO_ELOD_MUESTRAVALOR;
				cambiofondo(ILI9341_LIGHTCORAL);
				break;

			case ENSAYO_ELOD_MUESTRAVALOR:
				valorlongondaselecc(texto);
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
							//aQUI CHEQUEO POSICIONCERO SI PASA VOY A CONFIRMACION
							if(longitudonda|=0){
								cambiofondo(ILI9341_LIGHTCORAL);
								posicioncero();
								xQueueSend(valorLOselec_queue, &valor_minimo, 20);
								longitudonda=valor_minimo;

							}
							ensayoeblo=ENSAYO_EBLO_CONFIRMACION;
							cambiofondo(ILI9341_LIGHTCORAL);
							break;
						case ENSAYO_EBLO_CONFIRMACION:

							//poner mensajes en diaplay de confirmacion o salida

							confirmacionensayoEblo();
							if(xSemaphoreTake(tecla_config[2].sem_tec_pulsada ,0)){
								ensayoeblo =ENSAYO_EBLO_PROCESO;
							}
							if (xSemaphoreTake(tecla_config[3].sem_tec_pulsada ,0)){
								ensayoeblo =ENSAYO_EBLO_FINAL;
								cambiofondo(ILI9341_LIGHTCORAL);
							}
							break;

						case ENSAYO_EBLO_PROCESO:
							//disparo la tarea de motor

							xQueueSend(valorLOselec_queue, &valor_maximo, 1);  //Aqui mando el barrido, con el valor maximo
							longitudonda=valor_maximo; //Esto asigno porque aqui quedaria la longitud de onda

							ensayoeblo =ENSAYO_EBLO_FINAL;

							xQueueSend(valorLOselec_queue, &valor_minimo, 20); //vuelvo a posicion cero el motor
							longitudonda=valor_minimo; //Esto asigno porque aqui quedaria la longitud de onda
							cambiofondo(ILI9341_LIGHTCORAL);

							break;

						case ENSAYO_EBLO_FINAL:
							//vuelvo a colocar el motor en posicion cero
							//cambio variable global a cero
							//
							fsmState=ESTADO_MENU_ENSAYOS;
							tipoensayo=ENSAYOS;
							break;

						default:

							break;
						}
                     break;




			/*while(!swichtIrqEstado(&swichtIrq)){			//veo la posicion del swicht sino esta en cero giro el motor
			        /* que conviene aqui llamar directamente a la funcion de girra el motor
			         * de la libreria de sttepermotor.h o un semaforo que vaya a la tarea?
			         */

						// xSemaphoreGive(sem_posicioncero);No conviene ponerlo en la tarea
					//	 stepperMotorL297Move1nanometerCCW(&steppermotor);








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




