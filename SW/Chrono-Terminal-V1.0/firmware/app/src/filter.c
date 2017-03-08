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
lpfPrim_type                lpfPrim;

/*!****************************************************************************
* @brief    Primary filter for accelerometer RAW data
* @param    
* @retval   
*/
int16_t lpfAccPrim(lpfAxs_type *axis, int16_t raw){
    axis->Dacc = axis->Dacc + raw - axis->Dout;
    axis->Dout = axis->Dacc/(int16_t)K_ACC;
    return axis->Dout;
}

/*!****************************************************************************
* @brief    Kalman filter for single accelerometer axis
* @param    
* @retval   Filtered accelerometer axis value
*/
int16_t kalmanAccCorr(kAxis_type *axis, int16_t raw){
    int32_t i, val[KALMAN_N_VALS], sum, acc, preVal;
    float sumNorm, F, gain, H, P;
    //Prediction stage
    F = axis->F;
    H = kalman.H;
    for(i = 0; i < KALMAN_N_VALS - 1; i++){                                     //Copy previous values
        val[i + 1] = axis->val[i];
    }
    preVal = (int16_t)(F * val[1]);
    axis->preVal = preVal;
    P = F * axis->covariance * F + kalman.Q;
    axis->P = P;
    //Update stage
    gain = H * P/(H * P * H + kalman.R);
    axis->gain = gain;
    val[0] = ((int16_t)(preVal + gain * (raw - H * preVal)));
    for(i = 0; i < KALMAN_N_VALS; i++){
        axis->val[i] = val[i];
    }
    axis->covariance = (1 - gain * H) * P;
    //Calculate value acceleration and speed
    acc = val[0] - 2 * val[1] + val[2];
    sum = axis->V + acc;
    axis->V = sum;
    sumNorm = (float)sum / ACC_MAX;
    if(sumNorm <= 1 && sumNorm >= -1) axis->F = 1 + sumNorm;
    if(isnan(axis->covariance)) axis->covariance = 0;                           //Protection
    return val[0];
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
    Dacc = Dacc + data - Dout;
    Dout = Dacc/(int16_t)K;
    return Dout;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
