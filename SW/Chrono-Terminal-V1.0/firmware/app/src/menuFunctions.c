/*!****************************************************************************
* @file    menuFunctions.c
* @author  4eef
* @version V1.0
* @date    23.10.2018, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "menuFunctions.h"

extern menu_type            menu;
extern chron_type           chron;
extern sysPars_type         sysPars;

const char statsTxt[] = "London is the capital and most populous city of England and the United Kingdom. Standing on the River Thames in the southeast of the island of Great Britain, London has been a major settlement for two millennia.";

void statsParser(void){
    strcpy(menu.infoWindow.text, "KyKyPy3HocTb - eTo o6IIIa9I 4epTa MeH9I\nu Moeu JI/-O6uMou AJIuHKu :)");
}

void aboutParser(void){
    
}

void sinfoParser(void){
    
}

void swResFunc(void){
    
}

void chrBindFunc(void){
    
}

void offsCalFunc(void){
    
}

void gainCalFunc(void){
    
}

void chrSetsRst(void){
    if((sysPars.dispMode == eChronograph) && (chron.statShots != 0)){
        chron.statShots = 0;
        chron.statSpeedsSum = 0;
        chron.statSdev = 0;
        chron.statMean = 0;
        Menu_putMsg("Stats cleared", MSG_CNT_DEFAULT);
    }
}

void clipReload(void){
    if(((sysPars.dispMode == eHybrid) || (sysPars.dispMode == eChronograph)) && (chron.clipCapacity > 1) && (chron.clipCurrent != chron.clipCapacity)){
        chron.clipCurrent = chron.clipCapacity;
        Menu_putMsg("Clip reloaded", MSG_CNT_DEFAULT);
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
