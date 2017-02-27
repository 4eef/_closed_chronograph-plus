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
#define SPS_ACC                 1600UL
#define Trc_ACC                 0.03f
#define K_ACC                   (SPS_ACC*Trc_ACC)
//#define K_ACC                   SAMPLE_RATE / (6.283185307*10)                  //K=FN/(pi*Fs) - K>1. 
//#define R_ACC                   1 + K_ACC * 0.707                               //R=K*Q+1 - Добротность элементов фильтра НЧ 
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
//    double      L1;
//    double      C1;
//    double      L2;
//    double      C2;
//    double      L3;
//    double      C3;
//    double      L4;
//    double      C4;
//    double      L5;
//    double      C5;
//    double      L;
//    double      C;
    int32_t     Dacc;
    int16_t     Dout;
}lpfAxs_type;

typedef struct{
    lpfAxs_type x;
    lpfAxs_type y;
    lpfAxs_type z;
}lpfPrim_type;

typedef struct{
    int16_t     val[KALMAN_N_VALS];
    int16_t     preVal;
    int16_t     V;
    int16_t     a;
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
