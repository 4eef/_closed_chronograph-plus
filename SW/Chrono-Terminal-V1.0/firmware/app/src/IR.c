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
extern meas_type            meas;
extern chron_type           chron;
extern stats_type           stats;
extern pellets_type         pellets;
extern sysPars_type         sysPars;
extern power_type           power;
irRxData_type               irRxData;

void hndlIRData(void){
    uint32_t dist, sgn, spd0, spd1, spd2, spd3, spd4, val1, val2, val3, val4;
    uint8_t i;
    if(irRxData.rxState == IR_DATA_READY){
        sgn = (irRxData.rxByte[IR_MAX_BYTES-1]) | (irRxData.rxByte[IR_MAX_BYTES-2])<<8 | (irRxData.rxByte[IR_MAX_BYTES-3])<<16 | (irRxData.rxByte[IR_MAX_BYTES-4])<<24;
        if((sysPars.dispMode != eInclinometer) && ((chron.chrSgntr == sgn) || (chron.chrBindCnt != 0))){
            power.uptimeCurr = 0;
            if(chron.chrBindCnt != 0){
                chron.chrBindCnt = 0;
                chron.chrSgntr = sgn;
                Menu_putMsg("Binded", MSG_CNT);
            }
            stats.shotsTotal++;
            //Measurements
            dist = chron.sensDist*CHR_DIST_MPLY;
            spd0 = dist/(((irRxData.rxByte[0]<<8 | irRxData.rxByte[1])*CHR_TCK_NS)/100);
            chron.pelSgntr = irRxData.rxByte[2]<<8 | irRxData.rxByte[3];
            spd1 = dist/(((irRxData.rxByte[4]<<8 | irRxData.rxByte[5])*CHR_TCK_NS)/100);
            spd2 = dist/(((irRxData.rxByte[6]<<8 | irRxData.rxByte[7])*CHR_TCK_NS)/100);
            spd3 = dist/(((irRxData.rxByte[8]<<8 | irRxData.rxByte[9])*CHR_TCK_NS)/100);
            spd4 = dist/(((irRxData.rxByte[10]<<8 | irRxData.rxByte[11])*CHR_TCK_NS)/100);
            chron.speed0 = spd0;
            if(spd0 >= CHR_SPD_MAX) chron.speed0 = CHR_SPD_MAX;
            chron.speed1 = spd1;
            if(spd1 >= CHR_SPD_MAX) chron.speed1 = CHR_SPD_MAX;
            chron.speed2 = spd2;
            if(spd2 >= CHR_SPD_MAX) chron.speed2 = CHR_SPD_MAX;
            chron.speed3 = spd3;
            if(spd3 >= CHR_SPD_MAX) chron.speed3 = CHR_SPD_MAX;
            chron.speed4 = spd4;
            if(spd4 >= CHR_SPD_MAX) chron.speed4 = CHR_SPD_MAX;
            //Ñalculate clip status
            if(chron.clipCapacity > 1){
                if(chron.clipCurrent == 0){
                    chron.clipCurrent = chron.clipCapacity;
                    Menu_putMsg("Clip reloaded", MSG_CNT);
                }else if(chron.clipCurrent == 1){
                    Menu_putMsg("Replace clip", MSG_CNT);
                }
                chron.clipCurrent--;
            }
            //Pellet recognition by signature
            if(pellets.irRxPelSgn.pelStat == PELLET_OK){
                val1 = chron.pelSgntr-chron.pelSgntr/PELLET_SGN_TOLERANCE;
                val2 = chron.pelSgntr+chron.pelSgntr/PELLET_SGN_TOLERANCE;
                //Compare current signature with existing in database
                if((pellets.pel[chron.pellet].sgn >= val1) && (pellets.pel[chron.pellet].sgn <= val2) && (pellets.matchedSgnNum != 0)){
                    pellets.irRxPelSgn.newSgnSum = 0;
                    pellets.irRxPelSgn.newSgnCnt = 0;
                    pellets.irRxPelSgn.newSgnErrCnt = 0;
                }else if(pellets.matchedSgnNum != 0){
                    pellets.irRxPelSgn.newSgnSum += chron.pelSgntr;
                    pellets.irRxPelSgn.newSgnCnt++;
                    pellets.irRxPelSgn.newSgnErrCnt++;
                    if(pellets.irRxPelSgn.newSgnCnt >= PELLET_CHANGE_THR){
                        pellets.irRxPelSgn.newSgn = pellets.irRxPelSgn.newSgnSum/pellets.irRxPelSgn.newSgnCnt;
                        pellets.matchedSgnNum = 0;
                        val3 = pellets.irRxPelSgn.newSgn-pellets.irRxPelSgn.newSgn/PELLET_SGN_TOLERANCE;
                        val4 = pellets.irRxPelSgn.newSgn+pellets.irRxPelSgn.newSgn/PELLET_SGN_TOLERANCE;
                        for(i = 1; i < PELLET_DB_QTY; i++){
                            if(pellets.pel[i].sgn >= val3 && pellets.pel[i].sgn <= val4){
                                pellets.matchedSgnNum = i;
                                pellets.irRxPelSgn.pelStat = PELLET_CONFIRM;
                                break;
                            }
                        }
                    }
                }else{                                                          //No existing pellet found
                    pellets.irRxPelSgn.newSgnErrCnt++;
                    if((pellets.irRxPelSgn.newSgn >= val1) && (pellets.irRxPelSgn.newSgn <= val2)){
                        pellets.irRxPelSgn.newSgnCnt++;
                        pellets.irRxPelSgn.newSgnSum += chron.pelSgntr;
                    }else{
                        pellets.irRxPelSgn.newSgn = chron.pelSgntr;
                        pellets.irRxPelSgn.newSgnCnt = 1;
                        pellets.irRxPelSgn.newSgnSum = chron.pelSgntr;
                    }
                    if(pellets.irRxPelSgn.newSgnCnt >= PELLET_NEW_SGN_THR){
                        pellets.irRxPelSgn.newSgn = pellets.irRxPelSgn.newSgnSum/pellets.irRxPelSgn.newSgnCnt;
                        pellets.irRxPelSgn.pelStat = PELLET_NEW;
                        pellets.matchedSgnNum = 0;
                        pellets.irRxPelSgn.newSgnErrCnt = 0;
                        pellets.irRxPelSgn.newSgnCnt = 0;
                        pellets.irRxPelSgn.newSgnSum = 0;
                    }else if(pellets.irRxPelSgn.newSgnErrCnt >= PELLET_NEW_SGN_BOUND){
                        Menu_putMsg("Error pellet ID", MSG_CNT);
                        pellets.irRxPelSgn.pelStat = PELLET_OK;
                        pellets.irRxPelSgn.newSgnErrCnt = 0;
                        pellets.irRxPelSgn.newSgnCnt = 0;
                        pellets.irRxPelSgn.newSgnSum = 0;
                    }
                }
            }
            //Statistics calculation
            if(sysPars.dispMode == eChronograph){
                if(chron.statShots >= STAT_SHOTS_MAX){
                    Menu_putMsg("Buffer is full", MSG_CNT);
                }else{
                    chron.statSpeeds[chron.statShots] = chron.speed0;
                    chron.statSpeedsSum += chron.statSpeeds[chron.statShots];
                    chron.statShots++;
                    if(chron.statShots >= 2){
                        chron.statMean = chron.statSpeedsSum/chron.statShots;
                        chron.statDevsSum = 0;
                        for(i = 0; i < chron.statShots; i++){
                            if(chron.statMean >= chron.statSpeeds[i]){
                                chron.statDevsSum += chron.statMean - chron.statSpeeds[i];
                            }else{
                                chron.statDevsSum += chron.statSpeeds[i] - chron.statMean;
                            }
                        }
                        chron.statSdev = chron.statDevsSum/chron.statShots;
                    }
                    if(pellets.pel[chron.pellet].wght != 0){
                        chron.statEnergy = (((chron.speed0*chron.speed0)/STAT_ENERGY_DIV_COEFF)*pellets.pel[chron.pellet].wght)/STAT_ENERGY_DIV_COEFF/2;
                    }
                }
            }
        }
        irRxData.rxState = IR_READY;                                            //Release receiver
    }
}

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
    TIM2->CCER      |= (TIM_CCER_CC1P | TIM_CCER_CC1NP);        //Falling and rising edges
    TIM2->CCMR1     &= ~(0 << TIM_CCMR1_IC1PSC_Pos);            //No prescaler
    TIM2->CCER      |= TIM_CCER_CC1E;                           //Enable capturing
    TIM2->DIER      |= TIM_DIER_CC1IE;                          //Enable interrupts
    TIM2->DIER      |= TIM_DIER_UIE;
    TIM2->SR        &= ~TIM_SR_CC1OF;                           //Clear flags
    TIM2->SR        &= ~TIM_SR_CC1IF;
    TIM2->SR        &= ~TIM_SR_UIF;
    irRxData.rxBitCnt = 0;
    irRxData.rxBytesCnt = 0;
    irRxData.rxState = IR_READY;
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 15);
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
    irRxData.rxState = IR_BUSY;                                 //Set receiver busy
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
        if(irRxData.rxState == IR_DATA_READY){                  //Avoid data overrun
            return;
        }else{
            irRxData.rxBitCnt = 0;
            irRxData.rxBytesCnt = 0;
            irRxData.rxState = IR_READY;
        }
    }else if((TIM2->SR & TIM_SR_CC1IF) != 0){
        TIM2->SR &= ~TIM_SR_CC1IF;
        if(irRxData.rxState == IR_DATA_READY){
            TIM2->CR1 &= ~TIM_CR1_CEN;
            return;
        }else if((GPIOA->IDR & GPIO_IDR_5) != 0){               //Rising edge
            if((TIM2->CCR1 <= (IR_STOPBIT_TIME+IR_STOPBIT_TIME/10)) && (TIM2->CCR1 >= (IR_STOPBIT_TIME-IR_STOPBIT_TIME/10))){
                TIM2->CR1 &= ~TIM_CR1_CEN;
                irRxData.rxBitCnt = 0;
                //Security checks
                if((irRxData.rxBytesCnt == IR_MAX_BYTES) && (irRxData.rxState != IR_DATA_OVERRUN)){
                    irRxData.rxState = IR_DATA_READY;
                }else{
                    irRxData.rxBytesCnt = 0;
                    irRxData.rxState = IR_READY;
                }
            }
            TIM2->CCR1 = 0;
        }else{                                                  //Falling edge
            if((TIM2->CR1 & TIM_CR1_CEN) != 0){
                TIM2->CR1 &= ~TIM_CR1_CEN;
                if((TIM2->CCR1 <= (IR_PREAMBLE_TIME+IR_PREAMBLE_TIME/CHR_TOLERANCE)) && (TIM2->CCR1 >= (IR_PREAMBLE_TIME-IR_PREAMBLE_TIME/CHR_TOLERANCE)) && (irRxData.rxState == IR_READY)){
                    irRxData.rxBitCnt = 0;
                    irRxData.rxBytesCnt = 0;
                    irRxData.rxState = IR_BUSY;
                }else if((TIM2->CCR1 <= (IR_LOG_ZERO_TIME+IR_LOG_ZERO_TIME/CHR_TOLERANCE)) && (TIM2->CCR1 >= (IR_LOG_ZERO_TIME-IR_LOG_ZERO_TIME/CHR_TOLERANCE)) && (irRxData.rxState == IR_BUSY)){
                    irRxData.rxByte[irRxData.rxBytesCnt] &= ~(1 << irRxData.rxBitCnt);
                    irRxData.rxBitCnt++;
                    if(irRxData.rxBitCnt > IR_MAX_BITS){
                        irRxData.rxBitCnt = 0;
                        irRxData.rxBytesCnt++;
                        if(irRxData.rxBytesCnt > IR_MAX_BYTES){ //Ring the buffer with overrun flag
                            irRxData.rxState = IR_DATA_OVERRUN;
                            irRxData.rxBytesCnt = 0;
                        }
                    }
                }else if((TIM2->CCR1 <= (IR_LOG_ONE_TIME+IR_LOG_ONE_TIME/CHR_TOLERANCE)) && (TIM2->CCR1 >= (IR_LOG_ONE_TIME-IR_LOG_ONE_TIME/CHR_TOLERANCE)) && (irRxData.rxState == IR_BUSY)){
                    irRxData.rxByte[irRxData.rxBytesCnt] |= (1 << irRxData.rxBitCnt);
                    irRxData.rxBitCnt++;
                    if(irRxData.rxBitCnt > IR_MAX_BITS){
                        irRxData.rxBitCnt = 0;
                        irRxData.rxBytesCnt++;
                        if(irRxData.rxBytesCnt > IR_MAX_BYTES){
                            irRxData.rxState = IR_DATA_OVERRUN;
                            irRxData.rxBytesCnt = 0;
                        }
                    }
                }else{                                          //Error
                    TIM2->CR1 &= ~TIM_CR1_CEN;
                    irRxData.rxBitCnt = 0;
                    irRxData.rxBytesCnt = 0;
                    irRxData.rxState = IR_READY;
                    return;
                }
            }
            TIM2->CNT = 0;
            TIM2->CR1 |= TIM_CR1_CEN;
        }
    }else if((TIM2->SR & TIM_SR_UIF) != 0){
        TIM2->SR &= ~TIM_SR_UIF;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        if(irRxData.rxState == IR_DATA_READY){
            return;
        }else{
            irRxData.rxBitCnt = 0;
            irRxData.rxBytesCnt = 0;
            irRxData.rxState = IR_READY;
        }
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
