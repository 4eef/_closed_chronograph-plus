/*!****************************************************************************
* @file    main.h
* @author  4eef
* @version V1.0
* @date    07.08.2016
* @brief   --
*/
#ifndef main_H
#define main_H

/*!****************************************************************************
* Include
*/
#include "stm32f0xx.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "clock.h"
#include "gpio.h"
#include "i2c.h"
#include "delay.h"
#include "ug2864.h"
#include "lis3.h"
#include "MicroMenu.h"
#include "IR.h"
#include "adc.h"
#include "sync.h"
#include "power.h"
#include "buttons.h"
#include "battery.h"
#include "system.h"
#include "fix16.h"

/*!****************************************************************************
* User define
*/
#define INC_BORDER_MAX          90
#define INC_BORDER_MIN          1

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
void main(void);
void parEditRedir(void);
int16_t parEdit(int16_t param);
void modeEdit(void);
void drawDisplay(void);

#endif //main_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
