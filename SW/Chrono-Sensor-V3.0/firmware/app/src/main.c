/*!****************************************************************************
* @file    main.c
* @author  4eef
* @version V1.0
* @date    16.12.2017, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "main.h"

/*!****************************************************************************
* MEMORY
*/

/*!****************************************************************************
* @brief    
* @param    
* @retval   
*/
void main(void){
    RCC->APB1ENR    |= RCC_APB1ENR_PWREN;
    RCC->CFGR       |= RCC_CFGR_STOPWUCK;
    SCB->SCR        |= SCB_SCR_SLEEPDEEP_Msk;
    PWR->CR         &= ~PWR_CR_PDDS;
    PWR->CR         |= PWR_CR_LPSDSR;
    PWR->CR         |= PWR_CR_ULP;
    PWR->CR         |= PWR_CR_CWUF;
    __WFI();
    while(1){
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
