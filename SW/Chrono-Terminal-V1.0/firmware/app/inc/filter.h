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

/*!****************************************************************************
* User define
*/
#define SPS                     800UL
#define Trc                     0.005f
#define K                       (SPS*Trc)

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    uint16_t F;
    uint16_t H;
    uint16_t R;
    uint16_t Q;
    uint16_t X0;
    float P0;
    float Kgain;
    float Covariance;
    uint16_t State;
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
uint16_t Correct(uint16_t data);

#endif //filter_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
