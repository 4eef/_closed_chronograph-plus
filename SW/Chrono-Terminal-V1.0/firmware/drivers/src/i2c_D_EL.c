/**
*******************************************************************************
* @file    i2c.c 
* @author  Storozhenko Roman - D_EL
* @version V1.1
* @date    1-06-2015
* @brief
*/
#include "i2c_D_EL.h"

/**************************************
* i2c1 memory
*/
#if (I2C1_USE == 1)
i2c_type        i2c1Sct;
i2c_type        *i2c1 = &i2c1Sct;
uint8_t         i2c1TxBff[I2C1_TxBffSz];
uint8_t         i2c1RxBff[I2C1_RxBffSz];
#endif //I2C1_USE

/**************************************
* i2c2 memory
*/
#if (I2C2_USE == 1)
i2c_type        i2c2Sct;
i2c_type        *i2c2 = &i2c2Sct;
uint8_t         i2c2TxBff[I2C2_TxBffSz];
uint8_t         i2c2RxBff[I2C2_RxBffSz];
#endif //I2C2_USE

/**************************************
* i2c3 memory
*/
#if (I2C3_USE == 1)
i2c_type        i2c3Sct;
i2c_type        *i2c3 = &i2c3Sct;
uint8_t         i2c3TxBff[I2C3_TxBffSz];
uint8_t         i2c3RxBff[I2C3_RxBffSz];
#endif //I2C3_USE


/******************************************************************************
*
*/
void i2c_init(i2c_type *i2cx){
    I2C_InitTypeDef     i2cInit;
    I2C_TypeDef         *I2Cx;
    uint32_t            dmaChannelRx; 
    uint32_t            dmaChannelTx;
    
    #if (I2C1_USE == 1)
    if(i2cx == i2c1){
        /************************************************
        * Memory setting
        */
        I2Cx                = I2C1;
        i2cx->I2c           = I2C1;
        i2cx->pDmaStreamRx  = DMA1_Stream5;
        i2cx->pDmaStreamTx  = DMA1_Stream6; 
        dmaChannelRx = 1;
        dmaChannelTx = 1;
        
        /************************************************
        * IO
        * PB6, PB8 - clk
        * PB7, PB9 - sda
        */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6 , GPIO_AF_I2C1);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7 , GPIO_AF_I2C1);
        gpinit(GPIO_Mode_AF, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
        
        /************************************************
        * NVIC
        */
        NVIC_EnableIRQ(I2C1_EV_IRQn);           //Event interrupt enable
        NVIC_SetPriority(I2C1_EV_IRQn, I2C1_EventInterruptPrior);
        NVIC_EnableIRQ(I2C1_ER_IRQn);           //Error interrupt enable
        NVIC_SetPriority(I2C1_ER_IRQn, I2C1_ErrorInterruptPrior);
        
        NVIC_EnableIRQ(DMA1_Stream5_IRQn);      //RX
        NVIC_SetPriority(DMA1_Stream5_IRQn, I2C1_RxDmaInterruptPrior); 
        NVIC_EnableIRQ(DMA1_Stream6_IRQn);      //TX
        NVIC_SetPriority(DMA1_Stream6_IRQn, I2C1_TxDmaInterruptPrior);
        
        /************************************************
        * I2C clock
        */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
        
        /************************************************
        * DMA clock
        */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    }
    #endif //I2C1_USE
    
    #if (I2C2_USE == 1)
    if(i2cx == i2c2){
    }
    #endif //I2C2_USE
    
    /************************************************
    * I2C
    */    
    i2cInit.I2C_Mode                = I2C_Mode_I2C;
    i2cInit.I2C_DutyCycle           = I2C_DutyCycle_16_9;
    i2cInit.I2C_OwnAddress1         = 0x00;
    i2cInit.I2C_Ack                 = I2C_Ack_Enable;
    i2cInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    i2cInit.I2C_ClockSpeed          = i2cx->clockSpeed;
    
    I2Cx->CR2 |= I2C_CR2_ITEVTEN;
    I2Cx->CR2 |= I2C_CR2_ITERREN;
    I2Cx->CR2 |= I2C_CR2_DMAEN;
    I2Cx->CR2 |= I2C_CR2_LAST;
    
    //I2C Initialize
    I2C_Init(I2Cx, &i2cInit);
    //I2C ENABLE
    I2C_Cmd(I2Cx, ENABLE);
    
    /************************************************
    * DMA
    */
    //DMA I2C RX
    i2cx->pDmaStreamRx->CR |= (uint32_t)((dmaChannelRx & 0x03) << 25);  //Channel selection
    i2cx->pDmaStreamRx->CR |= DMA_SxCR_PL_1;                            //Priority level High
    i2cx->pDmaStreamRx->CR &= ~DMA_SxCR_MSIZE;                          //Memory data size 8 bit
    i2cx->pDmaStreamRx->CR &= ~DMA_SxCR_PSIZE;                          //Memory data size 8 bit
    i2cx->pDmaStreamRx->CR |= DMA_SxCR_MINC;                            //Memory increment mode enebled
    i2cx->pDmaStreamRx->CR &= ~DMA_SxCR_PINC;                           //Peripheral increment mode disabled
    i2cx->pDmaStreamRx->CR &= ~DMA_SxCR_DIR;                            //Direction Peripheral-to-memory
    i2cx->pDmaStreamRx->CR |= DMA_SxCR_TCIE;                            //Transfer complete interrupt enable
    i2cx->pDmaStreamRx->PAR = (uint32_t)&i2cx->I2c->DR;
    //DMA I2C TX
    i2cx->pDmaStreamTx->CR |= (uint32_t)((dmaChannelTx & 0x03) << 25);  //Channel selection
    i2cx->pDmaStreamTx->CR |= DMA_SxCR_PL_1;                            //Priority level High
    i2cx->pDmaStreamTx->CR &= ~DMA_SxCR_MSIZE;                          //Memory data size 8 bit
    i2cx->pDmaStreamTx->CR &= ~DMA_SxCR_PSIZE;                          //Memory data size 8 bit
    i2cx->pDmaStreamTx->CR |= DMA_SxCR_MINC;                            //Memory increment mode enebled
    i2cx->pDmaStreamTx->CR &= ~DMA_SxCR_PINC;                           //Peripheral increment mode disabled
    i2cx->pDmaStreamTx->CR |= DMA_SxCR_DIR_0;                           //Direction Memory-to-peripheral
    i2cx->pDmaStreamTx->CR |= DMA_SxCR_TCIE;                            //Transfer complete interrupt enable
    i2cx->pDmaStreamTx->PAR = (uint32_t)&i2cx->I2c->DR;
}

