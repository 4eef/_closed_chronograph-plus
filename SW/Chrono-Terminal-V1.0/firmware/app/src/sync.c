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
extern power_type           power;
extern menu_type            menu;

/*!****************************************************************************
* @brief    Syncronizer deinitialization function
* @param    
* @retval   
*/
void syncDeinit(void){
    RCC->APB2RSTR   |= RCC_APB2RSTR_TIM1RST;
    RCC->APB2ENR    &= ~RCC_APB2ENR_TIM1EN;
}

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
    TIM1->ARR       = CYC_PERIOD_US;
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
    uint8_t retVal, time;
    uint16_t tmp, diff;
    if(TIM1->CR1 & TIM_CR1_CEN){
        while(TIM1->CR1 & TIM_CR1_CEN) __NOP();
        retVal = 0;
    }else{
        retVal = 1;
    }
    power.uptimeCurr++;
    tmp = (power.uptimeSet*MIN_TO_US)/CYC_PERIOD_US;
    diff = tmp - power.uptimeCurr;
    if((diff < FIVE_SEC_WARNING) && (diff > 0)){
        time = 1 + (diff*CYC_PERIOD_US)/S_TO_US;
        sprintf(menu.message, "Shutdown in %u...", time);
        menu.msgCnt = MSG_FOR_TIMER;
    }else if((power.uptimeCurr > tmp) && (power.mode == POWER_RUN)) powerOff();
    else if((power.uptimeCurr > CYC_POFF_MAX) && (power.mode == POWER_STOP)) powerOff();
    TIM1->CR1       |= TIM_CR1_CEN;
    ADC1->CR        |= ADC_CR_ADSTART;
    return retVal;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
