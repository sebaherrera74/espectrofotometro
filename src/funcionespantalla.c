/*=====[Module Name]===========================================================
 * Copyright YYYY Author Compelte Name <author@mail.com>
 * All rights reserved.
 * License: license text or at least name and link 
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Creation Date: YYYY/MM/DD
 */
 
/*=====[Inclusion of own header]=============================================*/

#include "funcionespantalla.h"

#define MSG_PANTALLA1   "El Motor no esta\n posicionado en CERO"
#define MSG_PANTALLA2   "Posicionando motor\nen longitud de\nonda cero"
#define MSG_PANTALLA3   "Ensayo determinada\nlongitud onda\npresione:: TEC1"
#define MSG_PANTALLA3_1 "Ensayo Barrido\nlongitud de onda\npresione:: TEC4"

/*=====[Implementations of public functions]=================================*/

void InitLcd(void){
	TM_ILI9341_Init();
}

void RotarPantalla(void){
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_1);
}




void cambiofondo(uint32_t color){
	TM_ILI9341_Fill(color);

}
void posicioncero(void){
	//TM_ILI9341_Fill(ILI9341_COLOR_RED);
	TM_ILI9341_Puts(0,0, MSG_PANTALLA1, &TM_Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_RED);
	TM_ILI9341_Puts(0,120, MSG_PANTALLA2, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_BLUE);
	TM_ILI9341_Puts(0,120, MSG_PANTALLA2, &TM_Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_RED);
	//delay(250);
}


void barraColores(void){
	uint16_t i=0;
	uint16_t j=0;
	for(i=0;i<25;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_WHITE);
		}
	}
	for(i=25;i<50;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_BLACK);
		}
	}
	for(i=50;i<75;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_RED);
		}
	}
	for(i=75;i<100;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_GREEN);
		}
	}
	for(i=100;i<125;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_GREEN2);
		}
	}
	for(i=125;i<150;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_BLUE);
		}
	}
	for(i=150;i<175;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_BLUE2);
		}
	}
	for(i=175;i<200;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_YELLOW);
		}
	}
	for(i=200;i<225;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_ORANGE);
		}
	}
	for(i=225;i<250;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_CYAN);
		}
	}
	for(i=250;i<275;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_MAGENTA);
		}
	}
	for(i=275;i<300;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_GRAY);
		}
	}
	for(i=300;i<320;i++){
		for(j=0;j<320;j++){
			TM_ILI9341_DrawPixel(i,j,ILI9341_COLOR_BROWN);
		}
	}
}

void tipoensayos(void){
	TM_ILI9341_Puts(0,0, MSG_PANTALLA3, &TM_Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_RED);
	TM_ILI9341_Puts(0,120, MSG_PANTALLA3_1, &TM_Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_RED);
}

void ensayobarrido(void){

}

void ensayolongitudonda(void){

}

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/



/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/
