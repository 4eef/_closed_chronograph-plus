/*!****************************************************************************
* @file    lis3.c
* @author  4eef
* @version V1.0
* @date    30.08.2016
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "lis3.h"

/*!****************************************************************************
* MEMORY
*/
accel_type accel;

/*!****************************************************************************
* @brief   Initialize the LIS3DSH
* @param    
* @retval   
*/
void lis3init(void){
    lis3_write(0x23, 0x01);                         //Soft reset
    delay_us(10000);
    //Configuration
    lis3_write(0x23, 0x00);                         //All is disabled
    lis3_write(0x24, 0x00);                         //AAF BW 800Hz, 2G, no self-test
    lis3_write(0x25, 0x10);                         //Enable address increment
    lis3_write(0x10, 0x00);                         //Output(X) = Measurement(X) - OFFSET(X) * 32;
    lis3_write(0x11, 0x00);
    lis3_write(0x12, 0x00);
}

/*!****************************************************************************
* @brief   Write to LIS3DSH function
* @param    
* @retval   
*/
void lis3_write(uint8_t reg, uint8_t com){
    uint8_t sadd = 0x3A, data[] = {reg, com};
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    HAL_I2C_Master_Transmit(&hi2c1, sadd, data, 2, 1000);
}

/*!****************************************************************************
* @brief   Read from LIS3DSH function
* @param    
* @retval   
*/
uint8_t lis3_read(uint8_t reg){
    uint8_t sadd = 0x3A, rdata;
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    HAL_I2C_Master_Transmit(&hi2c1, sadd, &reg, 1, 1000);
    
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    HAL_I2C_Master_Receive(&hi2c1, (sadd | 1), &rdata, 1, 1000);
    
    return rdata;
}

/*!****************************************************************************
* @brief   Send data to GDDRAM
* @param    
* @retval   
*/
void lis3_getXYZ(void){
    uint8_t sadd = 0x3A, reg = 0x28;
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    HAL_I2C_Master_Transmit(&hi2c1, sadd, &reg, 1, 1000);
    
    HAL_I2C_Master_Receive(&hi2c1, (sadd | 1), &accel.rawXL, 6, 1000);
    
    accel.rawX = (int16_t)(accel.rawXL | (accel.rawXH<<8));
    accel.rawY = (int16_t)(accel.rawYL | (accel.rawYH<<8));
    accel.rawZ = (int16_t)(accel.rawZL | (accel.rawZH<<8));
}
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
