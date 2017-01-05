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
TIM_HandleTypeDef   htim2;

/*!****************************************************************************
* @brief    IR receiver initial configuration
* @param    
* @retval   
*/
void initIRConfig(void){
//    RCC->APB1ENR    |= RCC_APB1ENR_TIM2EN;
//    RCC->APB1RSTR   |= RCC_APB1RSTR_TIM2RST;                    //Reset peripheral
//    RCC->APB1RSTR   &= ~RCC_APB1RSTR_TIM2RST;
//    TIM2->PSC       = F_APB1/1000000+1;                         //Count in microseconds
//    TIM2->ARR       = IR_MAX_TIM_US;
//    TIM2->CCMR1     |= (0x1 << TIM_CCMR1_CC1S_Pos);             //Input
//    TIM2->CCMR1     &= ~(0 << TIM_CCMR1_IC1F_Pos);
////    TIM2->CCER      |= (TIM_CCER_CC1NP | TIM_CCER_CC1P);        //Rising and falling edges
//    TIM2->CCMR1     &= ~(0 << TIM_CCMR1_IC1PSC_Pos);
//    TIM2->CCER      |= TIM_CCER_CC1E;                           //Enable capturing
//    TIM2->DIER      |= TIM_DIER_CC1IE;                          //Enable interrupt
//    TIM2->DIER      |= TIM_DIER_UIE;
////    TIM2->CCMR1     |= (0x1 << TIM_CCMR2_CC4S_Pos);             //Input
////    TIM2->CCMR1     &= ~(0 << TIM_CCMR2_IC4F_Pos);
//////    TIM2->CCER      |= (TIM_CCER_CC4NP | TIM_CCER_CC4P);        //Rising and falling edges
////    TIM2->CCMR1     &= ~(0 << TIM_CCMR2_IC4PSC_Pos);
////    TIM2->CCER      |= TIM_CCER_CC4E;                           //Enable capturing
////    TIM2->DIER      |= TIM_DIER_CC4IE;                          //Enable interrupt
////    TIM2->DIER      |= TIM_DIER_UIE;
//    NVIC_EnableIRQ(TIM2_IRQn);
//    NVIC_SetPriority(TIM2_IRQn, 5);
//    //Initialize updates generation
//    //TIM2->EGR       |= TIM_EGR_CC1G;
//    //TIM2->EGR       |= TIM_EGR_UG;
    MX_TIM2_Init();
}

/* TIM2 init function */
static void MX_TIM2_Init(void){
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_IC_InitTypeDef sConfigIC;

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 48;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 300000;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&htim2) != HAL_OK){
        __NOP();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK){
        __NOP();
    }

    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK){
        __NOP();
    }
}

/*!****************************************************************************
* @brief    Receive info via IR interface (NEC IR protocol with 40 kHz clock used)
* @param    
* @retval   
*/
__irq void TIM2_IRQHandler(void){
    if((TIM2->SR & TIM_SR_CC1IF) != 0){
        TIM2->SR        &= ~TIM_SR_CC1IF;
    }else
//    if((TIM2->SR & TIM_SR_CC4IF) != 0){
//        TIM2->SR        &= ~TIM_SR_CC4IF;
//    }else
    if((TIM2->SR & TIM_SR_UIF) != 0){
        TIM2->SR        &= ~TIM_SR_UIF;
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
