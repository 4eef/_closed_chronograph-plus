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
extern menu_type            menu;
extern lpfPrim_type         lpfPrim;
extern kalman_type          kalman;
extern meas_type            meas;
lis3AxisCal_type            lis3AxisCal;
accel_type                  accel;

/*!****************************************************************************
* @brief    Initialize the LIS3DSH
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
    accel.gainX = ACCEL_MAX;
    accel.gainY = ACCEL_MAX;
    accel.gainZ = ACCEL_MAX;
}

/*!****************************************************************************
* @brief    Write to LIS3DSH function
* @param    
* @retval   
*/
void lis3_write(uint8_t reg, uint8_t com){
    uint8_t sadd = 0x3A, data[2] = {reg, com};
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CTx(sadd, &data[0], 2);
}

/*!****************************************************************************
* @brief    Read from LIS3DSH function
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
* @brief    Send data to GDDRAM
* @param    
* @retval   
*/
void lis3_getXYZ(void){
    uint8_t sadd = 0x3A, reg = 0x28;
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CTx(sadd, &reg, 1);
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CRx(sadd | 1, &accel.rawXL, 6);
}

/*!****************************************************************************
* @brief    Tilt angle calculation routine from Q16.16 fixed point format
* @param    X, Y, Z - values in radians (Q16.16)
* @retval   Angle in degrees (Q16.16)
*/
int32_t q16TiltCalc(int32_t X, int32_t Y, int32_t Z){
    int32_t tilt, Ysq, Zsq, sumSqYZ, div;
    //Prepare arguments
    Ysq = fix16_mul(Y, Y);
    Zsq = fix16_mul(Z, Z);
    sumSqYZ = fix16_add(Ysq, Zsq);
    //Divide-by-zero protection
    if(sumSqYZ == 0) sumSqYZ = 1;
    //Tilt calculation
    div = fix16_div(X, sumSqYZ);
    tilt = fix16_atan(div);
    return tilt;
}

/*!****************************************************************************
* @brief    Tilt angle calculation routine
* @param    X, Y, Z - normalized projections to axises
* @retval   Angle in degrees (float)
*/
float tiltAngCalc(float X, float Y, float Z){
    float tilt, Ysq, Zsq, sumSqYZ, div;
    //Prepare arguments
    Ysq = Y*Y;
    Zsq = Z*Z;
    sumSqYZ = Ysq + Zsq;
    //Divide-by-zero protection
    if(sumSqYZ == 0) sumSqYZ = 0.000000000000001;
    //Tilt calculation
    div = X/sumSqYZ;
    tilt = (atan(div))*degRad;
    return tilt;
}

/*!****************************************************************************
* @brief    Signed 16-bit variable normalizer
* @param    Signed 16-bit value
* @retval   Normalized (-1...1) float value
*/
float s16fNorm(int16_t val){
    return val/16383.;
}

/*!****************************************************************************
* @brief    Get tilt data from accelerometer readings
* @param    
* @retval   
*/
void trxAccData(void){
    uint8_t reg, numSamples = ACCEL_N_SAMPLES;
    int32_t X, Y, Z, tmpRoll, tmpPitch, tmpConv;
    //Settings
    lis3_write(0x10, accel.offsetX);                                            //Offsets
    lis3_write(0x11, accel.offsetY);
    lis3_write(0x12, accel.offsetZ);
    lis3_write(0x20, 0x87);                                                     //1600 Hz sample rate
    //Get the samples
    while(numSamples != 0){
        reg = lis3_read(0x27);                                                  //Check if data is ready
        if((reg & 0x3) != 0){
            lis3_getXYZ();                                                      //Get sampled data
            lis3AxisCal.calAxisX = accel.corrX = lpfAccPrim(&lpfPrim.x, ((int16_t)(accel.rawXL | (accel.rawXH << 8))));
            lis3AxisCal.calAxisY = accel.corrY = lpfAccPrim(&lpfPrim.y, ((int16_t)(accel.rawYL | (accel.rawYH << 8))));
            lis3AxisCal.calAxisZ = accel.corrZ = lpfAccPrim(&lpfPrim.z, ((int16_t)(accel.rawZL | (accel.rawZH << 8))));
            numSamples--;
        }
    }
    lis3_write(0x20, 0x00);                                                     //Off
    //Data filtering
    accel.corrX = kalmanAccCorr(&kalman.x, accel.corrX);
    accel.corrY = kalmanAccCorr(&kalman.y, accel.corrY);
    accel.corrZ = kalmanAccCorr(&kalman.z, accel.corrZ);
    //Axises scaling by measured gain
    X = (accel.corrX * fix16_one) / accel.gainX;
    Y = (accel.corrY * fix16_one) / accel.gainY;
    Z = (accel.corrZ * fix16_one) / accel.gainZ;
    //Clipping
    if(X >= fix16_one) X = fix16_one - 10; else if(X <= -fix16_one) X = -fix16_one + 10;
    if(Y >= fix16_one) Y = fix16_one - 10; else if(Y <= -fix16_one) Y = -fix16_one + 10;
    if(Z >= fix16_one) Z = fix16_one - 10; else if(Z <= -fix16_one) Z = -fix16_one + 10;
    //Linearization
//    X = fix16_asin(X);
//    Y = fix16_asin(Y);
//    Z = fix16_asin(Z);
//    X = fix16_sin(X);
//    Y = fix16_sin(Y);
//    Z = fix16_sin(Z);
    //Calculate values
    tmpConv = Q16_RAD_DEG;
    tmpRoll = q16TiltCalc(Y, X, Z);                                             //Roll calculation
    tmpRoll = fix16_mul(tmpConv, tmpRoll);
    meas.accRoll = ((tmpRoll) * 100) >> 16;
    tmpPitch = q16TiltCalc(Z, X, Y);                                            //Pitch calculation
    tmpPitch = fix16_mul(tmpConv, tmpPitch);
    meas.accPitch = ((tmpPitch) * 100) >> 16;
}

