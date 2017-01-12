/*!****************************************************************************
* @file    IR.c
* @author  4eef
* @version V1.0
* @date    05.01.2017, 4eef
* @brief   IR receiver with NEC IR protocol implemented
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
    TIM2->CNT = 0;                                              //Clear the timer
    TIM2->PSC       = F_APB1/1000000+1;                         //Count in microseconds
    TIM2->ARR       = IR_MAX_TIM_US;                            //Max count value
    gppin_set(GP_IRR_VCC);                                      //Enable Vcc for IR receiver
    TIM2->CCMR1     |= (0x1 << TIM_CCMR1_CC1S_Pos);             //Input
    TIM2->CCMR1     |= (3 << TIM_CCMR1_IC1F_Pos);               //8 Samples
    TIM2->CCER      |= (TIM_CCER_CC1P | TIM_CCER_CC1NP);        //Falling and rising edges
    TIM2->CCMR1     &= ~(0 << TIM_CCMR1_IC1PSC_Pos);            //No prescaler
    TIM2->CCER      |= TIM_CCER_CC1E;                           //Enable capturing
    TIM2->DIER      |= TIM_DIER_CC1IE;                          //Enable interrupts
    TIM2->DIER      |= TIM_DIER_UIE;
    TIM2->SR        &= ~TIM_SR_CC1OF;                           //Clear flags
    TIM2->SR        &= ~TIM_SR_CC1IF;
    TIM2->SR        &= ~TIM_SR_UIF;
    IRRXData.rxBitCnt = 0;
    IRRXData.rxBytesCnt = 0;
    IRRXData.rxState = IR_READY;
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 1);
    TIM2->EGR       |= TIM_EGR_UG;                              //Debug
    TIM2->EGR       |= TIM_EGR_CC1G;
}

/*!****************************************************************************
* @brief    IR receiver deinitializer
* @param    
* @retval   
*/
void deInitIR(void){
    RCC->APB1ENR    &= ~RCC_APB1ENR_TIM2EN;                     //Disable clock
    gppin_reset(GP_IRR_VCC);                                    //Disable Vcc for IR receiver
    TIM2->CCER      &= ~TIM_CCER_CC1E;                          //Disable capturing
    TIM2->DIER      &= ~TIM_DIER_CC1IE;                         //Disable interrupts
    TIM2->DIER      &= ~TIM_DIER_UIE;
    IRRXData.rxState = IR_BUSY;                                 //Set receiver busy
    NVIC_DisableIRQ(TIM2_IRQn);
}

/*!****************************************************************************
* @brief    Timer interrupt handler
* @param    
* @retval   
*/
__irq void TIM2_IRQHandler(void){
    if((TIM2->SR & TIM_SR_CC1OF) != 0){
        TIM2->SR &= ~TIM_SR_CC1OF;                              //RX failed, set receiver
        TIM2->SR &= ~TIM_SR_CC1IF;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        IRRXData.rxBitCnt = 0;
        IRRXData.rxBytesCnt = 0;
        IRRXData.rxState = IR_READY;
    }else if((TIM2->SR & TIM_SR_CC1IF) != 0){
        TIM2->SR &= ~TIM_SR_CC1IF;
        if((GPIOA->IDR & GPIO_IDR_5) != 0){                     //Rising edge
            if((TIM2->CCR1 <= (IR_STOPBIT_TIME+IR_STOPBIT_TIME/10)) && (TIM2->CCR1 >= (IR_STOPBIT_TIME-IR_STOPBIT_TIME/10))){
                TIM2->CR1 &= ~TIM_CR1_CEN;
                IRRXData.rxBitCnt = 0;
                //Security checks
                if((IRRXData.rxBytesCnt == IR_MAX_BYTES) && (IRRXData.rxState != IR_DATA_OVERRUN)){
                    IRRXData.rxState = IR_DATA_READY;
                }else{
                    IRRXData.rxBytesCnt = 0;
                    IRRXData.rxState = IR_READY;
                }
            }
            TIM2->CCR1 = 0;
        }else{                                                  //Falling edge
            if((TIM2->CR1 & TIM_CR1_CEN) != 0){
                TIM2->CR1 &= ~TIM_CR1_CEN;
                if((TIM2->CCR1 <= (IR_PREAMBLE_TIME+IR_PREAMBLE_TIME/5)) && (TIM2->CCR1 >= (IR_PREAMBLE_TIME-IR_PREAMBLE_TIME/5))){
                    IRRXData.rxBitCnt = 0;
                    IRRXData.rxBytesCnt = 0;
                    IRRXData.rxState = IR_BUSY;
                }else if((TIM2->CCR1 <= (IR_LOG_ZERO_TIME+IR_LOG_ZERO_TIME/10)) && (TIM2->CCR1 >= (IR_LOG_ZERO_TIME-IR_LOG_ZERO_TIME/10)) && (IRRXData.rxState == IR_BUSY)){
                    IRRXData.rxByte[IRRXData.rxBytesCnt] &= ~(1 << IRRXData.rxBitCnt);
                    IRRXData.rxBitCnt++;
                    if(IRRXData.rxBitCnt > IR_MAX_BITS){
                        IRRXData.rxBitCnt = 0;
                        IRRXData.rxBytesCnt++;
                        if(IRRXData.rxBytesCnt > IR_MAX_BYTES){ //Ring the buffer with overrun flag
                            IRRXData.rxState = IR_DATA_OVERRUN;
                            IRRXData.rxBytesCnt = 0;
                        }
                    }
                }else if((TIM2->CCR1 <= (IR_LOG_ONE_TIME+IR_LOG_ONE_TIME/10)) && (TIM2->CCR1 >= (IR_LOG_ONE_TIME-IR_LOG_ONE_TIME/10)) && (IRRXData.rxState == IR_BUSY)){
                    IRRXData.rxByte[IRRXData.rxBytesCnt] |= (1 << IRRXData.rxBitCnt);
                    IRRXData.rxBitCnt++;
                    if(IRRXData.rxBitCnt > IR_MAX_BITS){
                        IRRXData.rxBitCnt = 0;
                        IRRXData.rxBytesCnt++;
                        if(IRRXData.rxBytesCnt > IR_MAX_BYTES){
                            IRRXData.rxState = IR_DATA_OVERRUN;
                            IRRXData.rxBytesCnt = 0;
                        }
                    }
                }else{                                          //Error
                    TIM2->CR1 &= ~TIM_CR1_CEN;
                    IRRXData.rxBitCnt = 0;
                    IRRXData.rxBytesCnt = 0;
                    IRRXData.rxState = IR_READY;
                    return;
                }
            }
            TIM2->CCR1 = 0;
            TIM2->CNT = 0;
            TIM2->CR1 |= TIM_CR1_CEN;
        }
    }else if((TIM2->SR & TIM_SR_UIF) != 0){
        TIM2->SR &= ~TIM_SR_UIF;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        IRRXData.rxBitCnt = 0;
        IRRXData.rxBytesCnt = 0;
        IRRXData.rxState = IR_READY;
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