/******************************************************************************
*
*/
void i2c_reInit(i2c_type *i2cx){
    i2cx->pDmaStreamRx->CR  &= ~DMA_SxCR_EN;
    i2cx->pDmaStreamTx->CR  &= ~DMA_SxCR_EN;
    i2c_init(i2cx);
}

/******************************************************************************
*
*/
void i2c_write(i2c_type *i2cx, void *src, uint16_t len, uint8_t slaveAdr){
    i2cx->pDmaStreamTx->CR      &= ~DMA_SxCR_EN; 
    i2cx->pDmaStreamTx->M0AR    = (uint32_t)src;
    i2cx->pDmaStreamTx->NDTR    = len;
    i2cx->slaveAdr              = slaveAdr & 0xFE;              //Write     
    i2cx->pDmaStreamTx->CR      |= DMA_SxCR_EN;
    i2cx->I2c->CR1              |= I2C_CR1_START;               //Generation start condition
    i2cx->state                 = i2cTxRun;
}

/******************************************************************************
*
*/
void i2c_read(i2c_type *i2cx, void *dst, uint16_t len, uint8_t slaveAdr){
    i2cx->pDmaStreamRx->CR      &= ~DMA_SxCR_EN;
    i2cx->pDmaStreamRx->M0AR    = (uint32_t)dst;
    i2cx->pDmaStreamRx->NDTR    = len;
    i2cx->slaveAdr              = slaveAdr | 0x01;              //Read
    i2cx->pDmaStreamRx->CR      |= DMA_SxCR_EN;
    I2C1->CR1                   |= I2C_CR1_START;               //Generation start condition
    i2cx->state                 = i2cRxRun;
}

