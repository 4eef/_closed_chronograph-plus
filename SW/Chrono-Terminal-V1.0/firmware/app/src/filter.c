/*!****************************************************************************
* @file    filter.c
* @author  4eef
* @version V1.0
* @date    05.01.2017, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "filter.h"

/*!****************************************************************************
* MEMORY
*/
kalman_type                 kalman;

/*!****************************************************************************
* @brief    Kalman filter for single accelerometer axis
* @param    
* @retval   Accelerometer axis value
*/
int16_t kalmanAccCorr(kAxis_type *axis, int16_t raw){
    int16_t val;
    double spd;
    //Prediction stage
    axis->preVal = (int16_t)(axis->F*axis->val);
    axis->P = axis->F*axis->covariance*axis->F + kalman.Q;
    //Update stage
    axis->gain = kalman.H*axis->P/(kalman.H*axis->P*kalman.H + kalman.R);
    val = ((int16_t)(axis->preVal + axis->gain*(raw - kalman.H*axis->preVal)));
    axis->covariance = (1 - axis->gain*kalman.H)*axis->P;
    //Calculate value changing speed
    spd = ((double)(val - axis->val))/ACC_MAX;
    if(spd <= 1 && spd >= -1) axis->F = 1 + asin(spd)/RADIAN;
    axis->val = val;
    if(isnan(axis->covariance)) axis->covariance = 0;//Reset
    return val;
}

/*!****************************************************************************
* @brief    Square root from uint32 calculator
* @param    
* @retval   Square root value
*/
uint32_t isqrt32(uint32_t n){
    register uint32_t root, remainder, place;
    root = 0;
    remainder = n;
    place = 0x40000000;
    while (place > remainder)
        place = place >> 2;
    while (place){
        if (remainder >= root + place){
            remainder = remainder - root - place;
            root = root + (place << 1);
        }
        root = root >> 1;
        place = place >> 2;
    }
    return root;
}

/*!****************************************************************************
* @brief    Damp the value with LPF
* @param    
* @retval   
*/
int16_t lpf(int16_t data){
    static int32_t Dacc = 0;
    static int16_t Dout = 0;
    int16_t Din = data;
    Dacc = Dacc + Din - Dout;
    Dout = Dacc/(int16_t)K;
    return Dout;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
