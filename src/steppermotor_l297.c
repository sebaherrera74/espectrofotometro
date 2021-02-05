/*=====[Nombre del modulo]=====================================================
 * Copyright Sebastian Herrrera  <sebaherrera152@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Fecha de creacion: 2021/01/02
 */
/* Motor utilizado es el PM5555L-048, de cuya hojas de datos se saca que:
 * Numeros de pasos por rotacion es: 48 (7,5°/por paso), esto significa que da una vuelta completa
 * 360° en 48 pasos.
 * ver hoja de datos en carpeta
*/

/*====[Inclusion de su propia cabecera]=====================================*/

#include "steppermotor_l297.h"

/*=====[Inclusiones de dependencias de funciones privadas]===================*/

#include "sapi.h"
//#include <dependencia.h>

/*=====[Macros de definicion de constantes privadas]=========================*/

#define QMC5883L_REG_Y_LSB       0x02
#define MAX_AMOUNT_OF_RGB_LEDS   9

// Posibles salidas de toggle pin on match

// ENET_CRS_DV ---> P1_16, FUNC4 = T0_MAT0
// ENET_RXD0   ---> P1_15, FUNC4 = T0_MAT1
// ENET_TXD0   ---> P1_18, FUNC4 = T0_MAT3

// GPIO3       ---> P6_7,  FUNC5 = T2_MAT0
// GPIO4       ---> P6_8,  FUNC5 = T2_MAT1
// GPIO5       ---> P6_9,  FUNC5 = T2_MAT2
// GPIO7       ---> P6_11, FUNC5 = T2_MAT3

// RS232_TXD   ---> P2_3,  FUNC6 = T3_MAT0
// RS232_RXD   ---> P2_4,  FUNC6 = T3_MAT1

#define SIGNAL_PIN_SCU_PORT   6              // GPIO3 (EDU-CIAA-NXP) --> P6
#define SIGNAL_PIN_SCU_PIN    7              // GPIO3 (EDU-CIAA-NXP) --> _7
#define SIGNAL_PIN_FUNC       5              // GPIO3 (EDU-CIAA-NXP) --> FUNC5

#define SIGNAL_TIMER          LPC_TIMER2     // LPC4337 TIMER2
#define SIGNAL_TIMER_CLK      CLK_MX_TIMER2  // LPC4337 TIMER2 CLOCK
#define SIGNAL_TIMER_RESET    RGU_TIMER2_RST // LPC4337 TIMER2 RGU
#define SIGNAL_MATCH_NUMBER   0              // MATCH0

#define SIGNAL_COMPARE_VAUE    300000 //51000      // TimerClk = 204 MHz ==> 204 MHz / 51000 = 4 KHz ==> 250 us
#define SIGNAL_INITIAL_STATE  0              // Signal initial state low

//225000 es el valor para velocidad maxima TimerClk = 204 MHz ==> 204 MHz / 225000 = 906 HZ frecuencia del clk 453 HZ



/*=====[Macros estilo funcion privadas]======================================*/

#define rtcConfig rtcInit

/*=====[Definiciones de tipos de datos privados]=============================*/

// Tipo de datos que renombra un tipo basico
typedef uint32_t gpioRegister_t;

// Tipo de datos de puntero a funcion
typedef void (*FuncPtrPrivado_t)(void *);

// Tipo de datos enumerado


// Tipo de datos estructua, union o campo de bits


/*=====[Definiciones de Variables globales publicas externas]================*/

extern int32_t varGlobalExterna;

/*=====[Definiciones de Variables globales publicas]=========================*/

int32_t varGlobalPublica = 0;

/*=====[Definiciones de Variables globales privadas]=========================*/

static int32_t varGlobalPrivada = 0;

static volatile uint32_t countIrq=0;    //contador de interrupciones por timer
static volatile uint32_t countPasos=0;  //Variable Global contador de pasos
static volatile uint32_t PosicionActual=0;
static volatile uint32_t PosicionDeseada=0;
static volatile uint32_t DiferenciaPosicion=0;

/*=====[Prototipos de funciones privadas]====================================*/

//funcion p inicializar el Timer 2
static void signalInit( void );  //Inicializa el Timer
static void signalStart (void);  //Lanza timer
static void signalStop( void );   //Para el timer

void TIMER2_IRQHandler(void);
static void funPrivada(void);

/*=====[Implementaciones de funciones publicas]==============================*/


