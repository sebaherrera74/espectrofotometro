/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/04/library-08-ili9341-lcd-on-stm32f429-discovery-board/
 * @version v1.3
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   ILI9341 library for STM32F4xx with SPI communication, without LTDC hardware
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_ILI9341_H
#define TM_ILI9341_H 130

/**
 * @addtogroup TM_STM32F4xx_Libraries
 * @{
 */

/**
 * @defgroup TM_ILI9341
 * @brief    ILI9341 library for STM32F4xx with SPI communication, without LTDC hardware - http://stm32f4-discovery.com/2014/04/library-08-ili9341-lcd-on-stm32f429-discovery-board/
 * @{
 *
 * This driver works for all STM32F4xx series with built in SPI peripheral.
 *	
 * \par Default pinout
 *
@verbatim
ILI9341      STM32F4xx    DESCRIPTION
		
SDO (MISO    PF8          Output from LCD for SPI.	Not used, can be left
LED          3.3V         Backlight
SCK          PF7          SPI clock
SDI (MOSI)   PF9          SPI master output
WRX or D/C   PD13         Data/Command register
RESET        PD12         Reset LCD
CS           PC2          Chip select for SPI
GND          GND          Ground
VCC          3.3V         Positive power supply
@endverbatim
 *		
 * All pins can be changed in your defines.h file
 *		
@verbatim
//Default SPI used is SPI5. Check my SPI library for other pinouts
#define ILI9341_SPI           SPI5
#define ILI9341_SPI_PINS      TM_SPI_PinsPack_1
		
//Default CS pin. Edit this in your defines.h file
#define ILI9341_CS_PORT       GPIOC
#define ILI9341_CS_PIN        GPIO_PIN_2
		
//Default D/C (or WRX) pin. Edit this in your defines.h file
#define ILI9341_WRX_PORT      GPIOD
#define ILI9341_WRX_PIN       GPIO_PIN_13
@endverbatim
 *
 * Reset pin can be disabled, if you need GPIOs for other purpose.
 * To disable RESET pin, add line below to defines.h file
 * If you disable pin, then set LCD's RESET pin to VCC.
 *	
@verbatim
//Disable RESET pin
#define ILI9341_USE_RST_PIN			0
@endverbatim
 *		
 * But if you want to use RESET pin, you can change its settings in defines.h file
 *	
@verbatim
//Default RESET pin. Edit this in your defines.h file
#define ILI9341_RST_PORT			GPIOD
#define ILI9341_RST_PIN				GPIO_PIN_12
@endverbatim
 *
 * \par Changelog
 *
@verbatim
 Version 1.3
  - June 06, 2015
  - Added support for SPI DMA for faster refreshing
 
 Version 1.2
  - March 14, 2015
  - Added support for new GPIO system
  - Added functions TM_ILI9341_DisplayOff() and TM_ILI9341_DisplayOn()
 
 Version 1.0
  - First release
@endverbatim
 *
 * \par Dependencies
 *
@verbatim
 - STM32F4xx
 - STM32F4xx RCC
 - STM32F4xx GPIO
 - STM32F4xx SPI
 - defines.h
 - TM SPI
 - TM DMA
 - TM SPI DMA
 - TM FONTS
 - TM GPIO
@endverbatim
 */

#include "tm_stm32f4_fonts.h"

/**
 * @defgroup TM_ILI9341_Macros
 * @brief    Library defines
 * @{
 */


/* LCD settings */
#define ILI9341_WIDTH        240
#define ILI9341_HEIGHT       320
#define ILI9341_PIXEL        76800

/* Colors */
#define ILI9341_COLOR_WHITE			0xFFFF
#define ILI9341_COLOR_BLACK			0x0000
#define ILI9341_COLOR_RED           0xF800
#define ILI9341_COLOR_GREEN			0x07E0
#define ILI9341_COLOR_GREEN2		0xB723
#define ILI9341_COLOR_BLUE			0x001F
#define ILI9341_COLOR_BLUE2			0x051D
#define ILI9341_COLOR_YELLOW		0xFFE0
#define ILI9341_COLOR_ORANGE		0xFBE4
#define ILI9341_COLOR_CYAN			0x07FF
#define ILI9341_COLOR_MAGENTA		0xA254
#define ILI9341_COLOR_GRAY			0x7BEF
#define ILI9341_COLOR_BROWN			0xBBCA


