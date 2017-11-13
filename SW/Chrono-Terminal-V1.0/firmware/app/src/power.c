/*!****************************************************************************
* @file    power.c
* @author  4eef
* @version V1.0
* @date    17.02.2017, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "power.h"

/*!****************************************************************************
* MEMORY
*/
extern buttons_type             buttons;
extern menu_type                menu;
power_type                      power;

/*!****************************************************************************
* @brief    Cancel button pressed during Stop mode
* @param    
* @retval   
*/
__irq void EXTI4_15_IRQHandler(void){
    if(EXTI->PR & EXTI_PR_PR7){
        EXTI->PR |= EXTI_PR_PR7;
        initClocks();
        NVIC_DisableIRQ(EXTI4_15_IRQn);
    }
}

/*!****************************************************************************
* @brief    Stop mode setting routine
* @param    
* @retval   
*/
void stopModeConfig(void){
    //Stop mode enable & config
    RCC->APB1ENR        |= RCC_APB1ENR_PWREN;
    SCB->SCR            |= SCB_SCR_SLEEPDEEP_Msk;
    PWR->CR             &= ~PWR_CR_PDDS;
    PWR->CR             |= PWR_CR_LPDS;
    //EXTI config
    SYSCFG->EXTICR[1]   |= SYSCFG_EXTICR2_EXTI7_PA;
    EXTI->IMR           |= EXTI_IMR_IM7;
    EXTI->FTSR          |= EXTI_FTSR_FT7;
    EXTI->RTSR          &= ~EXTI_RTSR_RT7;
    EXTI->PR            |= EXTI_PR_PR7;
	NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 15);
}

/*!****************************************************************************
* @brief    Power off sequence
* @param    
* @retval   
*/
void powerOff(void){
    power.pwrMode = ePwrStop;
    power.uptimeCurr = 0;
    buttons.okLck = BUTTON_ENABLE;
    buttons.clLck = BUTTON_ENABLE;
    ug2864off();
    deInitIR();
    adcDeinit();
    //deinitI2C1();
    stopModeConfig();
    __WFI();
    adcInit();
}

/*!****************************************************************************
* @brief    Power on sequence
* @param    
* @retval   
*/
void powerOn(void){
    power.pwrMode = ePwrRun;
    //initI2C1();
    ug2864on();
    initIRConfig();
}

/*!****************************************************************************
* @brief    Peripherals initialization sequence
* @param    
* @retval   
*/
void initPeriphs(void){
    initClocks();
    initGpios();
    initI2C1();
    ug2864init();
    lis3init();
    syncInit();
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
