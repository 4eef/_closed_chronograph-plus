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

/*!****************************************************************************
* @brief    I2C1 deinitializer
* @param    
* @retval   
*/
void deinitI2C1(void){
    RCC->APB1RSTR   |= RCC_APB1RSTR_I2C1RST;
    RCC->APB1ENR    &= ~RCC_APB1ENR_I2C1EN;
}

/*!****************************************************************************
* @brief    I2C1 initializer
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
    RCC->APB1RSTR   |= RCC_APB1RSTR_I2C1RST;
    RCC->APB1RSTR   &= ~RCC_APB1RSTR_I2C1RST;
    I2C1->TIMINGR   |= (0x9 << I2C_TIMINGR_SCLL_Pos);           //400 kHz settings
    I2C1->TIMINGR   |= (0x3 << I2C_TIMINGR_SCLH_Pos);
    I2C1->TIMINGR   |= (0x3 << I2C_TIMINGR_SDADEL_Pos);
    I2C1->TIMINGR   |= (0x3 << I2C_TIMINGR_SCLDEL_Pos);
//    I2C1->CR1       |= (I2C_CR1_ERRIE | I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_RXIE | I2C_CR1_TXIE);
//    NVIC_EnableIRQ(I2C1_IRQn);
//    NVIC_SetPriority(I2C1_IRQn, 1);
//    //DMA
//    RCC->AHBENR     |= RCC_AHBENR_DMAEN;
//    //Channel 3 (peripheral to memory)
//    DMA1_Channel3->CCR &= ~DMA_CCR_MEM2MEM;
//    DMA1_Channel3->CCR |= (2 << DMA_CCR_PL_Pos);
//    DMA1_Channel3->CCR &= ~(3 << DMA_CCR_MSIZE_Pos);
//    DMA1_Channel3->CCR &= ~(3 << DMA_CCR_PSIZE_Pos);
//    DMA1_Channel3->CCR |= DMA_CCR_MINC;
//    DMA1_Channel3->CCR &= ~DMA_CCR_PINC;
//    DMA1_Channel3->CCR &= ~DMA_CCR_CIRC;
//    DMA1_Channel3->CCR &= ~DMA_CCR_DIR;
//    DMA1_Channel3->CCR &= ~DMA_CCR_TEIE;
//    DMA1_Channel3->CCR &= ~DMA_CCR_HTIE;
//    DMA1_Channel3->CCR &= ~DMA_CCR_TCIE;
//    DMA1_Channel3->CCR &= ~DMA_CCR_EN;
//    //Channel 2 (memory to peropheral)
//    DMA1_Channel2->CCR &= ~DMA_CCR_MEM2MEM;
//    DMA1_Channel2->CCR |= (1 << DMA_CCR_PL_Pos);
//    DMA1_Channel2->CCR &= ~(3 << DMA_CCR_MSIZE_Pos);
//    DMA1_Channel2->CCR &= ~(3 << DMA_CCR_PSIZE_Pos);
//    DMA1_Channel2->CCR |= DMA_CCR_MINC;
//    DMA1_Channel2->CCR &= ~DMA_CCR_PINC;
//    DMA1_Channel2->CCR &= ~DMA_CCR_CIRC;
//    DMA1_Channel2->CCR |= DMA_CCR_DIR;
//    DMA1_Channel2->CCR &= ~DMA_CCR_TEIE;
//    DMA1_Channel2->CCR &= ~DMA_CCR_HTIE;
//    DMA1_Channel2->CCR &= ~DMA_CCR_TCIE;
//    DMA1_Channel2->CCR &= ~DMA_CCR_EN;
//    NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
//    NVIC_SetPriority(DMA1_Channel2_3_IRQn, 2);
}

/*!****************************************************************************
* @brief    I2C receive initizlization function
* @param    
* @retval   
*/
void I2CRxInit(uint8_t sadd, uint16_t len){
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR1 |= I2C_CR1_PE;
    I2C1->CR2 &= ~(0xFF << I2C_CR2_NBYTES_Pos);
    if(len > I2C_NBYTES_MAX){
        I2C1->CR2 |= (I2C_NBYTES_MAX << I2C_CR2_NBYTES_Pos);
        I2C1->CR2 |= I2C_CR2_RELOAD;
    }else{
        I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos);
        I2C1->CR2 &= ~I2C_CR2_RELOAD;
    }
    I2C1->CR2 &= ~(0xFF << I2C_CR2_SADD_Pos);
    I2C1->CR2 |= (sadd << I2C_CR2_SADD_Pos);
    I2C1->CR2 |= I2C_CR2_RD_WRN;
    I2C1->CR2 |= I2C_CR2_START;
}

