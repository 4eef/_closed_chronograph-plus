/*!****************************************************************************
* @file    system.c
* @author  4eef
* @version V1.0
* @date    25.02.2017, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "system.h"

/*!****************************************************************************
* MEMORY
*/
extern ssdSettings_type     ssdSettings;
extern menu_type            menu;
meas_type                   meas;
sysPars_type                sysPars;
pellets_type                pellets;


/*!****************************************************************************
* @brief    Fill video buffer with "menu"
* @param    
* @retval   
*/
void drawMenu(void){
    char arrow[2] = {26, 0};
    uint8_t i, pos, offs;
    if(ssdSettings.status == DISPLAY_CLEAR){
        ssd_putMenuFolder();                                                        //Folder for parent position
        if(menu.totItems > MENU_POSITIONS) ssd_putMenuScroll();                     //Put scroll bar
        //Offset calculation
        offs = menu.offs;
        if(((menu.currItem - MENU_POSITIONS) > offs) && (menu.currItem > MENU_POSITIONS)){
            offs = menu.currItem - MENU_POSITIONS;
        }else if(((menu.currItem - MENU_POSITIONS) < offs) && ((offs - (menu.currItem - MENU_POSITIONS)) > (MENU_POSITIONS-1))){
            offs--;
        }
        menu.offs = offs;
        //Calculate arrow position
        pos = menu.currItem - offs;
        //Put strings
        ssd_putString6x8(14, 1, &menu.parent[0]);
        ssd_putString6x8(0, (MENU_START+MENU_INTERVAL*(pos-1)), &arrow[0]);         //Arrow to current position
        for(i = 0; i < MENU_POSITIONS; i++){
            ssd_putString6x8(8, MENU_START+MENU_INTERVAL*i, &menu.child[offs+i][0]);
        }
        ssdSettings.status = DISPLAY_REFRESH;
    }
}

/*!****************************************************************************
* @brief    Fill video buffer with "main screen"
* @param    
* @retval   
*/
void drawHybrScr(void){
    static uint16_t prvSpd0, prvSpd1, prvSpd2, prvSpd3, prvSpd4, prvMagStat, prvRoll, prvPitch;
    static uint8_t prvSgnNum;
    char speed0[8], speed1[4], speed2[4], speed3[4], speed4[4], magStat[6], rollAng[8], pitchAng[8];
    uint16_t val1, val2, ptch1, ptch2, roll1, roll2;
    //Pelet signature
    if((prvSgnNum != meas.chron.pellet) || (ssdSettings.status != DISPLAY_OK)){
        prvSgnNum = meas.chron.pellet;
        ssdSettings.status = DISPLAY_REFRESH;
        ssd_putStrClr(0, 0, &pellets.pelStrings[meas.chron.pellet][0], 18, FONT_6X8);
    }
    //Speed 0
    if((prvSpd0 != meas.chron.speed0) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd0 = meas.chron.speed0;
        ssdSettings.status = DISPLAY_REFRESH;
        val1 = meas.chron.speed0/FRACT_HUNDREDTHS;
        val2 = meas.chron.speed0-(val1*FRACT_HUNDREDTHS);
        if(val1 == 0) val2 = meas.chron.speed0;
        sprintf(speed0, "%u.%.2u%c", val1, val2, 47);
        ssd_putStrClr(0, 24, &speed0[0], 7, FONT_12x16);
    }
    //Speed 1
    if((prvSpd1 != meas.chron.speed1) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd1 = meas.chron.speed1;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(speed1, "%u", meas.chron.speed1/100);
        ssd_putStrClr(98, 15, &speed1[0], 3, FONT_6X8);
    }
    //Speed 2
    if((prvSpd2 != meas.chron.speed2) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd2 = meas.chron.speed2;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(speed2, "%u", meas.chron.speed2/100);
        ssd_putStrClr(98, 25, &speed2[0], 3, FONT_6X8);
    }
    //Speed 3
    if((prvSpd3 != meas.chron.speed3) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd3 = meas.chron.speed3;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(speed3, "%u", meas.chron.speed3/100);
        ssd_putStrClr(98, 35, &speed3[0], 3, FONT_6X8);
    }
    //Magazine status/Speed4
    if(meas.chron.clipCapacity > 1){
        if((prvMagStat != meas.chron.clipCurrent) || (ssdSettings.status != DISPLAY_OK)){
            prvMagStat = meas.chron.clipCurrent;
            ssdSettings.status = DISPLAY_REFRESH;
            sprintf(magStat, "%u/%u", meas.chron.clipCurrent, meas.chron.clipCapacity);
            ssd_putStrClr(92, 45, &magStat[0], 5, FONT_6X8);
        }
    }else if((prvSpd4 != meas.chron.speed4) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd4 = meas.chron.speed4;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(speed4, "%u", meas.chron.speed4/100);
        ssd_putStrClr(98, 45, &speed4[0], 3, FONT_6X8);
    }
    //Roll angle
    val1 = abs(meas.accRoll) / 10;
    if((prvRoll != val1) || (ssdSettings.status != DISPLAY_OK)){
        prvRoll = val1;
        ssdSettings.status = DISPLAY_REFRESH;
        roll1 = val1 / FRACT_TENTHS;
        roll2 = val1 - (roll1 * FRACT_TENTHS);
        if(roll1 == 0) roll2 = val1;
        sprintf(rollAng, "%c%u.%.1u%c", 226, roll1, roll2, 248);
        ssd_putStrClr(0, 40, &rollAng[0], 6, FONT_6X8);
    }
    //Put roll bar
    ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_LOW_Y, ROLL_LOW_HEIGHT);
    //Pitch angle (add clearing of previous level)
    val1 = abs(meas.accPitch) / 10;
    if((prvPitch != val1) || (ssdSettings.status != DISPLAY_OK)){
        prvPitch = val1;
        ssdSettings.status = DISPLAY_REFRESH;
        ptch1 = val1 / FRACT_TENTHS;
        ptch2 = val1 - (ptch1*FRACT_TENTHS);
        if(ptch1 == 0) ptch2 = val1;
        sprintf(pitchAng, "%c%u.%.1u%c", 232, ptch1, ptch2, 248);
        ssd_putStrClr(42, 40, &pitchAng[0], 6, FONT_6X8);
    }
    //Put pitch bar
    ssd_putPitchBar(meas.accPitch, meas.accPitchBorder);
}

