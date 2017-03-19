/*!****************************************************************************
* @file    IR.h
* @author  4eef
* @version V1.0
* @date    05.01.2017, 4eef
* @brief   --
*/
#ifndef IR_H
#define IR_H

/*!****************************************************************************
* Include
*/
#include "stm32f0xx.h"
#include "clock.h"
#include "gpio.h"
#include "system.h"
#include "power.h"

/*!****************************************************************************
* User define
*/
#define F_APB1                  48000000
#define IR_MAX_TIM_US           300000
#define IR_PREAMBLE_TIME        6300
#define IR_STOPBIT_TIME         1100
#define IR_ACTIVE_TIME          550
#define IR_LOG_ZERO_TIME        2200
#define IR_LOG_ONE_TIME         1100
#define IR_READY                0
#define IR_BUSY                 1
#define IR_DATA_READY           2
#define IR_DATA_OVERRUN         3
#define IR_MAX_BITS             7
#define IR_MAX_BYTES            16
#define CHR_TOLERANCE           10
#define CHR_DIST_DEFAULT        200
#define CHR_DIST_MPLY           100000
#define CHR_TCK_NS              125
#define CHR_DIST_MIN            100
#define CHR_DIST_MAX            999
#define CHR_SPD_MAX             65535

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    uint8_t     rxBitCnt;
    uint8_t     rxBytesCnt;
    uint8_t     rxState;
    uint8_t     rxByte[IR_MAX_BYTES];
}IRRXData_type;

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void initIRConfig(void);
void deInitIR(void);
void hndlIRData(void);

#endif //IR_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
