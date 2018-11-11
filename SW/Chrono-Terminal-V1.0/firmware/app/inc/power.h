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
#include "ug2864.h"
#include "clock.h"
#include "sync.h"
#include "lis3.h"
#include "i2c.h"
#include "buttons.h"

/*!****************************************************************************
* User define
*/
#define POWER_RUN_MAX           30
#define POWER_RUN_MIN           1
#define POWER_RUN_DEFAULT       5

/*!****************************************************************************
* User enum
*/
typedef enum{
    ePwrStop = 0,
    ePwrRun
}ePwrMode_type;

/*!****************************************************************************
* User typedef
*/
typedef struct{
    ePwrMode_type   pwrMode;
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
void powerOn(void);
void powerOff(void);
void powerTimRst(void);
void initPeriphs(void);
void stopModeConfig(void);

#endif //power_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