void drawChrScr(void){
    static uint16_t prvSpd0, prvSpd1, prvSpd2, prvSpd3, prvSpd4, prvMagStat, prvEnrg, prvStatShots, prvMean, prvSDev;
    static uint8_t prvSgnNum;
    char speed0[8], speed1[4], speed2[4], speed3[4], speed4[4], magStat[6], energy[8], statShots[4], mean[8], sdev[8];
    uint16_t val1, val2;
    //Pelet signature
    if((prvSgnNum != meas.chron.pellet) || (ssdSettings.status != DISPLAY_OK)){
        prvSgnNum = meas.chron.pellet;
        ssdSettings.status = DISPLAY_REFRESH;
        ssd_putStrClr(0, 0, &pellets.pelStrings[meas.chron.pellet][0], 18, FONT_6X8);
    }
    //Speed 0
    if((prvSpd0 != meas.chron.speed0) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd0 = meas.chron.speed0;
        ssdSettings.status = DISPLAY_REFRESH;
        val1 = meas.chron.speed0/FRACT_HUNDREDTHS;
        val2 = meas.chron.speed0-(val1*FRACT_HUNDREDTHS);
        if(val1 == 0) val2 = meas.chron.speed0;
        sprintf(speed0, "%u.%.2u%c", val1, val2, 47);
        ssd_putStrClr(0, 24, &speed0[0], 7, FONT_12x16);
    }
    //Speed 1
    if((prvSpd1 != meas.chron.speed1) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd1 = meas.chron.speed1;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(speed1, "%u", meas.chron.speed1/100);
        ssd_putStrClr(98, 15, &speed1[0], 3, FONT_6X8);
    }
    //Speed 2
    if((prvSpd2 != meas.chron.speed2) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd2 = meas.chron.speed2;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(speed2, "%u", meas.chron.speed2/100);
        ssd_putStrClr(98, 25, &speed2[0], 3, FONT_6X8);
    }
    //Speed 3
    if((prvSpd3 != meas.chron.speed3) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd3 = meas.chron.speed3;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(speed3, "%u", meas.chron.speed3/100);
        ssd_putStrClr(98, 35, &speed3[0], 3, FONT_6X8);
    }
    //Magazine status/Speed4
    if(meas.chron.clipCapacity > 1){
        if((prvMagStat != meas.chron.clipCurrent) || (ssdSettings.status != DISPLAY_OK)){
            prvMagStat = meas.chron.clipCurrent;
            ssdSettings.status = DISPLAY_REFRESH;
            sprintf(magStat, "%u/%u", meas.chron.clipCurrent, meas.chron.clipCapacity);
            ssd_putStrClr(92, 45, &magStat[0], 5, FONT_6X8);
        }
    }else if((prvSpd4 != meas.chron.speed4) || (ssdSettings.status != DISPLAY_OK)){
        prvSpd4 = meas.chron.speed4;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(speed4, "%u", meas.chron.speed4/100);
        ssd_putStrClr(98, 45, &speed4[0], 3, FONT_6X8);
    }
    //Number of shots per measurement
    if((prvStatShots != meas.chron.statShots) || (ssdSettings.status != DISPLAY_OK)){
        prvStatShots = meas.chron.statShots;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(statShots, "%u", meas.chron.statShots);
        ssd_putStrClr(98, 55, &statShots[0], 3, FONT_6X8);
    }
    //Mean of current measurement
    if((prvMean != meas.chron.statMean) || (ssdSettings.status != DISPLAY_OK)){
        prvMean = meas.chron.statMean;
        ssdSettings.status = DISPLAY_REFRESH;
        val1 = meas.chron.statMean/FRACT_HUNDREDTHS;
        val2 = meas.chron.statMean-(val1*FRACT_HUNDREDTHS);
        if(val1 == 0) val2 = meas.chron.statMean;
        sprintf(mean, "%c%u.%.2u", 230, val1, val2);
        ssd_putStrClr(0, 36, &mean[0], 7, FONT_6X8);
    }
    //Standard deviation
    if((prvSDev != meas.chron.statSdev) || (ssdSettings.status != DISPLAY_OK)){
        prvSDev = meas.chron.statSdev;
        ssdSettings.status = DISPLAY_REFRESH;
        val1 = meas.chron.statSdev/FRACT_HUNDREDTHS;
        val2 = meas.chron.statSdev-(val1*FRACT_HUNDREDTHS);
        if(val1 == 0) val2 = meas.chron.statSdev;
        sprintf(sdev, "%c%u.%.2u", 229, val1, val2);
        ssd_putStrClr(49, 36, &sdev[0], 7, FONT_6X8);
    }
    //Calculated energy
    if((prvEnrg != meas.chron.statEnergy) || (ssdSettings.status != DISPLAY_OK)){
        prvEnrg = meas.chron.statEnergy;
        ssdSettings.status = DISPLAY_REFRESH;
        val1 = meas.chron.statEnergy/FRACT_HUNDREDTHS;
        val2 = meas.chron.statEnergy-(val1*FRACT_HUNDREDTHS);
        sprintf(energy, "%u.%.2u%c", val1, val2, 58);
        if(val1 == 0) val2 = meas.chron.statEnergy;
        ssd_putStrClr(0, 56, &energy[0], 7, FONT_12x16);
    }
}

