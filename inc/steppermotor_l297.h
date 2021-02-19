/*=====[Nombre del modulo]=====================================================
 * Sebastian Herrrera <sebaherrera@gmail.com>
 * All rights reserved.
 * Licencia: Texto de la licencia o al menos el nombre y un link
         (ejemplo: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Fecha de creacion: 2021/01/02
 */

/*=====[Evitar inclusion multiple comienzo]==================================*/

#ifndef _STEPPERMOTOR_L297_H_
#define _STEPPERMOTOR_L297_H_

/*=====[Inclusiones de dependencias de funciones publicas]===================*/

#include "sapi.h"


/*=====[C++ comienzo]========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Macros de definicion de constantes publicas]=========================*/

#define PI 3.14
#define BAJA    204000000
#define MEDIA   1000000
#define ALTA    700000

//Definiciones de .....

#define PASO_MINIMO   0
#define PASO_MAXIMO   12600   //Esta es la cantidad maxima de pasos que dara el motor
#define NANOMT_XPASO  12      //Esto me da la cantidad de pasos x 1 nanometro que el motor tiene que girar

/*=====[Macros estilo funcion publicas]======================================*/


/*=====[Definiciones de tipos de datos publicos]=============================*/

// Tipo de datos que renombra un tipo basico
typedef uint8_t bool_t;

// Tipo de datos de puntero a funcion
typedef void (*callBackFuncPtr_t)(void *);

// Tipo de datos enumerado
typedef enum {
   velocidad_baja,
   velocidad_media,
   velocidad_alta
} steppermotor_l297_velocidad;

typedef enum {
   sentido_cw,      //direccion en las agujas del reloj
   sentido_ccw      //direccion en sentido contrario a las agujas del reloj
} steppermotor_l297_direccion;

typedef enum {
   motor_enable,     //motor habilitado
   motor_disable     //motor deshabilitado
} steppermotor_l297_enable;

typedef enum {
   l297_set,       //set l297
   l297_reset      //reset l297
} steppermotor_l297_set_reset;

typedef enum {
   l297_half,     //half l297
   l297_full      //full l297
} steppermotor_l297_half_full;


typedef enum {
	motor_estado_inicial,     //motor en estado inicial
	motor_estado_avance,     //motor en estado de avance o girnado en sentido de las agujas del reloj
	motor_estado_retroceso,   //motor en estado de avance o girnado en sentido contrario de las agujas del reloj
	motor_estado_final
} steppermotor_l297_estadosmotor;




// Tipo de datos estructurados, union o campo de bits
typedef struct {
	uint32_t cantidad_pasos;
	uint32_t contador_pasos;
	float  Numeros_pasosxvuelta;
	float   angulo_resolucion;
	gpioMap_t Gpiohalf_full_step;
	gpioMap_t Gpioreset;
	gpioMap_t Gpiodireccion;           //sentido agujas del reloj Horario (CW) o antihorario (CCW)
	gpioMap_t Gpioenable;
	gpioMap_t control;
	uint32_t PosicionActual;               //Varaible que me va ha decir la posicion actual del motor
	uint32_t PosicionDeseada;              //Varaible que me va ha decir la psocion deseada en donde quiero posicionar el motor
	uint32_t DiferenciaPosicion;           //diferencia
	steppermotor_l297_direccion direccion;
	steppermotor_l297_velocidad velocidad;
	steppermotor_l297_enable enable;
	steppermotor_l297_estadosmotor estado_motor;
	steppermotor_l297_set_reset set_reset_l297;  //Variable para saber si l297 esta en set o reset
	steppermotor_l297_half_full half_full_l297;  //Varaible para saber si L297 esta en modo half o full
} steppermotor_l297_t;

steppermotor_l297_t  steppermotor;


/*=====[Prototipos de funciones publicas]====================================*/

//Funcion de inicializacion del motor paso a paso a utilizar, se pasa una structura , los numeros de pasos
//por vuelta
//Tambien esta funcion inicializa el Timer() para generar el clock
//seguir agregando lo que hace el timer

void stepperMotorL297Init(steppermotor_l297_t *steppermotor,uint32_t numerodepasosxvuelta,
		                  gpioMap_t enable,gpioMap_t reset,gpioMap_t half_full,gpioMap_t cw_ccw);

//Funcion para setear la velocidad
void stepperMotorL297SetVelocidad(steppermotor_l297_t *steppermotor,steppermotor_l297_velocidad velocidad);

//Funcion para determinar el sentido de Giro
void stepperMotorL297SetDireccionGiro(steppermotor_l297_t *steppermotor,steppermotor_l297_direccion direcciongiro);

//Funcion que me devuelve el sentido de giro en el que esta el motor
steppermotor_l297_direccion stepperMotorL297GetDireccionGiro(steppermotor_l297_t *steppermotor);

//Funcion que habilita el motor o lo deshabilita
void stepperMotorL297SetEnable(steppermotor_l297_t *steppermotor,steppermotor_l297_enable habilitacion );

//Funcion que me dice si el motor esta habilitado o deshabilitado
steppermotor_l297_enable stepperMotorL297GetEnable(steppermotor_l297_t *steppermotor);

//Funcion para setear o resetear el L297 de acuerdo al pin especicificado en la funcion init
void stepperMotorL297SetReset(steppermotor_l297_t *steppermotor,steppermotor_l297_set_reset setreset);

//Funcion para saber en estado esta el L297 si esta set o reset
steppermotor_l297_set_reset stepperMotorL297Get_SetReset(steppermotor_l297_t *steppermotor);

//Funcion para elegir el modo half o full del CI l297
void stepperMotorL297SetFullHalf(steppermotor_l297_t *steppermotor,steppermotor_l297_half_full secuenciapasos);

//Funcion para ver en que modo de secuencia estamos trabajando
steppermotor_l297_half_full stepperMotorL297GetHalfFull(steppermotor_l297_t *steppermotor);

/*Funcion para girar el motor de acuerdo a la longitud de onda seleccionada
 *De acuerdo a la longitud seleccionada girara en el sentido de las agujas del reloj o en sentido contrario
*/
void stepperMotorL297MoveXNanometers(steppermotor_l297_t *steppermotor,uint32_t LongOnda);


/*Funcion que mueve el motor en sentido de las agujas del reloj 1 nanometro*/

void stepperMotorL297Move1nanometerCW(steppermotor_l297_t *steppermotor);

/*Funcion que mueve el motor en sentido contrario de las agujas del reloj 1 nanometro*/

void stepperMotorL297Move1nanometerCCW(steppermotor_l297_t *steppermotor);

void stepperMotorL297ResetPosiciones(steppermotor_l297_t *steppermotor);

/*=====[Prototipos de funciones publicas de interrupcion]====================*/


/*=====[C++ fin]=============================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Evitar inclusion multiple fin]=======================================*/

#endif /* _MODULE_NAME_H_ */
