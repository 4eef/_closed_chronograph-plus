/*!****************************************************************************
* @file    sync.h
* @author  4eef
* @version V1.0
* @date    17.01.2017, 4eef
* @brief   --
*/
#ifndef sync_H
#define sync_H

/*!****************************************************************************
* Include
*/
#include "stm32f0xx.h"

/*!****************************************************************************
* User define
*/
#define F_APB1                              48000000
#define CYC_PERIOD                          40000//25000

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void syncInit(void);
uint8_t sync(void);

#endif //sync_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
