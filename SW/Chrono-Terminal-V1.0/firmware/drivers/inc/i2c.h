/*!****************************************************************************
* @file    i2c.h
* @author  4eef
* @version V1.0
* @date    07.08.2016
* @brief   --
*/
#ifndef i2c_H
#define i2c_H

/*!****************************************************************************
* Include
*/
#include "stm32f0xx.h"
#include "clock.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*!****************************************************************************
* User define
*/
#define I2C_NBYTES_MAX          255
#define I2C_WRITE               0
#define I2C_READ                1
#define I2C_STAT_OK             0
#define I2C_STAT_BUSY           1
#define I2C_STAT_TC             2
#define I2C_STAT_NACK           3
#define I2C_STAT_STOP           4

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

/******************************************************************************
* Extern viriables
*/


/*!****************************************************************************
* Prototypes for the functions
*/
void I2CRxInit(uint8_t sadd, uint16_t len);
uint8_t I2CRx(uint8_t sadd, uint8_t *dst, uint16_t len);
uint8_t I2CRxDma(uint8_t sadd, uint8_t *dst, uint16_t len);
void I2CTxInit(uint8_t sadd, uint16_t len);
uint8_t I2CTx(uint8_t sadd, uint8_t *src, uint16_t len);
uint8_t I2CTxDma(uint8_t sadd, uint8_t *src, uint16_t len);
void initI2C1(void);

#endif //i2c_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
