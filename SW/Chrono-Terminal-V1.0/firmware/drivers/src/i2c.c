/*!****************************************************************************
* @file    i2c.c
* @author  4eef
* @version V1.0
* @date    07.08.2016
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "i2c.h"

/*!****************************************************************************
* MEMORY
*/
I2C_HandleTypeDef       hi2c1;
DMA_HandleTypeDef       hdma_i2c1_rx;
DMA_HandleTypeDef       hdma_i2c1_tx;

/*!****************************************************************************
* @brief    Menu parameters routines
* @param    
* @retval   
*/
void initI2C1(void){
    //GPIO
    GPIOF->OSPEEDR  |= (3 << GPIO_OSPEEDR_OSPEEDR0_Pos);
    GPIOF->OSPEEDR  |= (3 << GPIO_OSPEEDR_OSPEEDR1_Pos);
    GPIOF->PUPDR    |= (1 << GPIO_PUPDR_PUPDR0_Pos);
    GPIOF->PUPDR    |= (1 << GPIO_PUPDR_PUPDR1_Pos);
    //I2C1
    RCC->APB1ENR    |= RCC_APB1ENR_I2C1EN;
    RCC->APB1ENR    |= RCC_APB1ENR_I2C1EN;
    RCC->APB1RSTR   &= ~RCC_APB1RSTR_I2C1RST;
    I2C1->TIMINGR   |= (0x9 << I2C_TIMINGR_SCLL_Pos);           //400 kHz settings
    I2C1->TIMINGR   |= (0x3 << I2C_TIMINGR_SCLH_Pos);
    I2C1->TIMINGR   |= (0x3 << I2C_TIMINGR_SDADEL_Pos);
    I2C1->TIMINGR   |= (0x3 << I2C_TIMINGR_SCLDEL_Pos);
    I2C1->CR2       |= I2C_CR2_AUTOEND;
    //I2C1->CR1       |= I2C_CR1_PE;
    NVIC_EnableIRQ(I2C1_IRQn);
    NVIC_SetPriority(I2C1_IRQn, 1);
    //DMA
    RCC->AHBENR     |= RCC_AHBENR_DMAEN;
    //Channel 3 (peripheral to memory)
    DMA1_Channel3->CCR &= ~DMA_CCR_MEM2MEM;
    DMA1_Channel3->CCR |= (2 << DMA_CCR_PL_Pos);
    DMA1_Channel3->CCR &= ~(3 << DMA_CCR_MSIZE_Pos);
    DMA1_Channel3->CCR &= ~(3 << DMA_CCR_PSIZE_Pos);
    DMA1_Channel3->CCR |= DMA_CCR_MINC;
    DMA1_Channel3->CCR &= ~DMA_CCR_PINC;
    DMA1_Channel3->CCR &= ~DMA_CCR_CIRC;
    DMA1_Channel3->CCR &= ~DMA_CCR_DIR;
    DMA1_Channel3->CCR &= ~DMA_CCR_TEIE;
    DMA1_Channel3->CCR &= ~DMA_CCR_HTIE;
    DMA1_Channel3->CCR &= ~DMA_CCR_TCIE;
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;
    //Channel 2 (memory to peropheral)
    DMA1_Channel2->CCR &= ~DMA_CCR_MEM2MEM;
    DMA1_Channel2->CCR |= (1 << DMA_CCR_PL_Pos);
    DMA1_Channel2->CCR &= ~(3 << DMA_CCR_MSIZE_Pos);
    DMA1_Channel2->CCR &= ~(3 << DMA_CCR_PSIZE_Pos);
    DMA1_Channel2->CCR |= DMA_CCR_MINC;
    DMA1_Channel2->CCR &= ~DMA_CCR_PINC;
    DMA1_Channel2->CCR &= ~DMA_CCR_CIRC;
    DMA1_Channel2->CCR |= DMA_CCR_DIR;
    DMA1_Channel2->CCR &= ~DMA_CCR_TEIE;
    DMA1_Channel2->CCR &= ~DMA_CCR_HTIE;
    DMA1_Channel2->CCR &= ~DMA_CCR_TCIE;
    DMA1_Channel2->CCR &= ~DMA_CCR_EN;
    NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
    NVIC_SetPriority(DMA1_Channel2_3_IRQn, 2);
}

/*!****************************************************************************
* I2C1 init function
*/
void MX_I2C1_Init(void){
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x00330309;//0000020B;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK){
        Error_Handler();
    }
    /* Configure Analogue filter */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK){
        Error_Handler();
    }
}

/*!****************************************************************************
* Enable DMA controller clock
*/
void MX_DMA_Init(void){
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel2_3_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
}

/*!****************************************************************************
* I2C read function
*/
void I2C_read(uint8_t sadd, uint8_t rgadd, uint8_t *dst, uint8_t len){
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    HAL_I2C_Master_Transmit(&hi2c1, (sadd | 0x1), &rgadd, 1, 1000);
    HAL_I2C_Master_Receive(&hi2c1, sadd, dst, len, 1000);
}

/*!****************************************************************************
* I2C read function with DMA usage
*/
void I2C_read_DMA(uint8_t sadd, uint8_t rgadd, uint8_t *dst, uint8_t len){
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY);
    HAL_I2C_Master_Transmit(&hi2c1, (sadd | 0x1), &rgadd, 1, 1000);
    HAL_I2C_Master_Receive_DMA(&hi2c1, sadd, dst, len);
}

/*!****************************************************************************
* I2C write function with DMA usage
*/
void I2C_write(uint8_t sadd, uint8_t *src, uint8_t len){
    HAL_I2C_Master_Transmit_DMA(&hi2c1, (sadd | 0x1), src, len);
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
