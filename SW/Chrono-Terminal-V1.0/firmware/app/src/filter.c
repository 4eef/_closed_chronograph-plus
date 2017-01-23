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
int16_t lpfx(int16_t data){
    static int32_t Dacc = 0;
    static int16_t Dout = 0;
    int16_t Din = data;
    
    Dacc = Dacc + Din - Dout;
    Dout = Dacc/(int16_t)ACC_K;
    
    return Dout;
}
int16_t lpfy(int16_t data){
    static int32_t Dacc = 0;
    static int16_t Dout = 0;
    int16_t Din = data;
    
    Dacc = Dacc + Din - Dout;
    Dout = Dacc/(int16_t)ACC_K;
    
    return Dout;
}
int16_t lpfz(int16_t data){
    static int32_t Dacc = 0;
    static int16_t Dout = 0;
    int16_t Din = data;
    
    Dacc = Dacc + Din - Dout;
    Dout = Dacc/(int16_t)ACC_K;
    
    return Dout;
}

/*!****************************************************************************
* @brief   Simplified Kalman filter
* @param    
* @retval   
*/
uint16_t Correct(uint16_t data){
    //time update - prediction
    kalman.X0 = kalman.F*kalman.State;
    kalman.P0 = kalman.F*kalman.Covariance*kalman.F + kalman.Q;

    //measurement update - correction
    kalman.Kgain = kalman.H*kalman.P0/(kalman.H*kalman.P0*kalman.H + kalman.R);
    kalman.State = (uint16_t)(kalman.X0 + kalman.Kgain*(data - kalman.H*kalman.X0));
    kalman.Covariance = (1 - kalman.Kgain*kalman.H)*kalman.P0;
    return kalman.State;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