void stepperMotorL297Init(steppermotor_l297_t *steppermotor,uint32_t numerodepasosxvuelta,
						  gpioMap_t enable,gpioMap_t reset,gpioMap_t half_full,gpioMap_t cw_ccw){
                      //   GPIO4             GPIO1             GPIO2              GPIO5
                      //Cambio los gpios me coinciden con los del lcd

	if (numerodepasosxvuelta!=0){
		steppermotor->Numeros_pasosxvuelta=numerodepasosxvuelta;
	    steppermotor->angulo_resolucion=360/(steppermotor->Numeros_pasosxvuelta); //Esto me da la resolucion que en este caso es 360/48=7,5°
	    }
	else {
		printf("El numero de pasos por vuelta no puede ser cero");
		}
	//Inicializo los pines

	steppermotor->Gpioenable=enable;
	steppermotor->Gpioreset=reset;
	steppermotor-> Gpiohalf_full_step=half_full;
	steppermotor->Gpiodireccion=cw_ccw;

	// configuro el como salidas los GPIO'S

	gpioConfig(steppermotor->Gpioenable         ,GPIO_OUTPUT);
	gpioConfig(steppermotor->Gpioreset          ,GPIO_OUTPUT);
	gpioConfig(steppermotor->Gpiohalf_full_step ,GPIO_OUTPUT);
	gpioConfig(steppermotor->Gpiodireccion      ,GPIO_OUTPUT);

	signalInit(); //Inicializacion de Timer 2, por GPIO3

//Aqui tambien tendria que ir la funcion del sensor de poscion en cero, o se cuando arranque
//que verfique que el motor se encuentra en cero
}

void stepperMotorL297SetDireccionGiro(steppermotor_l297_t *steppermotor,steppermotor_l297_direccion direcciongiro){

	switch (direcciongiro){

	case sentido_cw:
		gpioWrite(steppermotor->Gpiodireccion,ON);
		steppermotor->direccion=direcciongiro;
		break;

	case sentido_ccw:
		gpioWrite(steppermotor->Gpiodireccion,OFF);
		steppermotor->direccion=direcciongiro;
		break;

	default:
		gpioWrite(steppermotor->Gpiodireccion,ON);
		steppermotor->direccion=direcciongiro;

		break;
	}
}


steppermotor_l297_direccion stepperMotorL297GetDireccionGiro(steppermotor_l297_t *steppermotor){

	return steppermotor->direccion;
}


void stepperMotorL297SetEnable(steppermotor_l297_t *steppermotor,steppermotor_l297_enable habilitacion ){

	switch (habilitacion){

		case motor_enable:
			gpioWrite(steppermotor->Gpioenable,ON);
			steppermotor->enable=habilitacion;
			break;

		case motor_disable:
			gpioWrite(steppermotor->Gpioenable,OFF);
			steppermotor->enable=habilitacion;
			break;

		default:
			gpioWrite(steppermotor->Gpioenable,ON);
			steppermotor->enable=habilitacion;

			break;
		}
}

steppermotor_l297_enable stepperMotorL297GetEnable(steppermotor_l297_t *steppermotor){

	return steppermotor->enable;
}

void stepperMotorL297SetReset(steppermotor_l297_t *steppermotor,steppermotor_l297_set_reset setreset){

	switch (setreset){

		case l297_set:
			gpioWrite(steppermotor->Gpioreset,ON);
			steppermotor->set_reset_l297=setreset;
			break;

		case l297_reset:
			gpioWrite(steppermotor->Gpioreset,OFF);
			steppermotor->set_reset_l297=setreset;
			break;

		default:
			gpioWrite(steppermotor->Gpioreset,ON);
			steppermotor->set_reset_l297=setreset;
			break;
		}
}

steppermotor_l297_set_reset stepperMotorL297Get_SetReset(steppermotor_l297_t *steppermotor){

	return steppermotor->set_reset_l297;
}

void stepperMotorL297SetFullHalf(steppermotor_l297_t *steppermotor,steppermotor_l297_half_full secuenciapasos){

	switch (secuenciapasos){

		case l297_half:
			gpioWrite(steppermotor->Gpiohalf_full_step,ON);
			steppermotor->half_full_l297=secuenciapasos;
			break;

		case l297_full:
			gpioWrite(steppermotor->Gpiohalf_full_step,OFF);
			steppermotor->half_full_l297=secuenciapasos;
			break;

		default:
			gpioWrite(steppermotor->Gpiohalf_full_step,ON);
			steppermotor->half_full_l297=secuenciapasos;
			break;
		}
}

steppermotor_l297_half_full stepperMotorL297GetHalfFull(steppermotor_l297_t *steppermotor){

	return steppermotor->half_full_l297;
}