// Color definitions
#define ILI9341_ALICEBLUE 0xF7DF
#define ILI9341_ANTIQUEWHITE 0xFF5A
#define ILI9341_AQUA 0x07FF
#define ILI9341_AQUAMARINE 0x7FFA
#define ILI9341_AZURE 0xF7FF
#define ILI9341_BEIGE 0xF7BB
#define ILI9341_BISQUE 0xFF38
#define ILI9341_BLACK 0x0000
#define ILI9341_BLANCHEDALMOND 0xFF59
#define ILI9341_BLUE 0x001F
#define ILI9341_BLUEVIOLET 0x895C
#define ILI9341_BROWN 0xA145
#define ILI9341_BURLYWOOD 0xDDD0
#define ILI9341_CADETBLUE 0x5CF4
#define ILI9341_CHARTREUSE 0x7FE0
#define ILI9341_CHOCOLATE 0xD343
#define ILI9341_CORAL 0xFBEA
#define ILI9341_CORNFLOWERBLUE 0x64BD
#define ILI9341_CORNSILK 0xFFDB
#define ILI9341_CRIMSON 0xD8A7
#define ILI9341_CYAN 0x07FF
#define ILI9341_DARKBLUE 0x0011
#define ILI9341_DARKCYAN 0x0451
#define ILI9341_DARKGOLDENROD 0xBC21
#define ILI9341_DARKGRAY 0xAD55
#define ILI9341_DARKGREEN 0x0320
#define ILI9341_DARKKHAKI 0xBDAD
#define ILI9341_DARKMAGENTA 0x8811
#define ILI9341_DARKOLIVEGREEN 0x5345
#define ILI9341_DARKORANGE 0xFC60
#define ILI9341_DARKORCHID 0x9999
#define ILI9341_DARKRED 0x8800
#define ILI9341_DARKSALMON 0xECAF
#define ILI9341_DARKSEAGREEN 0x8DF1
#define ILI9341_DARKSLATEBLUE 0x49F1
#define ILI9341_DARKSLATEGRAY 0x2A69
#define ILI9341_DARKTURQUOISE 0x067A
#define ILI9341_DARKVIOLET 0x901A
#define ILI9341_DEEPPINK 0xF8B2
#define ILI9341_DEEPSKYBLUE 0x05FF
#define ILI9341_DIMGRAY 0x6B4D
#define ILI9341_DODGERBLUE 0x1C9F
#define ILI9341_FIREBRICK 0xB104
#define ILI9341_FLORALWHITE 0xFFDE
#define ILI9341_FORESTGREEN 0x2444
#define ILI9341_FUCHSIA 0xF81F
#define ILI9341_GAINSBORO 0xDEFB
#define ILI9341_GHOSTWHITE 0xFFDF
#define ILI9341_GOLD 0xFEA0
#define ILI9341_GOLDENROD 0xDD24
#define ILI9341_GRAY 0x8410
#define ILI9341_GREEN 0x0400
#define ILI9341_GREENYELLOW 0xAFE5
#define ILI9341_HONEYDEW 0xF7FE
#define ILI9341_HOTPINK 0xFB56
#define ILI9341_INDIANRED 0xCAEB
#define ILI9341_INDIGO 0x4810
#define ILI9341_IVORY 0xFFFE
#define ILI9341_KHAKI 0xF731
#define ILI9341_LAVENDER 0xE73F
#define ILI9341_LAVENDERBLUSH 0xFF9E
#define ILI9341_LAWNGREEN 0x7FE0
#define ILI9341_LEMONCHIFFON 0xFFD9
#define ILI9341_LIGHTBLUE 0xAEDC
#define ILI9341_LIGHTCORAL 0xF410
#define ILI9341_LIGHTCYAN 0xE7FF
#define ILI9341_LIGHTGOLDENRODYELLOW 0xFFDA
#define ILI9341_LIGHTGREEN 0x9772
#define ILI9341_LIGHTGREY 0xD69A
#define ILI9341_LIGHTPINK 0xFDB8
#define ILI9341_LIGHTSALMON 0xFD0F
#define ILI9341_LIGHTSEAGREEN 0x2595
#define ILI9341_LIGHTSKYBLUE 0x867F
#define ILI9341_LIGHTSLATEGRAY 0x7453
#define ILI9341_LIGHTSTEELBLUE 0xB63B
#define ILI9341_LIGHTYELLOW 0xFFFC
#define ILI9341_LIME 0x07E0
#define ILI9341_LIMEGREEN 0x3666
#define ILI9341_LINEN 0xFF9C
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_MAROON 0x8000
#define ILI9341_MEDIUMAQUAMARINE 0x6675
#define ILI9341_MEDIUMBLUE 0x0019
#define ILI9341_MEDIUMORCHID 0xBABA
#define ILI9341_MEDIUMPURPLE 0x939B
#define ILI9341_MEDIUMSEAGREEN 0x3D8E
#define ILI9341_MEDIUMSLATEBLUE 0x7B5D
#define ILI9341_MEDIUMSPRINGGREEN 0x07D3
#define ILI9341_MEDIUMTURQUOISE 0x4E99
#define ILI9341_MEDIUMVIOLETRED 0xC0B0
#define ILI9341_MIDNIGHTBLUE 0x18CE
#define ILI9341_MINTCREAM 0xF7FF
#define ILI9341_MISTYROSE 0xFF3C
#define ILI9341_MOCCASIN 0xFF36
#define ILI9341_NAVAJOWHITE 0xFEF5
#define ILI9341_NAVY 0x0010
#define ILI9341_OLDLACE 0xFFBC
#define ILI9341_OLIVE 0x8400
#define ILI9341_OLIVEDRAB 0x6C64
#define ILI9341_ORANGE 0xFD20
#define ILI9341_ORANGERED 0xFA20
#define ILI9341_ORCHID 0xDB9A
#define ILI9341_PALEGOLDENROD 0xEF55
#define ILI9341_PALEGREEN 0x9FD3
#define ILI9341_PALETURQUOISE 0xAF7D
#define ILI9341_PALEVIOLETRED 0xDB92
#define ILI9341_PAPAYAWHIP 0xFF7A
#define ILI9341_PEACHPUFF 0xFED7
#define ILI9341_PERU 0xCC27
#define ILI9341_PINK 0xFE19
#define ILI9341_PLUM 0xDD1B
#define ILI9341_POWDERBLUE 0xB71C
#define ILI9341_PURPLE 0x8010
#define ILI9341_RED 0xF800
#define ILI9341_ROSYBROWN 0xBC71
#define ILI9341_ROYALBLUE 0x435C
#define ILI9341_SADDLEBROWN 0x8A22
#define ILI9341_SALMON 0xFC0E
#define ILI9341_SANDYBROWN 0xF52C
#define ILI9341_SEAGREEN 0x2C4A
#define ILI9341_SEASHELL 0xFFBD
#define ILI9341_SIENNA 0xA285
#define ILI9341_SILVER 0xC618
#define ILI9341_SKYBLUE 0x867D
#define ILI9341_SLATEBLUE 0x6AD9
#define ILI9341_SLATEGRAY 0x7412
#define ILI9341_SNOW 0xFFDF
#define ILI9341_SPRINGGREEN 0x07EF
#define ILI9341_STEELBLUE 0x4416
#define ILI9341_TAN 0xD5B1
#define ILI9341_TEAL 0x0410
#define ILI9341_THISTLE 0xDDFB
#define ILI9341_TOMATO 0xFB08
#define ILI9341_TURQUOISE 0x471A
#define ILI9341_VIOLET 0xEC1D
#define ILI9341_WHEAT 0xF6F6
#define ILI9341_WHITE 0xFFFF
#define ILI9341_WHITESMOKE 0xF7BE
#define ILI9341_YELLOW 0xFFE0
#define ILI9341_YELLOWGREEN 0x9E66

