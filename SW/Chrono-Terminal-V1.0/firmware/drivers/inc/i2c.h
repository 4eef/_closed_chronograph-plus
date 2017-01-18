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

/*!****************************************************************************
* User define
*/

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
void I2C_read(uint8_t sadd, uint8_t rgadd, uint8_t *dst, uint8_t len);
void I2C_read_DMA(uint8_t sadd, uint8_t rgadd, uint8_t *dst, uint8_t len);
void I2C_write(uint8_t sadd, uint8_t *src, uint8_t len);
void initI2C1(void);
void MX_DMA_Init(void);
void MX_I2C1_Init(void);

#endif //i2c_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
