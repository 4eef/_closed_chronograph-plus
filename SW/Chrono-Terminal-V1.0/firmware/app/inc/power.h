/*!****************************************************************************
* @file    power.h
* @author  4eef
* @version V1.0
* @date    17.02.2017, 4eef
* @brief   --
*/
#ifndef power_H
#define power_H

/*!****************************************************************************
* Include
*/
#include "adc.h"
#include "IR.h"
#include "ug2864.h"
#include "clock.h"
#include "sync.h"
#include "lis3.h"
#include "i2c.h"

/*!****************************************************************************
* User define
*/
#define POWER_STOP              0
#define POWER_RUN               1
#define POWER_RUN_MAX           30
#define POWER_RUN_MIN           1
#define POWER_RUN_DEFAULT       5

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    uint8_t         mode;
    uint16_t        uptimeCurr;
    uint16_t        uptimeSet;
}power_type;

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void powerOff(void);
void powerOn(void);
void initPeriphs(void);
void stopModeConfig(void);

#endif //power_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
