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
#define ACCEL_CAL_OK            0
#define ACCEL_CAL_EN            1
#define ACCEL_CAL_CANCEL        2
#define ACCEL_CAL_SAVE_HALF     3
#define ACCEL_CAL_SAVE          4
#define ACCEL_MAX               16383
#define ACCEL_MIN               -16384

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
    int16_t     corrX;
    int16_t     corrY;
    int16_t     corrZ;
    int16_t     rawX;
    int16_t     rawY;
    int16_t     rawZ;
    uint8_t     rawXL;
    uint8_t     rawXH;
    uint8_t     rawYL;
    uint8_t     rawYH;
    uint8_t     rawZL;
    uint8_t     rawZH;
}accel_type;

typedef struct{
    int16_t     calFiltX;
    int16_t     calFiltY;
    int16_t     calFiltZ;
    int16_t     calMinX;
    int16_t     calMaxX;
    int16_t     calMinY;
    int16_t     calMaxY;
    int16_t     calMinZ;
    int16_t     calMaxZ;
    int16_t     calOffX;
    int16_t     calOffY;
    int16_t     calOffZ;
    int16_t     calTmpMinX;
    int16_t     calTmpMaxX;
    int16_t     calTmpMinY;
    int16_t     calTmpMaxY;
    int16_t     calTmpMinZ;
    int16_t     calTmpMaxZ;
    uint8_t     calXState;
    uint8_t     calYState;
    uint8_t     calZState;
    uint8_t     calState;
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
void accAxisCal(void);

#endif //lis3_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
