/*!****************************************************************************
* @file    chrono.c
* @author  4eef
* @version V1.0
* @date    07.11.2018, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "chrono.h"

/*!****************************************************************************
* MEMORY
*/
chrono_type                 chrono;
pellets_type                pellets;
extern irRxData_type        irRxData;
extern stats_type           stats;

/*!****************************************************************************
* @brief    Main function
*/
void hndlIRData(void){
    uint32_t dist, sgn, spd0, spd1, spd2, spd3, spd4, val1, val2, val3, val4;
    uint8_t i;
    if(irRxData.rxState == IR_DATA_READY){
        sgn = (irRxData.rxByte[IR_MAX_BYTES-1]) | (irRxData.rxByte[IR_MAX_BYTES-2])<<8 | (irRxData.rxByte[IR_MAX_BYTES-3])<<16 | (irRxData.rxByte[IR_MAX_BYTES-4])<<24;
        if((sysPars.dispMode != eInclinometer) && ((chrono.chrSgntr == sgn) || (chrono.chrBindCnt != 0))){
            powerTimRst();
            if(chrono.chrBindCnt != 0){
                chrono.chrBindCnt = 0;
                chrono.chrSgntr = sgn;
                menu_putMsg("Binded", MSG_CNT_DEFAULT);
            }
            stats.shotsTotal++;
            //Measurements
            dist = chrono.sensDist*CHR_DIST_MPLY;
            spd0 = dist/(((irRxData.rxByte[0]<<8 | irRxData.rxByte[1])*CHR_TCK_NS)/100);
            chrono.pelSgntr = irRxData.rxByte[2]<<8 | irRxData.rxByte[3];
            spd1 = dist/(((irRxData.rxByte[4]<<8 | irRxData.rxByte[5])*CHR_TCK_NS)/100);
            spd2 = dist/(((irRxData.rxByte[6]<<8 | irRxData.rxByte[7])*CHR_TCK_NS)/100);
            spd3 = dist/(((irRxData.rxByte[8]<<8 | irRxData.rxByte[9])*CHR_TCK_NS)/100);
            spd4 = dist/(((irRxData.rxByte[10]<<8 | irRxData.rxByte[11])*CHR_TCK_NS)/100);
            chrono.speed0 = spd0;
            if(spd0 >= CHR_SPD_MAX) chrono.speed0 = CHR_SPD_MAX;
            chrono.speed1 = spd1;
            if(spd1 >= CHR_SPD_MAX) chrono.speed1 = CHR_SPD_MAX;
            chrono.speed2 = spd2;
            if(spd2 >= CHR_SPD_MAX) chrono.speed2 = CHR_SPD_MAX;
            chrono.speed3 = spd3;
            if(spd3 >= CHR_SPD_MAX) chrono.speed3 = CHR_SPD_MAX;
            chrono.speed4 = spd4;
            if(spd4 >= CHR_SPD_MAX) chrono.speed4 = CHR_SPD_MAX;
            //Ñalculate clip status
            if(chrono.clipCapacity > 1){
                if(chrono.clipCurrent == 0){
                    chrono.clipCurrent = chrono.clipCapacity;
                    menu_putMsg("Clip reloaded", MSG_CNT_DEFAULT);
                }else if(chrono.clipCurrent == 1){
                    menu_putMsg("Replace clip", MSG_CNT_DEFAULT);
                }
                chrono.clipCurrent--;
            }
            //Pellet recognition by signature
            if(pellets.rxPelSgn.pelStat == PELLET_OK){
                val1 = chrono.pelSgntr-chrono.pelSgntr/PELLET_SGN_TOLERANCE;
                val2 = chrono.pelSgntr+chrono.pelSgntr/PELLET_SGN_TOLERANCE;
                //Compare current signature with existing in database
                if((pellets.pel[chrono.pellet].sgn >= val1) && (pellets.pel[chrono.pellet].sgn <= val2) && (pellets.matchedSgnNum != 0)){
                    pellets.rxPelSgn.newSgnSum = 0;
                    pellets.rxPelSgn.newSgnCnt = 0;
                    pellets.rxPelSgn.newSgnErrCnt = 0;
                }else if(pellets.matchedSgnNum != 0){
                    pellets.rxPelSgn.newSgnSum += chrono.pelSgntr;
                    pellets.rxPelSgn.newSgnCnt++;
                    pellets.rxPelSgn.newSgnErrCnt++;
                    if(pellets.rxPelSgn.newSgnCnt >= PELLET_CHANGE_THR){
                        pellets.rxPelSgn.newSgn = pellets.rxPelSgn.newSgnSum/pellets.rxPelSgn.newSgnCnt;
                        pellets.matchedSgnNum = 0;
                        val3 = pellets.rxPelSgn.newSgn-pellets.rxPelSgn.newSgn/PELLET_SGN_TOLERANCE;
                        val4 = pellets.rxPelSgn.newSgn+pellets.rxPelSgn.newSgn/PELLET_SGN_TOLERANCE;
                        for(i = 1; i < PELLET_DB_QTY; i++){
                            if(pellets.pel[i].sgn >= val3 && pellets.pel[i].sgn <= val4){
                                pellets.matchedSgnNum = i;
                                pellets.rxPelSgn.pelStat = PELLET_CONFIRM;
                                break;
                            }
                        }
                    }
                }else{                                                          //No existing pellet found
                    pellets.rxPelSgn.newSgnErrCnt++;
                    if((pellets.rxPelSgn.newSgn >= val1) && (pellets.rxPelSgn.newSgn <= val2)){
                        pellets.rxPelSgn.newSgnCnt++;
                        pellets.rxPelSgn.newSgnSum += chrono.pelSgntr;
                    }else{
                        pellets.rxPelSgn.newSgn = chrono.pelSgntr;
                        pellets.rxPelSgn.newSgnCnt = 1;
                        pellets.rxPelSgn.newSgnSum = chrono.pelSgntr;
                    }
                    if(pellets.rxPelSgn.newSgnCnt >= PELLET_NEW_SGN_THR){
                        pellets.rxPelSgn.newSgn = pellets.rxPelSgn.newSgnSum/pellets.rxPelSgn.newSgnCnt;
                        pellets.rxPelSgn.pelStat = PELLET_NEW;
                        pellets.matchedSgnNum = 0;
                        pellets.rxPelSgn.newSgnErrCnt = 0;
                        pellets.rxPelSgn.newSgnCnt = 0;
                        pellets.rxPelSgn.newSgnSum = 0;
                    }else if(pellets.rxPelSgn.newSgnErrCnt >= PELLET_NEW_SGN_BOUND){
                        menu_putMsg("Error pellet ID", MSG_CNT_DEFAULT);
                        pellets.rxPelSgn.pelStat = PELLET_OK;
                        pellets.rxPelSgn.newSgnErrCnt = 0;
                        pellets.rxPelSgn.newSgnCnt = 0;
                        pellets.rxPelSgn.newSgnSum = 0;
                    }
                }
            }
            //Statistics calculation
            if(sysPars.dispMode == eChronograph){
                if(chrono.statShots >= STAT_SHOTS_MAX){
                    menu_putMsg("Buffer is full", MSG_CNT_DEFAULT);
                }else{
                    chrono.statSpeeds[chrono.statShots] = chrono.speed0;
                    chrono.statSpeedsSum += chrono.statSpeeds[chrono.statShots];
                    chrono.statShots++;
                    if(chrono.statShots >= 2){
                        chrono.statMean = chrono.statSpeedsSum/chrono.statShots;
                        chrono.statDevsSum = 0;
                        for(i = 0; i < chrono.statShots; i++){
                            if(chrono.statMean >= chrono.statSpeeds[i]){
                                chrono.statDevsSum += chrono.statMean - chrono.statSpeeds[i];
                            }else{
                                chrono.statDevsSum += chrono.statSpeeds[i] - chrono.statMean;
                            }
                        }
                        chrono.statSdev = chrono.statDevsSum/chrono.statShots;
                    }
                    if(pellets.pel[chrono.pellet].wght != 0){
                        chrono.statEnergy = (((chrono.speed0*chrono.speed0)/STAT_ENERGY_DIV_COEFF)*pellets.pel[chrono.pellet].wght)/STAT_ENERGY_DIV_COEFF/2;
                    }
                }
            }
        }
        irRxData.rxState = IR_READY;                                            //Release receiver
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
