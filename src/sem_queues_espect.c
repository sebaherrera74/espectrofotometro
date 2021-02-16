
#include "sem_queues_espect.h"
#include "debouncetecla.h"

#include "sapi.h"


int sem_queues_init(void)
{
	//----Creacion de Colas----

   valorLO_queue     =xQueueCreate( 1, sizeof(uint16_t) );   //cola para pasar valor de longitud de onda
   valorLOselec_queue=xQueueCreate( 1, sizeof(uint16_t) );	//cola para pasar valor de longitud de onda seleccionada
   valorAnLeido      =xQueueCreate( 1, sizeof(char[10]) );
   txQueue           =xQueueCreate(128, sizeof(char));      //cola para enviar datos por la UART
   valormaximoLO_queue=xQueueCreate( 1, sizeof(uint16_t) );


   if((valorLO_queue==NULL)||(valorLOselec_queue==NULL)||(valorAnLeido==NULL)||(txQueue==NULL)||(valormaximoLO_queue==NULL))//chequeo que se hallan creado bien las colas
	{
	  printf ("Error al crear colas");
	  return 1;
	}

   //------Creacion de semaforos Mutex--------
   mutex_impresion	=  xSemaphoreCreateMutex();
   mutex_impresion_1=  xSemaphoreCreateMutex();

   if((mutex_impresion==NULL)||(mutex_impresion_1==NULL) )//verifico semaforos mutex
     {
      printf ("Error al crear Semaforos Mutex ");
      return 1;
     }

   //-----Creacion de Semaforos binarios--------



   sem_posicioncero=xSemaphoreCreateBinary();
   sem_inicio=xSemaphoreCreateBinary();
   sem_inicio_barrido=xSemaphoreCreateBinary();
   sem_final_barrido =xSemaphoreCreateBinary();
   sem_motorposicionadoLOD=xSemaphoreCreateBinary();


   if((sem_inicio_barrido==NULL)||(sem_final_barrido==NULL)||(sem_inicio==NULL)||
		   sem_posicioncero==NULL) //Verifico creacion de semaforos binarios
        {
         printf ("Error al crear Semaforos binarios ");
         return 1;
        }
   return 0;
}

int teclas_config(void){
	tecla_config[0].tecla= TEC1;
	tecla_config[0].sem_tec_pulsada	= xSemaphoreCreateBinary();

	tecla_config[1].tecla= TEC2;
	tecla_config[1].sem_tec_pulsada	= xSemaphoreCreateBinary();

	tecla_config[2].tecla= TEC3;
	tecla_config[2].sem_tec_pulsada	= xSemaphoreCreateBinary();

	tecla_config[3].tecla= TEC4;
	tecla_config[3].sem_tec_pulsada = xSemaphoreCreateBinary();

	if((tecla_config[0].sem_tec_pulsada==NULL)||(tecla_config[1].sem_tec_pulsada	==NULL)||(tecla_config[2].sem_tec_pulsada==NULL)||
			tecla_config[3].sem_tec_pulsada==NULL) //Verifico creacion de semaforos binarios
	        {
	         printf ("Error al crear Semaforos binarios ");
	         return 1;
	        }
   return 0;
}


