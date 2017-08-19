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
    uint8_t i, j, x, y, pos, offs, numItems;
    //Draw the folder
    for(i=0; i<sizeof(menuFolder); i++){
        for (j=0; j<8; j++){
            if(menuFolder[i] & (1 << j)){
                ssd_setpix(x+i, y+j, WHITE);
            }else{
                ssd_setpix(x+i, y+j, BLACK);
            }
        }
    }
    if(menu.menuItems.totItems > MENU_POSITIONS){
        //Put the line
        x = SCROLL_LINE_X_OFF;
        y = SCROLL_Y_OFF;
        for(i = 0; i < SCROLL_LINE_LENGTH; i++){
            for(j = 0; j < SCROLL_LINE_WIDTH; j++){
                ssd_setpix((x + j), (y + i), WHITE);
            }
        }
        //Put scroll onto line
        x = SCROLL_X_OFF;
        offs = (menu.menuItems.currItem-1)*(SCROLL_LINE_LENGTH-8)/(menu.menuItems.totItems-1);   //Calculate scroll offset
        y = SCROLL_Y_OFF + offs;
        for(i=0; i<sizeof(menuScroll); i++){
            for (j=0; j<8; j++){
                if(menuScroll[i] & (1 << j)){
                    ssd_setpix(x+i, y+j, WHITE);
                }else{
                    ssd_setpix(x+i, y+j, BLACK);
                }
            }
        }
    }
    //Offset calculation
    offs = menu.menuItems.wndOffs;
    if(((menu.menuItems.currItem - MENU_POSITIONS) > offs) && (menu.menuItems.currItem > MENU_POSITIONS)){
        offs = menu.menuItems.currItem - MENU_POSITIONS;
    }else if(((menu.menuItems.currItem - MENU_POSITIONS) < offs) && ((offs - (menu.menuItems.currItem - MENU_POSITIONS)) > (MENU_POSITIONS-1))){
        offs--;
    }
    menu.menuItems.wndOffs = offs;
    //Calculate arrow position
    pos = menu.menuItems.currItem - offs;
    //Put strings
    ssd_putString6x8(14, 1, &menu.menuItems.parent[0]);
    ssd_putString6x8(0, (MENU_START+MENU_INTERVAL*(pos-1)), &arrow[0]);         //Arrow to current position
    if(menu.menuItems.totItems <= MENU_POSITIONS){
        numItems = menu.menuItems.totItems;
    }else{
        numItems = MENU_POSITIONS;
    }
    for(i = 0; i < numItems; i++){
        ssd_putString6x8(8, MENU_START+MENU_INTERVAL*i, &menu.menuItems.child[offs+i][0]);
    }
}

/*!****************************************************************************
* @brief    Fill video buffer with "main screen"
* @param    
* @retval   
*/
void drawHybrScr(void){
    char speed0[8], speed1[4], speed2[4], speed3[4], speed4[4], magStat[6], rollAng[8], pitchAng[8];
    uint16_t val1, val2, ptch1, ptch2, roll1, roll2;
    //Pelet signature
    ssd_putStrClr(0, 0, &pellets.pelStrings[meas.chron.pellet][0], 18, FONT_6X8);
    //Speed 0
    val1 = meas.chron.speed0/FRACT_HUNDREDTHS;
    val2 = meas.chron.speed0-(val1*FRACT_HUNDREDTHS);
    if(val1 == 0) val2 = meas.chron.speed0;
    sprintf(speed0, "%u.%.2u%c", val1, val2, 47);
    ssd_putStrClr(0, 24, &speed0[0], 7, FONT_12x16);
    //Speed 1
    sprintf(speed1, "%u", meas.chron.speed1/100);
    ssd_putStrClr(98, 15, &speed1[0], 3, FONT_6X8);
    //Speed 2
    sprintf(speed2, "%u", meas.chron.speed2/100);
    ssd_putStrClr(98, 25, &speed2[0], 3, FONT_6X8);
    //Speed 3
    sprintf(speed3, "%u", meas.chron.speed3/100);
    ssd_putStrClr(98, 35, &speed3[0], 3, FONT_6X8);
    //Magazine status/Speed4
    if(meas.chron.clipCapacity > 1){
        sprintf(magStat, "%u/%u", meas.chron.clipCurrent, meas.chron.clipCapacity);
        ssd_putStrClr(92, 45, &magStat[0], 5, FONT_6X8);
    }else{
        sprintf(speed4, "%u", meas.chron.speed4/100);
        ssd_putStrClr(98, 45, &speed4[0], 3, FONT_6X8);
    }
    //Roll angle
    val1 = abs(meas.accRoll) / 10;
    roll1 = val1 / FRACT_TENTHS;
    roll2 = val1 - (roll1 * FRACT_TENTHS);
    if(roll1 == 0) roll2 = val1;
    sprintf(rollAng, "%c%u.%.1u%c", 226, roll1, roll2, 248);
    ssd_putStrClr(0, 40, &rollAng[0], 6, FONT_6X8);
    //Put roll bar
    ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_LOW_Y, ROLL_LOW_HEIGHT);
    //Pitch angle (add clearing of previous level)
    val1 = abs(meas.accPitch) / 10;
    ptch1 = val1 / FRACT_TENTHS;
    ptch2 = val1 - (ptch1*FRACT_TENTHS);
    if(ptch1 == 0) ptch2 = val1;
    sprintf(pitchAng, "%c%u.%.1u%c", 232, ptch1, ptch2, 248);
    ssd_putStrClr(42, 40, &pitchAng[0], 6, FONT_6X8);
    //Put pitch bar
    ssd_putPitchBar(meas.accPitch, meas.accPitchBorder);
}

