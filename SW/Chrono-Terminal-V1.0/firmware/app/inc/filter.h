/*!****************************************************************************
* @file    filter.h
* @author  4eef
* @version V1.0
* @date    05.01.2017, 4eef
* @brief   --
*/
#ifndef filter_H
#define filter_H

/*!****************************************************************************
* Include
*/
#include "stm32f0xx.h"
#include "math.h"
#include "string.h"

/*!****************************************************************************
* User define
*/
#define SPS                     20UL
#define Trc                     5.0f
#define K                       (SPS*Trc)
#define SAMPLE_RATE             1600
#define K_ACC                   SAMPLE_RATE / (6.283185307*10)                  //K=FN/(pi*Fs) - Резонансная частота элементов фильтров K>1. 
#define R_ACC                   1 + K_ACC * 0.707                               //R=K*Q+1 - Добротность элементов фильтра НЧ 
#define ACC_MAX                 16383
#define RADIAN                  1.57079633
#define KALMAN_N_VALS           3

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    int32_t     L1;
    int32_t     C1;
    int32_t     L2;
    int32_t     C2;
    int32_t     L3;
    int32_t     C3;
    int32_t     L4;
    int32_t     C4;
    int32_t     L5;
    int32_t     C5;
}lpfAxs_type;

typedef struct{
    lpfAxs_type x;
    lpfAxs_type y;
    lpfAxs_type z;
}lpfPrim_type;

typedef struct{
    int32_t     val[KALMAN_N_VALS];
    int32_t     preVal;
    int32_t     V;
    int32_t     a;
    double      F;
    double      gain;
    double      covariance;
    double      P;
}kAxis_type;

typedef struct{
    int16_t     R;
    int16_t     Q;
    int16_t     H;
    kAxis_type  x;
    kAxis_type  y;
    kAxis_type  z;
}kalman_type;

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
int16_t lpf(int16_t data);
uint32_t isqrt32(uint32_t n);
int16_t kalmanAccCorr(kAxis_type *axis, int16_t raw);
int16_t lpfAccPrim(lpfAxs_type *axis, int16_t raw);

#endif //filter_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
