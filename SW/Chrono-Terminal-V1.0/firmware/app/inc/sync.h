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
#include "power.h"
#include "MicroMenu.h"
#include "system.h"

/*!****************************************************************************
* User define
*/
#define F_APB1                  48000000
#define CYC_PERIOD_US           50000//25000
#define CYC_POFF_MAX            50
#define MIN_TO_US               60*1000000
#define S_TO_US                 1000000
#define FIVE_SEC_WARNING        (S_TO_US/CYC_PERIOD_US)*5

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
void syncDeinit(void);
void sync(void);

#endif //sync_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