void drawChrScr(void){
    char speed0[8], speed1[4], speed2[4], speed3[4], speed4[4], magStat[6], energy[8], statShots[4], mean[8], sdev[8];
    uint16_t val1, val2;
    //Pelet signature
    ssd_putStrClr(0, 0, &pellets.pelStrings[meas.chron.pellet][0], 18, FONT_6X8);
    //Speed 0
    val1 = meas.chron.speed0/FRACT_HUNDREDTHS;
    val2 = meas.chron.speed0-(val1*FRACT_HUNDREDTHS);
    if(val1 == 0) val2 = meas.chron.speed0;
    sprintf(speed0, "%u.%.2u%c", val1, val2, 47);
    ssd_putStrClr(0, 24, &speed0[0], 7, FONT_12x16);
    //Speed 1
    sprintf(speed1, "%u", meas.chron.speed1/100);
    ssd_putStrClr(98, 15, &speed1[0], 3, FONT_6X8);
    //Speed 2
    sprintf(speed2, "%u", meas.chron.speed2/100);
    ssd_putStrClr(98, 25, &speed2[0], 3, FONT_6X8);
    //Speed 3
    sprintf(speed3, "%u", meas.chron.speed3/100);
    ssd_putStrClr(98, 35, &speed3[0], 3, FONT_6X8);
    //Magazine status/Speed4
    if(meas.chron.clipCapacity > 1){
        sprintf(magStat, "%u/%u", meas.chron.clipCurrent, meas.chron.clipCapacity);
        ssd_putStrClr(92, 45, &magStat[0], 5, FONT_6X8);
    }else{
        sprintf(speed4, "%u", meas.chron.speed4/100);
        ssd_putStrClr(98, 45, &speed4[0], 3, FONT_6X8);
    }
    //Number of shots per measurement
    sprintf(statShots, "%u", meas.chron.statShots);
    ssd_putStrClr(98, 55, &statShots[0], 3, FONT_6X8);
    //Mean of current measurement
    val1 = meas.chron.statMean/FRACT_HUNDREDTHS;
    val2 = meas.chron.statMean-(val1*FRACT_HUNDREDTHS);
    if(val1 == 0) val2 = meas.chron.statMean;
    sprintf(mean, "%c%u.%.2u", 230, val1, val2);
    ssd_putStrClr(0, 36, &mean[0], 7, FONT_6X8);
    //Standard deviation
    val1 = meas.chron.statSdev/FRACT_HUNDREDTHS;
    val2 = meas.chron.statSdev-(val1*FRACT_HUNDREDTHS);
    if(val1 == 0) val2 = meas.chron.statSdev;
    sprintf(sdev, "%c%u.%.2u", 229, val1, val2);
    ssd_putStrClr(49, 36, &sdev[0], 7, FONT_6X8);
    //Calculated energy
    val1 = meas.chron.statEnergy/FRACT_HUNDREDTHS;
    val2 = meas.chron.statEnergy-(val1*FRACT_HUNDREDTHS);
    sprintf(energy, "%u.%.2u%c", val1, val2, 58);
    if(val1 == 0) val2 = meas.chron.statEnergy;
    ssd_putStrClr(0, 56, &energy[0], 7, FONT_12x16);
}

void drawIncScr(void){
    char rollAng[8], pitchAng[8], border[4];
    uint16_t val1, ptch1, ptch2, roll1, roll2;
    //Roll angle
    val1 = abs(meas.accRoll) / 10;
    roll1 = val1 / FRACT_TENTHS;
    roll2 = val1 - (roll1 * FRACT_TENTHS);
    if(roll1 == 0) roll2 = val1;
    sprintf(rollAng, "%c%u.%.1u%c", 59, roll1, roll2, 61);
    ssd_putStrClr(0, 8, &rollAng[0], 6, FONT_12x16);
    //Put roll bar
    ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_HIGH_Y, ROLL_HIGH_HEIGHT);
    //Pitch angle
    val1 = abs(meas.accPitch) / 10;
    ptch1 = val1 / FRACT_TENTHS;
    ptch2 = val1 - (ptch1*FRACT_TENTHS);
    if(ptch1 == 0) ptch2 = val1;
    sprintf(pitchAng, "%c%u.%.1u%c", 232, ptch1, ptch2, 248);
    ssd_putStrClr(76, 0, &pitchAng[0], 6, FONT_6X8);
    //Roll border
    sprintf(border, "%c%u%c", 241, meas.accRollBorder, 248);                //Roll border
    ssd_putString6x8(76, 9, &border[0]);
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
