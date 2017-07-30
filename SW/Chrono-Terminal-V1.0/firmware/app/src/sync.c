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
extern meas_type            meas;

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
void sync(void){
    char msg[20];
    uint8_t cntDn;
    uint16_t timeSet, timeDiff;
    if(TIM1->CR1 & TIM_CR1_CEN){
        while(TIM1->CR1 & TIM_CR1_CEN) __NOP();
    }else{
        meas.stats.cycBroken++;
    }
    power.uptimeCurr++;
    meas.stats.cycTotal++;
    timeSet = (power.uptimeSet*MIN_TO_US)/CYC_PERIOD_US;
    timeDiff = timeSet - power.uptimeCurr;
    if((timeDiff < FIVE_SEC_WARNING) && (timeDiff > 0)){
        cntDn = 1 + (timeDiff*CYC_PERIOD_US)/S_TO_US;
        sprintf(msg, "Shutdown in %u...", cntDn);
        Menu_putMessage(msg, MSG_FOR_TIMER);
    }else if((power.uptimeCurr > timeSet) && (power.mode == POWER_RUN)) powerOff();
    else if((power.uptimeCurr > CYC_POFF_MAX) && (power.mode == POWER_STOP)) powerOff();
    TIM1->CR1       |= TIM_CR1_CEN;
    ADC1->CR        |= ADC_CR_ADSTART;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
