/*!****************************************************************************
* @file    menuFunctions.h
* @author  4eef
* @version V1.0
* @date    23.10.2018, 4eef
* @brief   --
*/
#ifndef menuFunctions_H
#define menuFunctions_H

/*!****************************************************************************
* Include
*/
#include "string.h"
#include "microMenu.h"
#include "system.h"

/*!****************************************************************************
* User define
*/

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/

/*!****************************************************************************
* Extern viriables
*/
extern const char statsTxt[];

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void statsParser(void);
void aboutParser(void);
void sinfoParser(void);
void swResFunc(void);
void chrBindFunc(void);
void offsCalFunc(void);
void gainCalFunc(void);
void chrSetsRst(void);
void clipReload(void);

#endif //menuFunctions_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
