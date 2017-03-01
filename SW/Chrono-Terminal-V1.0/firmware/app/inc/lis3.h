/*!****************************************************************************
* @file    lis3.h
* @author  4eef
* @version V1.0
* @date    30.08.2016
* @brief   --
*/
#ifndef lis3_H
#define lis3_H

/*!****************************************************************************
* Include
*/
#include "stm32f0xx.h"
#include "i2c.h"
#include "delay.h"
#include "MicroMenu.h"

/*!****************************************************************************
* User define
*/
#define ACCEL_CAL_READY         0
#define ACCEL_CAL_SAVE          1
#define ACCEL_CAL_CANCEL        2
#define ACCEL_CAL_DONE          3
#define ACCEL_CAL_WAIT          4
#define ACCEL_X_CAL_SAVE_NEG    5
#define ACCEL_X_CAL_SAVE_POS    6
#define ACCEL_Y_CAL_SAVE_NEG    7
#define ACCEL_Y_CAL_SAVE_POS    8
#define ACCEL_Z_CAL_SAVE_NEG    9
#define ACCEL_Z_CAL_SAVE_POS    10
#define ACCEL_MAX               16383
#define ACCEL_OFF_MIN           -4096
#define ACCEL_OFF_MAX           4095
#define ACCEL_CAL_SAMPLES       25
#define ACCEL_N_SAMPLES         10

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    int8_t      offsetX;
    int8_t      offsetY;
    int8_t      offsetZ;
    int16_t     gainX;
    int16_t     gainY;
    int16_t     gainZ;
    int16_t     corrX;
    int16_t     corrY;
    int16_t     corrZ;
    uint8_t     rawXL;
    uint8_t     rawXH;
    uint8_t     rawYL;
    uint8_t     rawYH;
    uint8_t     rawZL;
    uint8_t     rawZH;
}accel_type;

typedef struct{
    int16_t     calAxisX;
    int16_t     calAxisY;
    int16_t     calAxisZ;
    int16_t     calTmpMinX;
    int16_t     calTmpMaxX;
    int16_t     calTmpMinY;
    int16_t     calTmpMaxY;
    int16_t     calTmpMinZ;
    int16_t     calTmpMaxZ;
    uint8_t     calState;
    uint8_t     calAxisState;
}lis3AxisCal_type;

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void lis3init(void);
void lis3_write(uint8_t reg, uint8_t data);
uint8_t lis3_read(uint8_t reg);
void lis3_getXYZ(void);
void accGainCal(void);

#endif //lis3_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