/*!****************************************************************************
* @brief    Axises calibration routine
* @param    
* @retval   
*/
void accGainCal(void){
    switch(lis3AxisCal.calState){
    case ACCEL_CAL_READY:
        menu.parEdit = PAR_EDIT_ENABLE;
        lis3AxisCal.calState = ACCEL_CAL_WAIT;
        lis3AxisCal.calAxisState = ACCEL_X_CAL_SAVE_NEG;
        break;
    case ACCEL_CAL_SAVE:
        switch(lis3AxisCal.calAxisState){
        case ACCEL_X_CAL_SAVE_NEG:
            lis3AxisCal.calTmpMinX = lis3AxisCal.calAxisX;
            lis3AxisCal.calState = ACCEL_CAL_WAIT;
            lis3AxisCal.calAxisState = ACCEL_X_CAL_SAVE_POS;
            break;
        case ACCEL_X_CAL_SAVE_POS:
            lis3AxisCal.calTmpMaxX = lis3AxisCal.calAxisX;
            lis3AxisCal.calState = ACCEL_CAL_WAIT;
            lis3AxisCal.calAxisState = ACCEL_Y_CAL_SAVE_NEG;
            break;
        case ACCEL_Y_CAL_SAVE_NEG:
            lis3AxisCal.calTmpMinY = lis3AxisCal.calAxisY;
            lis3AxisCal.calState = ACCEL_CAL_WAIT;
            lis3AxisCal.calAxisState = ACCEL_Y_CAL_SAVE_POS;
            break;
        case ACCEL_Y_CAL_SAVE_POS:
            lis3AxisCal.calTmpMaxY = lis3AxisCal.calAxisY;
            lis3AxisCal.calState = ACCEL_CAL_WAIT;
            lis3AxisCal.calAxisState = ACCEL_Z_CAL_SAVE_NEG;
            break;
        case ACCEL_Z_CAL_SAVE_NEG:
            lis3AxisCal.calTmpMinZ = lis3AxisCal.calAxisZ;
            lis3AxisCal.calState = ACCEL_CAL_WAIT;
            lis3AxisCal.calAxisState = ACCEL_Z_CAL_SAVE_POS;
            break;
        case ACCEL_Z_CAL_SAVE_POS:
            lis3AxisCal.calTmpMaxZ = lis3AxisCal.calAxisZ;
            lis3AxisCal.calState = ACCEL_CAL_DONE;
            break;
        default:
            break;
        }
        break;
    case ACCEL_CAL_DONE:
        //Save the result
        accel.gainX = (lis3AxisCal.calTmpMaxX-lis3AxisCal.calTmpMinX)/2;
        accel.gainY = (lis3AxisCal.calTmpMaxY-lis3AxisCal.calTmpMinY)/2;
        accel.gainZ = (lis3AxisCal.calTmpMaxZ-lis3AxisCal.calTmpMinZ)/2;
        menu.parEdit = PAR_EDIT_DISABLE;
        lis3AxisCal.calState = ACCEL_CAL_READY;
        break;
    case ACCEL_CAL_CANCEL:
        menu.parEdit = PAR_EDIT_DISABLE;
        lis3AxisCal.calState = ACCEL_CAL_READY;
        break;
    default:
        break;
    }
//    if(lis3AxisCal.calState == ACCEL_CAL_SAVE){
//        lis3AxisCal.calState = ACCEL_CAL_EN;
//        if(lis3AxisCal.calXState != ACCEL_CAL_SAVE){
//            if(lis3AxisCal.calXState == ACCEL_CAL_SAVE_HALF){
//                lis3AxisCal.calXState = ACCEL_CAL_SAVE;
//                lis3AxisCal.calTmpMinX = accel.corrX;
//            }else{
//                lis3AxisCal.calXState = ACCEL_CAL_SAVE_HALF;
//                lis3AxisCal.calTmpMaxX = accel.corrX;
//            }
//        }else if(lis3AxisCal.calYState != ACCEL_CAL_SAVE){
//            if(lis3AxisCal.calYState == ACCEL_CAL_SAVE_HALF){
//                lis3AxisCal.calYState = ACCEL_CAL_SAVE;
//                lis3AxisCal.calTmpMinY = accel.corrY;
//            }else{
//                lis3AxisCal.calYState = ACCEL_CAL_SAVE_HALF;
//                lis3AxisCal.calTmpMaxY = accel.corrY;
//            }
//        }else if(lis3AxisCal.calZState != ACCEL_CAL_SAVE){
//            if(lis3AxisCal.calZState == ACCEL_CAL_SAVE_HALF){
//                lis3AxisCal.calZState = ACCEL_CAL_SAVE;
//                menu.parEdit = PAR_EDIT_DISABLE;
//                lis3AxisCal.calXState = ACCEL_CAL_OK;
//                lis3AxisCal.calYState = ACCEL_CAL_OK;
//                lis3AxisCal.calZState = ACCEL_CAL_OK;
//                lis3AxisCal.calState = ACCEL_CAL_OK;
//                lis3AxisCal.calTmpMinZ = accel.corrZ;
//                //Gain calculation
//                lis3AxisCal.calGainX = (lis3AxisCal.calTmpMaxX-lis3AxisCal.calTmpMinX)/2;
//                lis3AxisCal.calGainY = (lis3AxisCal.calTmpMaxY-lis3AxisCal.calTmpMinY)/2;
//                lis3AxisCal.calGainZ = (lis3AxisCal.calTmpMaxZ-lis3AxisCal.calTmpMinZ)/2;
//                //Offset calculation
//                lis3AxisCal.calOffX = (lis3AxisCal.calTmpMaxX+lis3AxisCal.calTmpMinX)/2;
//                lis3AxisCal.calOffY = (lis3AxisCal.calTmpMaxY+lis3AxisCal.calTmpMinY)/2;
//                lis3AxisCal.calOffZ = (lis3AxisCal.calTmpMaxZ+lis3AxisCal.calTmpMinZ)/2;
//                //Protection
//                if((lis3AxisCal.calOffX < ACCEL_OFF_MIN) || (lis3AxisCal.calOffX > ACCEL_OFF_MAX) ||
//                   (lis3AxisCal.calOffY < ACCEL_OFF_MIN) || (lis3AxisCal.calOffY > ACCEL_OFF_MAX) ||
//                   (lis3AxisCal.calOffZ < ACCEL_OFF_MIN) || (lis3AxisCal.calOffZ > ACCEL_OFF_MAX)){
//                    strcpy(menu.message, "Offset off range");
//                    return;
//                }
//                accel.gainX = lis3AxisCal.calGainX;
//                accel.gainY = lis3AxisCal.calGainY;
//                accel.gainZ = lis3AxisCal.calGainZ;
//                accel.offsetX = lis3AxisCal.calOffX/32;
//                accel.offsetY = lis3AxisCal.calOffY/32;
//                accel.offsetZ = lis3AxisCal.calOffZ/32;
//            }else{
//                lis3AxisCal.calZState = ACCEL_CAL_SAVE_HALF;
//                lis3AxisCal.calTmpMaxZ = accel.corrZ;
//            }
//        }
//    }else if(lis3AxisCal.calState == ACCEL_CAL_CANCEL){
//        menu.parEdit = PAR_EDIT_DISABLE;
//        lis3AxisCal.calState = ACCEL_CAL_READY;
//    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
