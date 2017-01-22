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
lis3AxisCal_type        lis3AxisCal;
accel_type              accel;

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
}

/*!****************************************************************************
* @brief   Write to LIS3DSH function
* @param    
* @retval   
*/
void lis3_write(uint8_t reg, uint8_t com){
    uint8_t sadd = 0x3A, data[2] = {reg, com};
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CTx(sadd, &data[0], 2);
}

/*!****************************************************************************
* @brief   Read from LIS3DSH function
* @param    
* @retval   
*/
uint8_t lis3_read(uint8_t reg){
    uint8_t sadd = 0x3A, rdata;
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CTx(sadd, &reg, 1);
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CRx(sadd | 1, &rdata, 1);
    return rdata;
}

/*!****************************************************************************
* @brief   Send data to GDDRAM
* @param    
* @retval   
*/
void lis3_getXYZ(void){
    uint8_t sadd = 0x3A, reg = 0x28;
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CTx(sadd, &reg, 1);
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CRx(sadd | 1, &accel.rawXL, 6);
    accel.rawX |= (int16_t)(accel.rawXL | (accel.rawXH<<8));
    accel.rawY |= (int16_t)(accel.rawYL | (accel.rawYH<<8));
    accel.rawZ |= (int16_t)(accel.rawZL | (accel.rawZH<<8));
}
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
