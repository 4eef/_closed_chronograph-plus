/*!****************************************************************************
* @file    clock.c
* @author  4eef
* @version V1.0
* @date    07.08.2016
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "clock.h"

/*!****************************************************************************
* MEMORY
*/

/*!****************************************************************************
* @brief    System clock settings initializer
* @param    
* @retval   
*/
void initClocks(void){
    RCC->CR2        |= RCC_CR2_HSI14ON;
    while((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) __NOP();
    RCC->CR2        |= RCC_CR2_HSI48ON;
    while((RCC->CR2 & RCC_CR2_HSI48RDY) == 0) __NOP();
    RCC->CFGR       |= (3 << RCC_CFGR_SW_Pos);
    while((RCC->CFGR & RCC_CFGR_SWS >> RCC_CFGR_SWS_Pos) != 3) __NOP();
    FLASH->ACR      |= FLASH_ACR_PRFTBE;
    while((FLASH->ACR & FLASH_ACR_PRFTBS) == 0) __NOP();
    FLASH->ACR      |= (1 << FLASH_ACR_LATENCY_Pos);
    RCC->CFGR3      &= ~(RCC_CFGR3_USBSW | RCC_CFGR3_I2C1SW);
}

/*!****************************************************************************
* System Clock Configuration
*/
void SystemClock_Config(void){
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.HSI14CalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK){
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_I2C1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK){
        Error_Handler();
    }

    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/*!****************************************************************************
* This function is executed in case of error occurrence.
*/
void Error_Handler(void){
    while(1){
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
