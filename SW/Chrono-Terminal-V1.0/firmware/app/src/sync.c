/*!****************************************************************************
* @file    sync.c
* @author  4eef
* @version V1.0
* @date    17.01.2017, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "sync.h"

/*!****************************************************************************
* MEMORY
*/

/*!****************************************************************************
* @brief    Initialization function for syncronizer
* @param    
* @retval   
*/
void syncInit(void){
    RCC->APB2ENR    |= RCC_APB2ENR_TIM1EN;
    RCC->APB2RSTR   |= RCC_APB2RSTR_TIM1RST;
    RCC->APB2RSTR   &= ~RCC_APB2RSTR_TIM1RST;
    TIM1->PSC       = F_APB1/1000000+1;
    TIM1->ARR       = CYC_PERIOD;
	TIM1->EGR       |= TIM_EGR_UG;
	TIM1->CR1       |= TIM_CR1_OPM;
	TIM1->CR1       |= TIM_CR1_CEN;
}

/*!****************************************************************************
* @brief    Syncronizer function
* @param    
* @retval   Returns 1 if cycle is broken; 0 if everything is ok
*/
uint8_t sync(void){
    uint8_t retVal;
    if(TIM1->CR1 & TIM_CR1_CEN){
        while(TIM1->CR1 & TIM_CR1_CEN) __NOP();
        retVal = 0;
    }else{
        retVal = 1;
    }
    TIM1->CR1       |= TIM_CR1_CEN;
    ADC1->CR        |= ADC_CR_ADSTART;
    return retVal;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
