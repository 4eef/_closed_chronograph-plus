/*!****************************************************************************
* @file    adc.h
* @author  4eef
* @version V1.0
* @date    17.01.2017, 4eef
* @brief   --
*/
#ifndef adc_H
#define adc_H

/*!****************************************************************************
* Include
*/
#include "stm32f0xx.h"

/*!****************************************************************************
* User define
*/
#define ADC_N_TO_MV                         1026
#define ADC_READY                           0
#define ADC_DATA_READY                      1
#define ADC_OVERUN                          2

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    uint8_t     adcStat;
    uint16_t    adcRawData;
}adcData_type;

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void adcInit(void);
void startConv(void);
void adcDataConv(void);

#endif //adc_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
