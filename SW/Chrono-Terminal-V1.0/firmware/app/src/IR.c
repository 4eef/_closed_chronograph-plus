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
extern pellets_type         pellets;
extern sysPars_type         sysPars;
extern power_type           power;
IRRXData_type               IRRXData;

void hndlIRData(void){
    uint32_t dist, sgn, spd0, spd1, spd2, spd3, spd4, val1, val2, val3, val4;
    uint8_t i;
    if(IRRXData.rxState == IR_DATA_READY){
        sgn = (IRRXData.rxByte[IR_MAX_BYTES-1]) | (IRRXData.rxByte[IR_MAX_BYTES-2])<<8 | (IRRXData.rxByte[IR_MAX_BYTES-3])<<16 | (IRRXData.rxByte[IR_MAX_BYTES-4])<<24;
        if((sysPars.dispMode != eInclinometer) && ((meas.chron.chrSgntr == sgn) || (meas.chron.chrBindCnt != 0))){
            power.uptimeCurr = 0;
            if(meas.chron.chrBindCnt != 0){
                meas.chron.chrBindCnt = 0;
                meas.chron.chrSgntr = sgn;
                Menu_putMessage("Binded", MSG_CNT);
            }
            meas.stats.shotsTotal++;
            //Measurements
            dist = meas.chron.sensDist*CHR_DIST_MPLY;
            spd0 = dist/(((IRRXData.rxByte[0]<<8 | IRRXData.rxByte[1])*CHR_TCK_NS)/100);
            meas.chron.pelSgntr = IRRXData.rxByte[2]<<8 | IRRXData.rxByte[3];
            spd1 = dist/(((IRRXData.rxByte[4]<<8 | IRRXData.rxByte[5])*CHR_TCK_NS)/100);
            spd2 = dist/(((IRRXData.rxByte[6]<<8 | IRRXData.rxByte[7])*CHR_TCK_NS)/100);
            spd3 = dist/(((IRRXData.rxByte[8]<<8 | IRRXData.rxByte[9])*CHR_TCK_NS)/100);
            spd4 = dist/(((IRRXData.rxByte[10]<<8 | IRRXData.rxByte[11])*CHR_TCK_NS)/100);
            meas.chron.speed0 = spd0;
            if(spd0 >= CHR_SPD_MAX) meas.chron.speed0 = CHR_SPD_MAX;
            meas.chron.speed1 = spd1;
            if(spd1 >= CHR_SPD_MAX) meas.chron.speed1 = CHR_SPD_MAX;
            meas.chron.speed2 = spd2;
            if(spd2 >= CHR_SPD_MAX) meas.chron.speed2 = CHR_SPD_MAX;
            meas.chron.speed3 = spd3;
            if(spd3 >= CHR_SPD_MAX) meas.chron.speed3 = CHR_SPD_MAX;
            meas.chron.speed4 = spd4;
            if(spd4 >= CHR_SPD_MAX) meas.chron.speed4 = CHR_SPD_MAX;
            //Ñalculate clip status
            if(meas.chron.clipCapacity > 1){
                if(meas.chron.clipCurrent == 0){
                    meas.chron.clipCurrent = meas.chron.clipCapacity;
                    Menu_putMessage("Clip reloaded", MSG_CNT);
                }else if(meas.chron.clipCurrent == 1){
                    Menu_putMessage("Replace clip", MSG_CNT);
                }
                meas.chron.clipCurrent--;
            }
            //Pellet recognition by signature
            if(pellets.pelStat == PELLET_OK){
                val1 = meas.chron.pelSgntr-meas.chron.pelSgntr/PELLET_SGN_TOLERANCE;
                val2 = meas.chron.pelSgntr+meas.chron.pelSgntr/PELLET_SGN_TOLERANCE;
                //Compare current signature with existing in database
                if((pellets.pelSgntrs[meas.chron.pellet] >= val1) && (pellets.pelSgntrs[meas.chron.pellet] <= val2) && (pellets.matchedSgnNum != 0)){
                    pellets.newSgnSum = 0;
                    pellets.newSgnCnt = 0;
                    pellets.newSgnErrCnt = 0;
                }else if(pellets.matchedSgnNum != 0){
                    pellets.newSgnSum += meas.chron.pelSgntr;
                    pellets.newSgnCnt++;
                    pellets.newSgnErrCnt++;
                    if(pellets.newSgnCnt >= PELLET_CHANGE_THR){
                        pellets.newSgn = pellets.newSgnSum/pellets.newSgnCnt;
                        pellets.matchedSgnNum = 0;
                        val3 = pellets.newSgn-pellets.newSgn/PELLET_SGN_TOLERANCE;
                        val4 = pellets.newSgn+pellets.newSgn/PELLET_SGN_TOLERANCE;
                        for(i = 1; i < PELLET_DB_NUM; i++){
                            if(pellets.pelSgntrs[i] >= val3 && pellets.pelSgntrs[i] <= val4){
                                pellets.matchedSgnNum = i;
                                pellets.pelStat = PELLET_CONFIRM;
                                break;
                            }
                        }
                    }
                }else{                                                          //No existing pellet found
                    pellets.newSgnErrCnt++;
                    if((pellets.newSgn >= val1) && (pellets.newSgn <= val2)){
                        pellets.newSgnCnt++;
                        pellets.newSgnSum += meas.chron.pelSgntr;
                    }else{
                        pellets.newSgn = meas.chron.pelSgntr;
                        pellets.newSgnCnt = 1;
                        pellets.newSgnSum = meas.chron.pelSgntr;
                    }
                    if(pellets.newSgnCnt >= PELLET_NEW_SGN_THR){
                        pellets.newSgn = pellets.newSgnSum/pellets.newSgnCnt;
                        pellets.pelStat = PELLET_NEW;
                        pellets.matchedSgnNum = 1;
                        pellets.newSgnErrCnt = 0;
                        pellets.newSgnCnt = 0;
                        pellets.newSgnSum = 0;
                    }else if(pellets.newSgnErrCnt >= PELLET_NEW_SGN_BOUND){
                        Menu_putMessage("Error pellet ID", MSG_CNT);
                        pellets.pelStat = PELLET_OK;
                        pellets.newSgnErrCnt = 0;
                        pellets.newSgnCnt = 0;
                        pellets.newSgnSum = 0;
                    }
                }
            }
            //Statistics calculation
            if(sysPars.dispMode == eChronograph){
                if(meas.chron.statShots >= STAT_SHOTS_MAX){
                    Menu_putMessage("Buffer is full", MSG_CNT);
                }else{
                    meas.chron.statSpeeds[meas.chron.statShots] = meas.chron.speed0;
                    meas.chron.statSpeedsSum += meas.chron.statSpeeds[meas.chron.statShots];
                    meas.chron.statShots++;
                    if(meas.chron.statShots >= 2){
                        meas.chron.statMean = meas.chron.statSpeedsSum/meas.chron.statShots;
                        meas.chron.statDevsSum = 0;
                        for(i = 0; i < meas.chron.statShots; i++){
                            if(meas.chron.statMean >= meas.chron.statSpeeds[i]){
                                meas.chron.statDevsSum += meas.chron.statMean - meas.chron.statSpeeds[i];
                            }else{
                                meas.chron.statDevsSum += meas.chron.statSpeeds[i] - meas.chron.statMean;
                            }
                        }
                        meas.chron.statSdev = meas.chron.statDevsSum/meas.chron.statShots;
                    }
                    if((meas.chron.pellet != 0) && (pellets.pelWghts[meas.chron.pellet] != 0)){
                        meas.chron.statEnergy = (((meas.chron.speed0*meas.chron.speed0)/STAT_ENERGY_DIV_COEFF)*pellets.pelWghts[meas.chron.pellet])/STAT_ENERGY_DIV_COEFF/2;
                    }
                }
            }
        }
        IRRXData.rxState = IR_READY;                                            //Release receiver
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
    IRRXData.rxBitCnt = 0;
    IRRXData.rxBytesCnt = 0;
    IRRXData.rxState = IR_READY;
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
        if(IRRXData.rxState == IR_DATA_READY){                  //Avoid data overrun
            return;
        }else{
            IRRXData.rxBitCnt = 0;
            IRRXData.rxBytesCnt = 0;
            IRRXData.rxState = IR_READY;
        }
    }else if((TIM2->SR & TIM_SR_CC1IF) != 0){
        TIM2->SR &= ~TIM_SR_CC1IF;
        if(IRRXData.rxState == IR_DATA_READY){
            TIM2->CR1 &= ~TIM_CR1_CEN;
            return;
        }else if((GPIOA->IDR & GPIO_IDR_5) != 0){               //Rising edge
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
                if((TIM2->CCR1 <= (IR_PREAMBLE_TIME+IR_PREAMBLE_TIME/CHR_TOLERANCE)) && (TIM2->CCR1 >= (IR_PREAMBLE_TIME-IR_PREAMBLE_TIME/CHR_TOLERANCE)) && (IRRXData.rxState == IR_READY)){
                    IRRXData.rxBitCnt = 0;
                    IRRXData.rxBytesCnt = 0;
                    IRRXData.rxState = IR_BUSY;
                }else if((TIM2->CCR1 <= (IR_LOG_ZERO_TIME+IR_LOG_ZERO_TIME/CHR_TOLERANCE)) && (TIM2->CCR1 >= (IR_LOG_ZERO_TIME-IR_LOG_ZERO_TIME/CHR_TOLERANCE)) && (IRRXData.rxState == IR_BUSY)){
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
                }else if((TIM2->CCR1 <= (IR_LOG_ONE_TIME+IR_LOG_ONE_TIME/CHR_TOLERANCE)) && (TIM2->CCR1 >= (IR_LOG_ONE_TIME-IR_LOG_ONE_TIME/CHR_TOLERANCE)) && (IRRXData.rxState == IR_BUSY)){
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
            TIM2->CNT = 0;
            TIM2->CR1 |= TIM_CR1_CEN;
        }
    }else if((TIM2->SR & TIM_SR_UIF) != 0){
        TIM2->SR &= ~TIM_SR_UIF;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        if(IRRXData.rxState == IR_DATA_READY){
            return;
        }else{
            IRRXData.rxBitCnt = 0;
            IRRXData.rxBytesCnt = 0;
            IRRXData.rxState = IR_READY;
        }
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
