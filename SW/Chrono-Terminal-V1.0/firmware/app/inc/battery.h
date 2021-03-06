/*!****************************************************************************
* @file    battery.h
* @author  4eef
* @version V1.0
* @date    25.02.2017, 4eef
* @brief   --
*/
#ifndef battery_H
#define battery_H

/*!****************************************************************************
* Include
*/
#include "adc.h"
#include "filter.h"
#include "system.h"
#include "string.h"
#include "power.h"
#include "gpio.h"
#include "ug2864.h"

/*!****************************************************************************
* User define
*/
#define BATT_CHG_ARR_PTS        11
#define BATT_CHG_VMPLY_COEFF    10
#define BATT_ADD_PERC_MIN       0
#define BATT_ADD_PERC_MAX       9
#define BATT_MSG_PERIOD         1200
#define BATT_PERC_HYST          3
#define BATT_PERC_FLAG_NONE     0
#define BATT_PERC_FLAG_INC      1
#define BATT_PERC_FLAG_DEC      2
#define BATT_VOLT_LOW           3400

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    uint8_t         battCharge;
    uint8_t         battChgStat;
    uint16_t        battVolt;
    uint16_t        battVltPts[BATT_CHG_ARR_PTS];
    uint16_t        battMsgPer;
}battery_type;

/*!****************************************************************************
* Extern viriables
*/
extern battery_type         battery;

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void battCalc(void);

#endif //battery_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
