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
#include"funcionespantalla.h"


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


// Tipo de datos estructua, union o campo de bits

/*=====[Definiciones de Variables globales publicas externas]================*/



/*=====[Definiciones de Variables globales publicas]=========================*/



/*=====[Definiciones de Variables globales privadas]=========================*/
fsmtareaestados_t fsmState;


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

		break;



	case ESTADO_MENU_ENSAYOS:
		tipoensayos();

		break;

	case ESTADO_ELOD:

		/* UPDATE OUTPUTS */
		// Code for update outputs...

		/* CHECK TRANSITION CONDITIONS */
		// Code for check transitions (some ifs)...

		break;



	case ESTADO_EBLO:

		/* UPDATE OUTPUTS */
		// Code for update outputs...

		/* CHECK TRANSITION CONDITIONS */
		// Code for check transitions (some ifs)...

		break;

	case ESTADO_FINAL:

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