void drawIncScr(void){
    static uint16_t prvRoll, prvPitch, prvBrdr;
    char rollAng[8], pitchAng[8], border[4];
    uint16_t val1, ptch1, ptch2, roll1, roll2;
    //Roll angle
    val1 = abs(meas.accRoll) / 10;
    if((prvRoll != val1) || (ssdSettings.status != DISPLAY_OK)){
        prvRoll = val1;
        ssdSettings.status = DISPLAY_REFRESH;
        roll1 = val1 / FRACT_TENTHS;
        roll2 = val1 - (roll1 * FRACT_TENTHS);
        if(roll1 == 0) roll2 = val1;
        sprintf(rollAng, "%c%u.%.1u%c", 59, roll1, roll2, 61);
        ssd_putStrClr(0, 8, &rollAng[0], 6, FONT_12x16);
    }
    //Put roll bar
    ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_HIGH_Y, ROLL_HIGH_HEIGHT);
    //Pitch angle
    val1 = abs(meas.accPitch) / 10;
    if((prvPitch != val1) || (ssdSettings.status != DISPLAY_OK)){
        prvPitch = val1;
        ssdSettings.status = DISPLAY_REFRESH;
        ptch1 = val1 / FRACT_TENTHS;
        ptch2 = val1 - (ptch1*FRACT_TENTHS);
        if(ptch1 == 0) ptch2 = val1;
        sprintf(pitchAng, "%c%u.%.1u%c", 232, ptch1, ptch2, 248);
        ssd_putStrClr(76, 0, &pitchAng[0], 6, FONT_6X8);
    }
    //Roll border
    if((prvBrdr != meas.accRollBorder) || (ssdSettings.status != DISPLAY_OK)){
        prvBrdr = meas.accRollBorder;
        ssdSettings.status = DISPLAY_REFRESH;
        sprintf(border, "%c%u%c", 241, meas.accRollBorder, 248);                //Roll border
        ssd_putString6x8(76, 9, &border[0]);
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
