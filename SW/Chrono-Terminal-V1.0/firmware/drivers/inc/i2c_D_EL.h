/**
*******************************************************************************
* @file    i2c.h 
* @author  Storozhenko Roman - D_EL
* @version V1.1
* @date    1-06-2015
* @brief
*/
#ifndef i2c_H
#define i2c_H

#include "stdint.h"
#include "stdio.h"
//#include "globalIncludes.h"

/******************************************************************************
* User define
*/
//I2C1
#define     I2C1_USE                    1
#define     I2C1_Tx_HOOK                0
#define     I2C1_Rx_HOOK                0
#define     I2C1_TxBffSz                64
#define     I2C1_RxBffSz                32
#define     I2C1_EventInterruptPrior    15
#define     I2C1_ErrorInterruptPrior    15
#define     I2C1_RxDmaInterruptPrior    15
#define     I2C1_ClockFreg              400000  //[Hz]

//I2C2
#define     I2C2_USE                    0
#define     I2C2_Tx_HOOK                0
#define     I2C2_Rx_HOOK                0
#define     I2C2_TxBffSz                64
#define     I2C2_RxBffSz                32
#define     I2C2_EventInterruptPrior    15
#define     I2C2_ErrorInterruptPrior    15
#define     I2C2_RxDmaInterruptPrior    15
#define     I2C2_ClockFreg              400000  //[Hz]

//I2C3
#define     I2C3_USE                    0
#define     I2C3_Tx_HOOK                0
#define     I2C3_Rx_HOOK                0
#define     I2C3_TxBffSz                64
#define     I2C3_RxBffSz                32
#define     I2C3_EventInterruptPrior    15
#define     I2C3_ErrorInterruptPrior    15
#define     I2C3_RxDmaInterruptPrior    15
#define     I2C3_ClockFreg              400000  //[Hz]

/******************************************************************************
* User typedef
*/
typedef enum{
    i2cUnknownState,    
    i2cFree,
    i2cTxRun,
    i2cRxRun,
    i2cDmaRefresh,
    i2cTxSuccess,
    i2cRxSuccess,
    
    i2cErrTx,
    i2cErrRx,
    
    i2cOk,
    i2cBusError,
    i2cAcknowledgeFailure,
    i2cPECError,
    i2cTimeout,
    i2cSoftTimeout
}i2cState_type;

typedef struct{   
    uint32_t            clockSpeed;     //[Hz]
    I2C_TypeDef         *I2c;
    DMA_Stream_TypeDef  *pDmaStreamRx;
    DMA_Stream_TypeDef  *pDmaStreamTx;
    uint8_t             slaveAdr;
    i2cState_type       state;
}i2c_type;

/******************************************************************************
* User enum
*/

/******************************************************************************
* Extern viriables
*/
/*
* i2c1 memory
*/
#if (I2C1_USE == 1)
extern i2c_type        i2c1Sct;
extern i2c_type        *i2c1;
extern uint8_t         i2c1TxBff[I2C1_TxBffSz];
extern uint8_t         i2c1RxBff[I2C1_RxBffSz];
#endif //UART1_USE

/******************************************************************************
* Macro functions
*/

/******************************************************************************
* Prototypes for the functions
*/
void i2c_init(i2c_type *i2cx);	                                                //Инициализируем I2C1
void i2c_reInit(i2c_type *i2cx);
void i2c_write(i2c_type *i2cx, void *src, uint16_t len, uint8_t slaveAdr);	    //Записываем данные из буфера
void i2c_read(i2c_type *i2cx, void *dst, uint16_t len, uint8_t slaveAdr);       //Читаем данные в буфер

#endif //i2c_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