/*!****************************************************************************
* @brief    I2C receive initizlization function
* @param    
* @retval   
*/
uint8_t I2CRx(uint8_t sadd, uint8_t *dst, uint16_t len){
    uint8_t retVal;
    if((I2C1->ISR & I2C_ISR_BUSY) == 0){
        I2CRxInit(sadd, len);
        if(len > I2C_NBYTES_MAX) len -= I2C_NBYTES_MAX;
        //Receive loop
        while(1){
            if(I2C1->ISR & I2C_ISR_RXNE){
                *dst = I2C1->RXDR;
                dst++;
            }else if(I2C1->ISR & I2C_ISR_TC){
                I2C1->CR2 |= I2C_CR2_STOP;
                retVal = I2C_STAT_TC;
            }else if(I2C1->ISR & I2C_ISR_TCR){
                I2C1->CR2 &= ~(0xFF << I2C_CR2_NBYTES_Pos);
                if(len > I2C_NBYTES_MAX){
                    I2C1->CR2 |= (I2C_NBYTES_MAX << I2C_CR2_NBYTES_Pos);
                    I2C1->CR2 |= I2C_CR2_RELOAD;
                    len -= I2C_NBYTES_MAX;
                }else{
                    I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos);
                    I2C1->CR2 &= ~I2C_CR2_RELOAD;
                }
            }else if(I2C1->ISR & I2C_ISR_NACKF){
                I2C1->ICR |= I2C_ICR_NACKCF;
                I2C1->ICR |= I2C_ICR_STOPCF;
                retVal = I2C_STAT_NACK;
                break;
            }else if(I2C1->ISR & I2C_ISR_STOPF){
                I2C1->ICR |= I2C_ICR_STOPCF;
                break;
            }
        }
    }else{
        retVal = I2C_STAT_BUSY;
    }
    return retVal;
}

/*!****************************************************************************
* @brief    I2C receive initizlization function
* @param    
* @retval   
*/
void I2CTxInit(uint8_t sadd, uint16_t len){
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR1 |= I2C_CR1_PE;
    I2C1->CR2 &= ~(0xFF << I2C_CR2_NBYTES_Pos);
    if(len > I2C_NBYTES_MAX){
        I2C1->CR2 |= (I2C_NBYTES_MAX << I2C_CR2_NBYTES_Pos);
        I2C1->CR2 |= I2C_CR2_RELOAD;
    }else{
        I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos);
        I2C1->CR2 &= ~I2C_CR2_RELOAD;
    }
    I2C1->CR2 &= ~(0xFF << I2C_CR2_SADD_Pos);
    I2C1->CR2 |= (sadd << I2C_CR2_SADD_Pos);
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    I2C1->CR2 |= I2C_CR2_START;
}

/*!****************************************************************************
* @brief    I2C transmitter configuration function (interrupt handler used)
* @param    
* @retval   
*/
uint8_t I2CTx(uint8_t sadd, uint8_t *src, uint16_t len){
    uint8_t retVal;
    if((I2C1->ISR & I2C_ISR_BUSY) == 0){
        I2CTxInit(sadd, len);
        if(len > I2C_NBYTES_MAX) len -= I2C_NBYTES_MAX;
        //Transfer loop
        while(1){
            if(I2C1->ISR & I2C_ISR_TXIS){
                I2C1->TXDR = *src;
                src++;
            }else if(I2C1->ISR & I2C_ISR_TC){
                I2C1->CR2 |= I2C_CR2_STOP;
                retVal = I2C_STAT_TC;
            }else if(I2C1->ISR & I2C_ISR_TCR){
                I2C1->CR2 &= ~(0xFF << I2C_CR2_NBYTES_Pos);
                if(len > I2C_NBYTES_MAX){
                    I2C1->CR2 |= (I2C_NBYTES_MAX << I2C_CR2_NBYTES_Pos);
                    I2C1->CR2 |= I2C_CR2_RELOAD;
                    len -= I2C_NBYTES_MAX;
                }else{
                    I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos);
                    I2C1->CR2 &= ~I2C_CR2_RELOAD;
                }
            }else if(I2C1->ISR & I2C_ISR_NACKF){
                I2C1->ICR |= I2C_ICR_NACKCF;
                I2C1->ICR |= I2C_ICR_STOPCF;
                retVal = I2C_STAT_NACK;
                break;
            }else if(I2C1->ISR & I2C_ISR_STOPF){
                I2C1->ICR |= I2C_ICR_STOPCF;
                break;
            }
        }
    }else{
        retVal = I2C_STAT_BUSY;
    }
    return retVal;
}

/*!****************************************************************************
* @brief    I2C interrupt handler
* @param    
* @retval   
*/
__irq void I2C1_IRQHandler(void){

}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