void stepperMotorL297SetVelocidad(steppermotor_l297_t *steppermotor,steppermotor_l297_velocidad velocidad){
	steppermotor->velocidad=velocidad;

//Aqui iria el cambio del valor en set math para variar la velocidad del clock
	//Este seteo d elas velocidades ira acopañado de cuando se colocque el motor y ver
	//los resultados de los ensayos

	switch (steppermotor->velocidad){
	case velocidad_baja:
		//Chip_TIMER_Reset( SIGNAL_TIMER );
		Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,BAJA);
		break;

	case velocidad_media:
		//Chip_TIMER_Reset( SIGNAL_TIMER );
		Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,MEDIA);
		break;

	case velocidad_alta:
		//Chip_TIMER_Reset( SIGNAL_TIMER );
		Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,ALTA);
		break;

	default:
		Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,BAJA);
		break;
	}
}
//Falta funcion que me diga que velocidad esta

//------Funciones para el movimiento del motor-------

/*De acuerdo a lo estudiado en el manual del instrumento, el engranaje que mueve la varilla rosacada milimetrica
 * tiene 90 dientes y por lo deducido del original del instrumento, cada vez que el engranaje se mueve 3 dientes
 * esto es un 1 nanometro. El motor es de 48 pasos(7,5°) y tiene un engranaje de 12 dientes o sea que cuando hace una vuelta
 * completa de 48 pasos , se mueven los 12 dientes.
 * 48/12 =4 , esto seria cada 4 pasos muevo un diente, por lo que para mover 3 dientes(1 Nanometro)necesitor hacer 12 pasos
 * 12 pasos del motor=1 Nanometro
 * el instrumento tiene 0- a 1050 Nanometros o sea seria de 0 a 12600 pasos
*/
void stepperMotorL297MoveXNanometers(steppermotor_l297_t *steppermotor,uint32_t LongOnda){

	uint8_t flag=1;

	PosicionDeseada=LongOnda*NANOMT_XPASO;

	if (PosicionDeseada==PosicionActual){
		steppermotor->estado_motor=motor_estado_inicial;
		}
	else {
		if (PosicionDeseada>PosicionActual){
			if (PosicionDeseada>PASO_MAXIMO){ //Si el valor es mayor al maximo que tiene el motor deshabilito el mismo
				steppermotor->estado_motor=motor_estado_final;  //Aqui habria que enviar un mensaje de error, diciendo que supera el valor maximo
			}
			else{
				steppermotor->estado_motor=motor_estado_avance;
				DiferenciaPosicion=PosicionDeseada-PosicionActual;
			}
		}
		else{
			steppermotor->estado_motor=motor_estado_retroceso;
			DiferenciaPosicion=PosicionActual-PosicionDeseada; //Calculo la diferencia es lo que tendria que "retroceder" el motor

			}
	}

	while (flag){
	switch (steppermotor->estado_motor){

		case motor_estado_inicial:
			//Ver que hacemos aqui
            flag=0;
			break;

		case motor_estado_avance:
			//habilito el motor
			stepperMotorL297SetEnable(steppermotor,motor_enable);
			//habilito giro en sentido agujas de reloj
			stepperMotorL297SetDireccionGiro(steppermotor,sentido_cw);
            //lanzo timmer
			signalStart();
			while(countIrq<=2*DiferenciaPosicion); //Quedo en un lazo cerrado hasta que cuente las interrupciones con la posiciondeseada
			steppermotor->estado_motor=motor_estado_final;

			break;

		case motor_estado_retroceso:
			//habilito el motor
			stepperMotorL297SetEnable(steppermotor,motor_enable);
			//habilito giro en sentido contrario a las  agujas de reloj
			stepperMotorL297SetDireccionGiro(steppermotor,sentido_ccw);
			//lanzo timmer
			signalStart();
			while(countIrq<=2*DiferenciaPosicion); //Quedo en un lazo cerrado hasta que cuente las interrupciones con la posiciondeseada
			steppermotor->estado_motor=motor_estado_final;


			break;

		case motor_estado_final:
			stepperMotorL297SetEnable(steppermotor,motor_disable );
			signalStop();
			countIrq=0;
			PosicionActual=PosicionDeseada;
			flag=0;

			break;

		default:

			break;
		}
	}

}

//Muevo un nanometro en sentido de las agujas del reloj
void stepperMotorL297Move1nanometerCW(steppermotor_l297_t *steppermotor){

	            int i=0;
	            countIrq=0;
	            //habilito el motor
				stepperMotorL297SetEnable(steppermotor,motor_enable);
				//habilito giro en sentido agujas de reloj
				stepperMotorL297SetDireccionGiro(steppermotor,sentido_cw);
	            //lanzo timmer

				while(countIrq<=2*NANOMT_XPASO){
					signalStart();
				}
				stepperMotorL297SetEnable(steppermotor,motor_disable);
				signalStop();
}

