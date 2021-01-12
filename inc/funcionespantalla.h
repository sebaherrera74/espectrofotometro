/*=====[Module Name]===========================================================
 * Copyright YYYY Author Compelte Name <author@mail.com>
 * All rights reserved.
 * License: license text or at least name and link 
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 0.0.0
 * Creation Date: YYYY/MM/DD
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _FUNCIONESPANTALLA_H_
#define _FUNCIONESPANTALLA_H_

#include "sapi.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
/*=====[Inclusions of public function dependencies]==========================*/

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/
void InitLcd(void);
void RotarPantalla(void);
void cambiofondo(uint32_t color);
void posicioncero(void);

//Me pinta la barra de colores
void barraColores(void);

void tipoensayos(void);
void ensayobarrido(void);
void ensayolongitudonda(void);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _MODULE_NAME_H_ */