/* Transparent background, only for strings and chars */
#define ILI9341_TRANSPARENT			0x80000000

/**
 * @}
 */
 
/**
 * @defgroup TM_ILI9341_Typedefs
 * @brief    Library Typedefs
 * @{
 */


/**
 * @brief  Possible orientations for LCD
 */
typedef enum {
	TM_ILI9341_Orientation_Portrait_1,  /*!< Portrait orientation mode 1 */
	TM_ILI9341_Orientation_Portrait_2,  /*!< Portrait orientation mode 2 */
	TM_ILI9341_Orientation_Landscape_1, /*!< Landscape orientation mode 1 */
	TM_ILI9341_Orientation_Landscape_2  /*!< Landscape orientation mode 2 */
} TM_ILI9341_Orientation_t;

/**
 * @}
 */

/**
 * @defgroup TM_ILI9341_Functions
 * @brief    Library Functions
 * @{
 */



void TM_ILI9341_SendRAM(uint16_t color);



/**
 * @brief  Initializes ILI9341 LCD with LTDC peripheral
 *         It also initializes external SDRAM
 * @param  None
 * @retval None
 */
void TM_ILI9341_Init(void);

/**
 * @brief  Draws single pixel to LCD
 * @param  x: X position for pixel
 * @param  y: Y position for pixel
 * @param  color: Color of pixel
 * @retval None
 */
