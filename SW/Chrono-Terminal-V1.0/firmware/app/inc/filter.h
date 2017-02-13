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

/*!****************************************************************************
* User define
*/
#define SPS                     20UL
#define Trc                     5.0f
#define K                       (SPS*Trc)
#define ACC_MAX                 16383
#define RADIAN                  1.57079633

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct {
    int32_t     val;
    int32_t     preVal;
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
int16_t lpfx(int16_t data);
int16_t lpfy(int16_t data);
int16_t lpfz(int16_t data);
int16_t lpf(int16_t data);
uint32_t isqrt32(uint32_t n);
uint16_t vectCalc(int16_t x, int16_t y, int16_t z);
int16_t kalmanAccCorr(kAxis_type *axis, int16_t raw);

#endif //filter_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
