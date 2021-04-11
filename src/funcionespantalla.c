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
#define MSG_PANTALLA3   "Ensayo determinada\n   longitud onda\n"
#define MSG_PANTALLA3_1 "Ensayo Barrido\n longitud de onda\n"
#define MSG_PANTALLA4   "Seleccione Longitud \n      de Onda"
#define MSG_PANTALLA5   "Desea realizar el\nensayo en la\nlongitud de onda ?"
#define MSG_PANTALLA6   "SI Presione  ENTER"
#define MSG_PANTALLA7   "NO Presione RETURN"
#define MSG_PANTALLA8   "Longitud de onda\nseleccionada es:"
#define MSG_PANTALLA8_1 "Absobancia:"
#define MSG_PANTALLA8_2 "Transmitancia: "
#define MSG_PANTALLA9   "Desea realizar el\nensayo barrido\nlongitud de onda ?"
#define MSG_PANTALLA10  "POSICIONANDO MOTOR\nEspere....."
#define MSG_PANTALLA11  "Realizando ensayo\nlongitud de onda\nEspere....."

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
	TM_ILI9341_Puts(0,0, MSG_PANTALLA1, &TM_Font_16x26, COLORLETRA, ILI9341_COLOR_RED);
	TM_ILI9341_Puts(0,120, MSG_PANTALLA2, &TM_Font_11x18, COLORLETRA, ILI9341_COLOR_BLUE);
	TM_ILI9341_Puts(0,120, MSG_PANTALLA2, &TM_Font_11x18, COLORLETRA, ILI9341_COLOR_RED);
}

