/*!****************************************************************************
* @file    battery.c
* @author  4eef
* @version V1.0
* @date    25.02.2017, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "battery.h"

/*!****************************************************************************
* MEMORY
*/
extern adcData_type         adcData;
extern meas_type            meas;
extern sysPars_type         sysPars;
extern menu_type            menu;

/*!****************************************************************************
* @brief    Calculate battery voltage and charge level
* @param    
* @retval   
*/
void battCalc(void){
    uint16_t volt;
    uint8_t i, j, tmp, perc;
    static uint8_t percFlag;
    //Charge status
    meas.battery.battChgStat = !(gppin_get(GP_ChrgStatus) >> 8);
    //Charge level calculation
    volt = lpf((adcData.adcRawData*ADC_N_TO_UV)/1000);
    meas.battery.battVolt = volt;
    if(volt >= meas.battery.battVltPts[0]){
        perc = 100;
    }else if(volt <= meas.battery.battVltPts[BATT_CHG_ARR_PTS-1]){
        perc = 0;
        //Put message
        if(meas.battery.battMsgPer == 0){
            if(volt <= BATT_VOLT_LOW){
                strcpy(menu.message, "Battery depleted");
            }else{
                strcpy(menu.message, "Low charge");
            }
            menu.msgCnt = MSG_CNT_LONG;
            meas.battery.battMsgPer = BATT_MSG_PERIOD;
        }
    }else{
        meas.battery.battMsgPer = 0;
        //Calculate percentage
        for(i = BATT_CHG_ARR_PTS-1; i > 0; i--){
            if(volt >= meas.battery.battVltPts[i]){
                perc = 100-i*10;
                j = i;
            }
        }
        tmp = meas.battery.battVltPts[j-1] - meas.battery.battVltPts[j];
        volt -= meas.battery.battVltPts[j];
        tmp = (volt*BATT_CHG_VMPLY_COEFF)/tmp;
        if((tmp > BATT_ADD_PERC_MIN) && (tmp <= BATT_ADD_PERC_MAX)){
            perc += tmp;
        }
    }
    //Add hysteresis to indicated percentage
    if(perc > meas.battery.battCharge){
        if((perc > (meas.battery.battCharge + BATT_PERC_HYST)) || (percFlag == BATT_PERC_FLAG_INC)){
            percFlag = BATT_PERC_FLAG_INC;
            meas.battery.battCharge = perc;
        }
    }else if(perc < meas.battery.battCharge){
        if((perc < (meas.battery.battCharge - BATT_PERC_HYST)) || (percFlag == BATT_PERC_FLAG_DEC)){
            percFlag = BATT_PERC_FLAG_DEC;
            meas.battery.battCharge = perc;
        }
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
