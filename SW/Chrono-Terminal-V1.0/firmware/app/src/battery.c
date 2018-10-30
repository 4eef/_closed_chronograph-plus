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
extern battery_type         battery;
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
    if(adcData.adcStat != ADC_DATA_READY) return;
    adcData.adcStat = ADC_READY;
    //Charge status
    battery.battChgStat = !(gppin_get(GP_ChrgStatus) >> 8);
    //Charge level calculation
    volt = lpf((adcData.adcRawData*ADC_N_TO_UV)/1000);
    battery.battVolt = volt;
    if(volt >= battery.battVltPts[0]){
        perc = 100;
    }else if(volt <= battery.battVltPts[BATT_CHG_ARR_PTS-1]){
        perc = 0;
        //Put message
        if(battery.battMsgPer == 0){
            if(volt <= BATT_VOLT_LOW){
                Menu_putMsg("Battery depleted", MSG_CNT_LONG);
            }else{
                Menu_putMsg("Low charge", MSG_CNT_LONG);
            }
            battery.battMsgPer = BATT_MSG_PERIOD;
        }
    }else{
        battery.battMsgPer = 0;
        //Calculate percentage
        for(i = BATT_CHG_ARR_PTS-1; i > 0; i--){
            if(volt >= battery.battVltPts[i]){
                perc = 100-i*10;
                j = i;
            }
        }
        tmp = battery.battVltPts[j-1] - battery.battVltPts[j];
        volt -= battery.battVltPts[j];
        tmp = (volt*BATT_CHG_VMPLY_COEFF)/tmp;
        if((tmp > BATT_ADD_PERC_MIN) && (tmp <= BATT_ADD_PERC_MAX)){
            perc += tmp;
        }
    }
    //Add hysteresis to indicated percentage
    if(perc > battery.battCharge){
        if((perc > (battery.battCharge + BATT_PERC_HYST)) || (percFlag == BATT_PERC_FLAG_INC)){
            percFlag = BATT_PERC_FLAG_INC;
            battery.battCharge = perc;
        }
    }else if(perc < battery.battCharge){
        if((perc < (battery.battCharge - BATT_PERC_HYST)) || (percFlag == BATT_PERC_FLAG_DEC)){
            percFlag = BATT_PERC_FLAG_DEC;
            battery.battCharge = perc;
        }
    }
    //Counters managing
    if(battery.battMsgPer != 0) battery.battMsgPer--;
    if((battery.battMsgPer == 0) && (battery.battVolt <= BATT_VOLT_LOW)) Menu_pwrSw(ePwrOff);
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
