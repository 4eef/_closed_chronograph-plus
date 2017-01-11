/*!****************************************************************************
* @file    IR.c
* @author  4eef
* @version V1.0
* @date    05.01.2017, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "IR.h"

/*!****************************************************************************
* MEMORY
*/
IRRXData_type               IRRXData;

/*!****************************************************************************
* @brief    IR receiver initial configuration
* @param    
* @retval   
*/
void initIRConfig(void){
    RCC->APB1ENR    |= RCC_APB1ENR_TIM2EN;                      //Enable clock
    RCC->APB1RSTR   |= RCC_APB1RSTR_TIM2RST;                    //Reset peripheral
    RCC->APB1RSTR   &= ~RCC_APB1RSTR_TIM2RST;
    TIM2->PSC       = F_APB1/1000000+1;                         //Count in microseconds
    TIM2->ARR       = IR_MAX_TIM_US;                            //Max count value
    gppin_set(GP_IRR_VCC);                                      //Enable Vcc for IR receiver
    TIM2->CCMR1     |= (0x1 << TIM_CCMR1_CC1S_Pos);             //Input
    TIM2->CCMR1     |= (3 << TIM_CCMR1_IC1F_Pos);               //8 Samples
    TIM2->CCER      |= (TIM_CCER_CC1P | TIM_CCER_CC1P);         //Falling and rising edges
    TIM2->CCMR1     &= ~(0 << TIM_CCMR1_IC1PSC_Pos);            //No prescaler
    TIM2->CCER      |= TIM_CCER_CC1E;                           //Enable capturing
    TIM2->DIER      |= TIM_DIER_CC1IE;                          //Enable interrupts
    TIM2->DIER      |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 1);
}

/*!****************************************************************************
* @brief    Timer overflow interrupt
* @param    
* @retval   
*/
__irq void TIM2_IRQHandler(void){
    if((TIM2->SR & TIM_SR_CC1IF) != 0){
        TIM2->SR &= ~TIM_SR_CC1IF;
        if((GPIOA->IDR & GPIO_IDR_5) != 0){                     //Rising edge
            
        }else{                                                  //Falling edge
            if((TIM2->CR1 & TIM_CR1_CEN) != 0){
                TIM2->CR1 &= ~TIM_CR1_CEN;
                if((TIM2->CCR1 <= (IR_PREAMBLE_TIME+IR_PREAMBLE_TIME/10)) && (TIM2->CCR1 >= (IR_PREAMBLE_TIME-IR_PREAMBLE_TIME/10))){
                    
                }else
                if((TIM2->CCR1 <= (IR_LOG_ZERO_TIME+IR_LOG_ZERO_TIME/10)) && (TIM2->CCR1 >= (IR_LOG_ZERO_TIME-IR_LOG_ZERO_TIME/10))){
                    
                }else
                if((TIM2->CCR1 <= (IR_LOG_ONE_TIME+IR_LOG_ONE_TIME/10)) && (TIM2->CCR1 >= (IR_LOG_ONE_TIME-IR_LOG_ONE_TIME/10))){
                    
                }else{                                          //Error
                    
                }
            }
            TIM2->CR1 |= TIM_CR1_CEN;
        }
    }else
    if((TIM2->SR & TIM_SR_UIF) != 0){
        TIM2->SR &= ~TIM_SR_UIF;
        //TIM2->CR1 &= ~TIM_CR1_CEN;
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
