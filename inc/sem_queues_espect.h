
#include "FreeRTOS.h"
#include "semphr.h"
#include "sapi.h"


#ifndef _SEM_QUEUES_ESPECT_H_
#define _SEM_QUEUES_ESPECT_H_


//Aqui van todos los semaforos binarios, mutex y colas ya que son variables globales a utilizar

//SemaphoreHandle_t fin_posicion_motor;

SemaphoreHandle_t sem_posicioncero;
SemaphoreHandle_t sem_inicio_barrido;
SemaphoreHandle_t sem_final_barrido;
SemaphoreHandle_t sem_inicio;
SemaphoreHandle_t sem_motorposicionadoLOD; //Semaforo que avisa que el motor ya esta en la longitud de onda determinada

SemaphoreHandle_t mutex_impresion;
SemaphoreHandle_t mutex_impresion_1;


QueueHandle_t valorLO_queue;
QueueHandle_t valorLOselec_queue;
QueueHandle_t valorAnLeido;       //Valor analogico leido
QueueHandle_t txQueue;

int sem_queues_init(void);





#endif