/******************************************************************************
* I2C EVENT HANDLER
*/
void i2cEventHendler(i2c_type *i2cx){
    __IO uint32_t I2Cx_SR1;
    
    I2Cx_SR1 = i2cx->I2c->SR1;

    if(I2Cx_SR1 & I2C_SR1_SB){              //Start condition generated
        i2cx->I2c->DR = i2c1Sct.slaveAdr;
        return;
    }
    if(I2Cx_SR1 & I2C_SR1_ADDR){            //ADDR is set
        (void)i2cx->I2c->SR2;               //Clear ADDR flag by reading SR2 register
        return;
    }
    
    if(I2Cx_SR1 & I2C_SR1_BTF){             //Data byte transfer succeeded
        I2C1->CR1   |= I2C_CR1_STOP;     
        while((I2C1->CR1 & I2C_CR1_STOP) != 0);
        if(i2cx->state == i2cTxRun){
            i2cx->state = i2cTxSuccess;
        }
    }
}

/******************************************************************************
* I2C ERROR HANDLER
*/
void i2cErrorHendler(i2c_type *i2cx){
    __IO uint32_t I2Cx_SR1;
    
    I2Cx_SR1 = i2cx->I2c->SR1;
    
    if((I2Cx_SR1 & I2C_SR1_BERR) != 0){
        i2cx->state = i2cBusError;
    }
    else if((I2Cx_SR1 & I2C_SR1_AF) != 0){
        i2cx->state = i2cAcknowledgeFailure;
    }
    else if((I2Cx_SR1 & I2C_SR1_PECERR) != 0){
        i2cx->state = i2cPECError;
    }
    i2cx->I2c->SR1 = ~(I2C_SR1_TIMEOUT | I2C_SR1_PECERR | I2C_SR1_AF | I2C_SR1_ARLO | I2C_SR1_BERR);
}

/******************************************************************************
* I2C1
*/
#if (I2C1_USE == 1)
__irq void I2C1_EV_IRQHandler(void){
    i2cEventHendler(i2c1);
}
__irq void I2C1_ER_IRQHandler(void){
    i2cErrorHendler(i2c1);
}
__irq void DMA1_Stream5_IRQHandler(void){   //RX
    i2c1Sct.state = i2cRxSuccess;
    DMA1->HIFCR  =  DMA_HIFCR_CTCIF5;       //Clear flag
}
__irq void DMA1_Stream6_IRQHandler(void){   //TX
    DMA1->HIFCR  =  DMA_HIFCR_CTCIF6;       //Clear flag
}
#endif

/******************************************************************************
* I2C2
*/
#if (I2C2_USE == 1)
__irq void I2C1_EV_IRQHandler(void){
    i2cEventHendler(i2c2);
}
__irq void I2C1_ER_IRQHandler(void){
    i2cErrorHendler(i2c2);
}
__irq void DMA1_Stream3_IRQHandler(void){   //RX
    DMA1->HIFCR  =  DMA_HIFCR_CTCIF3;       //Clear flag
    i2c2Sct.state = i2cRxSuccess;
}
__irq void DMA1_Stream7_IRQHandler(void){   //TX
    DMA1->HIFCR  =  DMA_HIFCR_CTCIF7;       //Clear flag
}
#endif

/******************************************************************************
* I2C3
*/
#if (I2C3_USE == 1)
__irq void I2C1_EV_IRQHandler(void){
    i2cEventHendler(i2c3);
}
__irq void I2C1_ER_IRQHandler(void){
    i2cErrorHendler(i2c3);
}
__irq void DMA1_Stream2_IRQHandler(void){   //RX
    DMA1->HIFCR  =  DMA_HIFCR_CTCIF2;       //Clear flag
    i2c3Sct.state = i2cRxSuccess;
}
__irq void DMA1_Stream4_IRQHandler(void){   //TX
    DMA1->HIFCR  =  DMA_HIFCR_CTCIF4;       //Clear flag
}
#endif


















//##############################################################################
//    if(I2Cx == I2C2){
//        /************************************************
//        * IO
//        * PB10 - clk
//        * PB11, PB3, PB9 - sda
//        */
//        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//        gpinit(GPIO_Mode_AF, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
//        GPIO_PinAFConfig(GPIOB, GPIO_PinSource10 , GPIO_AF_I2C2);
//        GPIO_PinAFConfig(GPIOB, GPIO_PinSource11 , GPIO_AF_I2C2);
//    }
//    if(I2Cx == I2C3){
//        /************************************************
//        * IO
//        * PC9 - clk
//        * PA8 - sda
//        */
//        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//        gpinit(GPIO_Mode_AF, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIOA, GPIO_Pin_8);
//        gpinit(GPIO_Mode_AF, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_50MHz, GPIOC, GPIO_Pin_9);
//        GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);
//        GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);
//        
//
//        
//    }

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/