//Muevo un nanometro en sentido de las agujas del reloj
void stepperMotorL297Move1nanometerCCW(steppermotor_l297_t *steppermotor){

	            int i=0;
	            countIrq=0;
	            //habilito el motor
				stepperMotorL297SetEnable(steppermotor,motor_enable);
				//habilito giro en sentido agujas de reloj
				stepperMotorL297SetDireccionGiro(steppermotor,sentido_ccw);
	            //lanzo timmer

				while(countIrq<=2*NANOMT_XPASO){
					signalStart();
				}
				stepperMotorL297SetEnable(steppermotor,motor_disable);
				signalStop();


}

/*Funcion para el barrido de longitud de onda
 * Esta seria una funcion que cuando se selecciona el ensayo ese,
 * el motor primero se posicionaria en la posicion cero, scwitch de poscion en cero
 * Si el motor no esta en el swicth cero , giraria en sentido contrario a las agujas del reloj
 * hasta posicionarse en cero.
 * Una vez realizado esto comenzaria el movimiento del motor en sentido de las agujas del reloj
 * hasta contar los 12600 pasos(1050 Nanometros)
 *
 */







/*=====[Implementaciones de funciones de interrupcion publicas]==============*/
//contador de interrupciones,este contador contara tanto las interrupciones producidas por flancos positivos,
//como por flancos negativos del clock externo por GPIO3,

void TIMER2_IRQHandler(void){
	static bool On = false;

	if (Chip_TIMER_MatchPending(SIGNAL_TIMER, 2)) {

		gpioToggle(LEDB);
		countIrq++;
		Chip_TIMER_ClearMatch(SIGNAL_TIMER, 2);

		//On = (bool) !On;
		//Board_LED_Set(0, On);
	}
}


/*=====[Implementaciones de funciones privadas]==============================*/

static void signalInit( void ){

   // Set pin to Timer match output mode (generate signal), se utiliza el pin GPIO03
   Chip_SCU_PinMux( SIGNAL_PIN_SCU_PORT,
                    SIGNAL_PIN_SCU_PIN,
                    SCU_MODE_INACT | SCU_MODE_ZIF_DIS,
                    SIGNAL_PIN_FUNC );

   // Enable timer clock and reset it
   Chip_TIMER_Init( SIGNAL_TIMER );
   Chip_RGU_TriggerReset( SIGNAL_TIMER_RESET );
   while( Chip_RGU_InReset( SIGNAL_TIMER_RESET ) );

   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 0); //=> 204 MHz / 51000 = 4 KHz --4000/2=2000 Hz
   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 1); //=> 204 MHz / 51000 = 4 KHz --4000/4=1000 Hz
   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 2); //=> 204 MHz / 51000 = 4 KHz --4000/6=666 Hz
   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 3); //=> 204 MHz / 51000 = 4 KHz --4000/8=500 Hz
   //Chip_TIMER_PrescaleSet(SIGNAL_TIMER, 4); //=> 204 MHz / 51000 = 4 KHz --4000/10=400 Hz


   // Timer setup for match output toggle
   Chip_TIMER_Reset( SIGNAL_TIMER );

   Chip_TIMER_MatchEnableInt(SIGNAL_TIMER, 2);

   Chip_TIMER_SetMatch( SIGNAL_TIMER,SIGNAL_MATCH_NUMBER,SIGNAL_COMPARE_VAUE );

   Chip_TIMER_ResetOnMatchEnable( SIGNAL_TIMER,
                                  SIGNAL_MATCH_NUMBER );

   Chip_TIMER_ExtMatchControlSet( SIGNAL_TIMER,
                                  SIGNAL_INITIAL_STATE,
                                  TIMER_EXTMATCH_TOGGLE,
                                  SIGNAL_MATCH_NUMBER );
   /* Enable timer interrupt */
   	NVIC_EnableIRQ(TIMER2_IRQn);
   	NVIC_ClearPendingIRQ(TIMER2_IRQn);
}


static void signalStart( void )
{
   // Enable timer to start to generate signal
   Chip_TIMER_Enable( SIGNAL_TIMER );

}

static void signalStop( void )
{
   // Disable timer to stop to generate signal
   Chip_TIMER_Disable( SIGNAL_TIMER );
}


static void funPrivada(void)
{
   // ...
}

