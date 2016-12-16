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

/*!****************************************************************************
* User define
*/

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

/*!****************************************************************************
* Extern viriables
*/
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

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

#endif //lis3_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