void barraColores(void){
	uint16_t i=0;
	uint16_t j=0;
	uint8_t  ancho=25;
	uint32_t colores[13]={ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK,ILI9341_COLOR_RED,ILI9341_COLOR_GREEN,
			ILI9341_COLOR_GREEN2,ILI9341_COLOR_BLUE,ILI9341_COLOR_BLUE2,ILI9341_COLOR_YELLOW,
			ILI9341_COLOR_ORANGE,ILI9341_COLOR_CYAN,ILI9341_COLOR_MAGENTA,ILI9341_COLOR_GRAY,
			ILI9341_COLOR_BROWN};

		for(i=0;i<25;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[0]);
			}
		}
		for(i=25;i<50;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[1]);
			}
		}
		for(i=50;i<75;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[2]);
			}
		}
		for(i=75;i<100;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[3]);
			}
		}
		for(i=100;i<125;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[4]);
			}
		}
		for(i=125;i<150;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[5]);
			}
		}
		for(i=150;i<175;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[6]);
			}
		}
		for(i=175;i<200;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[7]);
			}
		}
		for(i=200;i<225;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[8]);
			}
		}
		for(i=225;i<250;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[9]);
			}
		}
		for(i=250;i<275;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[10]);
			}
		}
		for(i=275;i<300;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[11]);
			}
		}
		for(i=300;i<320;i++){
			for(j=0;j<320;j++){
				TM_ILI9341_DrawPixel(i,j,colores[12]);
			}
		}
	}

   void muestravaloranalogico(char * valoranalogicoleido){
	   TM_ILI9341_Puts(220,220,valoranalogicoleido , &TM_Font_11x18, COLORLETRA, ILI9341_COLOR_YELLOW);
   }

	void tipoensayos(void){
		TM_ILI9341_Puts(0,0, MSG_PANTALLA3, &TM_Font_16x26, COLORLETRA, COLORFONDO);
		TM_ILI9341_Puts(0,120, MSG_PANTALLA3_1, &TM_Font_16x26, COLORLETRA, COLORFONDO);
	}

	void ensayolongitudonda(void){
		TM_ILI9341_Puts(0,0, MSG_PANTALLA3, &TM_Font_16x26, COLORLETRA, ILI9341_COLOR_RED);
		TM_ILI9341_Puts(0,120, MSG_PANTALLA3_1, &TM_Font_16x26, COLORLETRA, COLORFONDO);
	}

	void ensayobarrido(void){
		TM_ILI9341_Puts(0,0, MSG_PANTALLA3, &TM_Font_16x26, COLORLETRA, COLORFONDO);
		TM_ILI9341_Puts(0,120, MSG_PANTALLA3_1, &TM_Font_16x26, COLORLETRA, ILI9341_COLOR_RED);
	}

	void seleccionlongonda(char *valorseleccionado){
		TM_ILI9341_Puts(0,0, MSG_PANTALLA4, &TM_Font_16x26, COLORLETRA, ILI9341_COLOR_RED);
		TM_ILI9341_Puts(130,130, valorseleccionado, &TM_Font_16x26, COLORLETRA, COLORLOFONDO);
	}

	void confirmacionensayo(void){
		TM_ILI9341_Puts(0,0, MSG_PANTALLA5, &TM_Font_16x26, COLORLETRA, ILI9341_COLOR_RED);
		TM_ILI9341_Puts(0,120, MSG_PANTALLA6, &TM_Font_16x26, COLORLETRA, COLORFONDO);
		TM_ILI9341_Puts(0,160, MSG_PANTALLA7, &TM_Font_16x26, COLORLETRA, COLORFONDO);
	}

	void confirmacionensayoEblo(void){
		TM_ILI9341_Puts(0,0, MSG_PANTALLA9, &TM_Font_16x26, COLORLETRA, ILI9341_COLOR_RED);
		TM_ILI9341_Puts(0,120, MSG_PANTALLA6, &TM_Font_16x26, COLORLETRA, COLORFONDO);
		TM_ILI9341_Puts(0,160, MSG_PANTALLA7, &TM_Font_16x26, COLORLETRA, COLORFONDO);

	}

	void valorlongondaselecc(char *valorseleccionado,char*valoranleido,char *transmitancia){
		TM_ILI9341_Puts(0,0, MSG_PANTALLA8, &TM_Font_16x26, COLORLETRA, COLORFONDO);
		TM_ILI9341_Puts(160,60, valorseleccionado, &TM_Font_16x26, COLORLETRA, ILI9341_YELLOW);
		TM_ILI9341_DrawFilledRectangle(0, 100, 320, 110, ILI9341_DARKBLUE  );
		TM_ILI9341_Puts(0,150, MSG_PANTALLA8_1, &TM_Font_16x26, COLORLETRA, COLORFONDO);
		TM_ILI9341_Puts(180,150, valoranleido, &TM_Font_16x26, COLORLETRA,  ILI9341_YELLOW);
		TM_ILI9341_Puts(0,180, MSG_PANTALLA8_2, &TM_Font_16x26, COLORLETRA, COLORFONDO);
		TM_ILI9341_Puts(220,180, transmitancia, &TM_Font_16x26, COLORLETRA,  ILI9341_YELLOW);
		TM_ILI9341_Puts(290,180, "%", &TM_Font_16x26, COLORLETRA,  COLORFONDO);
	}

	void respsi(void){
		TM_ILI9341_Puts(60,120, MSG_PANTALLA6, &TM_Font_16x26, COLORLETRA,ILI9341_COLOR_RED );
	}

	void respno(void){
		TM_ILI9341_Puts(120,120, MSG_PANTALLA7, &TM_Font_16x26, COLORLETRA,ILI9341_COLOR_RED );
	}

	void ensayoenproceso(void){
		TM_ILI9341_Puts(0,0,  MSG_PANTALLA10 , &TM_Font_16x26, COLORLETRA, ILI9341_COLOR_RED);
	}

	void ensayoenproceso_2(void){
		TM_ILI9341_Puts(0,0,  MSG_PANTALLA11 , &TM_Font_16x26, COLORLETRA, ILI9341_COLOR_RED);
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
