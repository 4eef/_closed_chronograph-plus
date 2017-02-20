/*!****************************************************************************
* @file    adc.c
* @author  4eef
* @version V1.0
* @date    17.01.2017, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "adc.h"

/*!****************************************************************************
* MEMORY
*/
adcData_type                adcData;

/*!****************************************************************************
* @brief    ADC deinitialization routine
* @param    
* @retval   
*/
void adcDeinit(void){
    RCC->APB2RSTR       |= RCC_APB2RSTR_ADCRST;
    RCC->APB2ENR        &= ~RCC_APB2ENR_ADCEN;
    NVIC_DisableIRQ(ADC1_IRQn);
}

/*!****************************************************************************
* @brief    ADC initialization routine
* @param    
* @retval   
*/
void adcInit(void){
    RCC->APB2ENR        |= RCC_APB2ENR_ADCEN;
    RCC->APB2RSTR       |= RCC_APB2RSTR_ADCRST;
    RCC->APB2RSTR       &= ~RCC_APB2RSTR_ADCRST;
    ADC1->CR            |= ADC_CR_ADCAL;
    while(ADC1->CR & ADC_CR_ADCAL) __NOP();
    ADC1->CR            |= ADC_CR_ADEN;
    while(ADC1->ISR & ADC_ISR_ADRDY == 0) __NOP();
    ADC1->CHSELR        |= ADC_CHSELR_CHSEL4;
    ADC1->IER           |= (ADC_IER_OVRIE | ADC_IER_EOSEQIE);
    NVIC_EnableIRQ(ADC1_IRQn);
    NVIC_SetPriority(ADC1_IRQn, 14);
}

/*!****************************************************************************
* @brief    ADC interrupt handler
* @param    
* @retval   
*/
__irq void ADC1_IRQHandler(void){
    if((ADC1->CR & ADC_CR_ADSTART) == 0){
        if((ADC1->ISR & ADC_ISR_EOS) != 0){
            ADC1->ISR |= ADC_ISR_EOS;
            adcData.adcRawData = ADC1->DR;
            adcData.adcStat = ADC_DATA_READY;
        }else if((ADC1->ISR & ADC_ISR_OVR) != 0){
            ADC1->ISR |= ADC_ISR_OVR;
            adcData.adcStat = ADC_READY;
        }
    }else{
        ADC1->CR |= ADC_CR_ADSTP;
        while(ADC1->CR & ADC_CR_ADSTP) __NOP();
        ADC1->ISR |= ADC_ISR_EOS;
        ADC1->ISR |= ADC_ISR_OVR;
        adcData.adcStat = ADC_READY;
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