void TM_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief  Fills entire LCD with color
 * @param  color: Color to be used in fill
 * @retval None
 */
void TM_ILI9341_Fill(uint32_t color);

/**
 * @brief  Rotates LCD to specific orientation
 * @param  orientation: LCD orientation. This parameter can be a value of @ref TM_ILI9341_Orientation_t enumeration
 * @retval None
 */
void TM_ILI9341_Rotate(TM_ILI9341_Orientation_t orientation);

/**
 * @brief  Puts single character to LCD
 * @param  x: X position of top left corner
 * @param  y: Y position of top left corner
 * @param  c: Character to be displayed
 * @param  *font: Pointer to @ref TM_FontDef_t used font
 * @param  foreground: Color for char
 * @param  background: Color for char background
 * @retval None
 */
void TM_ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t* font, uint32_t foreground, uint32_t background);

/**
 * @brief  Puts string to LCD
 * @param  x: X position of top left corner of first character in string
 * @param  y: Y position of top left corner of first character in string
 * @param  *str: Pointer to first character
 * @param  *font: Pointer to @ref TM_FontDef_t used font
 * @param  foreground: Color for string
 * @param  background: Color for string background
 * @retval None
 */
void TM_ILI9341_Puts(uint16_t x, uint16_t y, char* str, TM_FontDef_t *font, uint32_t foreground, uint32_t background);

/**
 * @brief  Gets width and height of box with text //Obtiene el ancho y el alto del cuadro con texto
 * @param  *str: Pointer to first character
 * @param  *font: Pointer to @ref TM_FontDef_t used font
 * @param  *width: Pointer to variable to store width
 * @param  *height: Pointer to variable to store height
 * @retval None
 */
void TM_ILI9341_GetStringSize(char* str, TM_FontDef_t* font, uint16_t* width, uint16_t* height);

/**
 * @brief  Draws line to LCD
 * @param  x0: X coordinate of starting point
 * @param  y0: Y coordinate of starting point
 * @param  x1: X coordinate of ending point
 * @param  y1: Y coordinate of ending point
 * @param  color: Line color
 * @retval None
 */
void TM_ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

/**
 * @brief  Draws rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Rectangle color
 * @retval None
 */
void TM_ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

/**
 * @brief  Draws filled rectangle on LCD
 * @param  x0: X coordinate of top left point
 * @param  y0: Y coordinate of top left point
 * @param  x1: X coordinate of bottom right point
 * @param  y1: Y coordinate of bottom right point
 * @param  color: Rectangle color
 * @retval None
 */
void TM_ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

/**
 * @brief  Draws circle on LCD
 * @param  x0: X coordinate of center circle point
 * @param  y0: Y coordinate of center circle point
 * @param  r: Circle radius
 * @param  color: Circle color
 * @retval None
 */
void TM_ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);

/**
 * @brief  Draws filled circle on LCD
 * @param  x0: X coordinate of center circle point
 * @param  y0: Y coordinate of center circle point
 * @param  r: Circle radius
 * @param  color: Circle color
 * @retval None
 */
void TM_ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);

/**
 * @brief   Enables display
 * @note    After initialization, LCD is enabled and you don't need to call this function
 * @param   None
 * @retval  None
 */

void TM_ILI9341_DrawCircle_N(int16_t x0, int16_t y0, int16_t r,uint8_t n, uint32_t color);
void arc(int16_t x0, int16_t y0, int16_t r, uint32_t color);

void TM_ILI9341_DisplayOn(void);

/**
 * @brief   Disables display
 * @param   None
 * @retval  None
 */
void TM_ILI9341_DisplayOff(void);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif

