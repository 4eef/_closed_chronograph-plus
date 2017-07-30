/*!****************************************************************************
* @file    main.c
* @author  4eef
* @version V1.0
* @date    14.08.2016
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "main.h"

/*!****************************************************************************
* MEMORY
*/
extern menu_type            menu;
extern adcData_type         adcData;
extern lis3AxisCal_type     lis3AxisCal;
extern power_type           power;
extern buttons_type         buttons;
extern meas_type            meas;
extern sysPars_type         sysPars;
extern pellets_type         pellets;
extern kalman_type          kalman;
Menu_Item_t                 *currMenuItem;

//Menu implementation
//        Name          Next            Previous        Parent          Child           SelectFunc      EnterFunc       Name                    Window title
//Main screen
MENU_ITEM(display,      NULL_MENU,      NULL_MENU,      NULL_MENU,      mode,           NULL,           NULL,           "Menu"                  );
//Menu
MENU_ITEM(mode,         stats,          NULL_MENU,      display,        mcommon,        NULL,           NULL,           "Main screen"           );
MENU_ITEM(stats,        setts,          mode,           display,        NULL_MENU,      NULL,           NULL,           "Statistics"            );
MENU_ITEM(setts,        about,          stats,          display,        sclipc,         NULL,           NULL,           "Settings"              );
MENU_ITEM(about,        service,        setts,          display,        NULL_MENU,      NULL,           NULL,           "About"                 );
MENU_ITEM(service,      NULL_MENU,      about,          display,        NULL_MENU,      NULL,           NULL,           "Service info"          );
//Menu/Display mode
MENU_ITEM(mcommon,      mchron,         NULL_MENU,      mode,           NULL_MENU,      NULL,           modeEdit,       "Hybrid"                );
MENU_ITEM(mchron,       mincline,       mcommon,        mode,           NULL_MENU,      NULL,           modeEdit,       "Chronograph"           );
MENU_ITEM(mincline,     NULL_MENU,      mchron,         mode,           NULL_MENU,      NULL,           modeEdit,       "Inclinometer"          );
//Menu/Settings
MENU_ITEM(sclipc,       schrono,        NULL_MENU,      setts,          NULL_MENU,      NULL,           parEditRedir,   "Clip capacity"         );
MENU_ITEM(schrono,      sincline,       sclipc,         setts,          scdist,         NULL,           NULL,           "Chronograph"           );
MENU_ITEM(sincline,     spofft,         schrono,        setts,          sibrdr,         NULL,           NULL,           "Inclinometer"          );
MENU_ITEM(spofft,       sswrst,         sincline,       setts,          NULL_MENU,      NULL,           parEditRedir,   "Power off timer"       );
MENU_ITEM(sswrst,       NULL_MENU,      spofft,         setts,          NULL_MENU,      NULL,           parEditRedir,   "Software reset"        );
//Menu/Settings/Chronograph
MENU_ITEM(scdist,       scbind,         NULL_MENU,      schrono,        NULL_MENU,      NULL,           parEditRedir,   "Sensor distance"       );
MENU_ITEM(scbind,       NULL_MENU,      scdist,         schrono,        NULL_MENU,      NULL,           parEditRedir,   "Bind"                  );
//Menu/Settings/Inclinometer
MENU_ITEM(sibrdr,       siofcal,        NULL_MENU,      sincline,       NULL_MENU,      NULL,           parEditRedir,   "Roll graph border"     );
MENU_ITEM(siofcal,      sigacal,        sibrdr,         sincline,       NULL_MENU,      NULL,           parEditRedir,   "Offset calibration"    );
MENU_ITEM(sigacal,      NULL_MENU,      siofcal,        sincline,       NULL_MENU,      NULL,           parEditRedir,   "Gain calibration"      );

/*!****************************************************************************
* @brief    Main function
*/
void main(void){
    uint8_t i;
    //Battery discharge graph points (temporary)
    meas.battery.battVltPts[0] = 4150;
    meas.battery.battVltPts[1] = 4050;
    meas.battery.battVltPts[2] = 3970;
    meas.battery.battVltPts[3] = 3905;
    meas.battery.battVltPts[4] = 3865;
    meas.battery.battVltPts[5] = 3785;
    meas.battery.battVltPts[6] = 3765;
    meas.battery.battVltPts[7] = 3750;
    meas.battery.battVltPts[8] = 3741;
    meas.battery.battVltPts[9] = 3705;
    meas.battery.battVltPts[10] = 3595;
    //Pellets database primary config
    strcpy(pellets.pelStrings[0], "<unknown pellet>");
    for(i = 1; i < PELLET_DB_NUM; i++){
        sprintf(pellets.pelStrings[i], "Default pellet %u", i);
    }
    //Initial parameters
    meas.accRollBorder = 5;
    meas.accPitchBorder = 90;
    meas.chron.clipCapacity = PELLET_MIN;
    meas.chron.sensDist = CHR_DIST_DEFAULT;
    kalman.x.F = 0.5;
    kalman.x.covariance = 0.1;
    kalman.x.gain = 0.5;
    kalman.y.F = 0.5;
    kalman.y.covariance = 0.1;
    kalman.y.gain = 0.5;
    kalman.z.F = 0.5;
    kalman.z.covariance = 0.1;
    kalman.z.gain = 0.5;
    kalman.H = 1;
    kalman.Q = 2;
    kalman.R = 15;
    power.uptimeSet = POWER_RUN_DEFAULT;
    //Navigate to an absolute menu item entry
    Menu_Navigate(&display);
    //Initialize hardware
    initPeriphs();
    powerOff();
//    adcInit();
//    powerOn();    
    
    while(1){
        currMenuItem = Menu_GetCurrentMenu();
        //Syncronize cycle
        sync();
        //meas.chron.speed0 = meas.stats.cycBroken;
        battCalc();                                                             //Calculate battery parameters
        //MicroMenu navigation
        switch(getButtonState()){
        case UP:
            if(menu.menuMode == eParEditWnd){
                if(menu.parEditWnd.parValue >= menu.parEditWnd.parBorderMax){
                    menu.parEditWnd.parValue = menu.parEditWnd.parBorderMin;
                }else{
                    menu.parEditWnd.parValue++;
                }
            }else if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
                if(pellets.matchedSgnNum >= PELLET_DB_NUM-1){
                    pellets.matchedSgnNum = PELLET_DB_NULL;
                }else{
                    pellets.matchedSgnNum++;
                }
            }else if(power.mode == POWER_RUN){
                Menu_Navigate(MENU_PREVIOUS);
            }
            break;
        case DOWN:
            if(menu.menuMode == eParEditWnd){
                if(menu.parEditWnd.parValue <= menu.parEditWnd.parBorderMin){
                    menu.parEditWnd.parValue = menu.parEditWnd.parBorderMax;
                }else{
                    menu.parEditWnd.parValue--;
                }
            }else if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
                if(pellets.matchedSgnNum <= PELLET_DB_NULL){
                    pellets.matchedSgnNum = PELLET_DB_NUM-1;
                }else{
                    pellets.matchedSgnNum--;
                }
            }else if(power.mode == POWER_RUN){
                Menu_Navigate(MENU_NEXT);
            }
            break;
        case OK:
            if((menu.menuMode == eParEditWnd) || (pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
                Menu_putMessage("Saved", MSG_CNT);
                if(menu.menuMode == eParEditWnd){
                    if(lis3AxisCal.calState == ACCEL_CAL_WAIT){
                        lis3AxisCal.calState = ACCEL_CAL_SAVE;
                    }else{
                        menu.navEvent = eSave;
                    }
                }else{
                    pellets.pelSgntrs[pellets.matchedSgnNum] = pellets.newSgn;
                    meas.chron.pellet = pellets.matchedSgnNum;
                    pellets.pelStat = PELLET_OK;
                }
            }else if(power.mode == POWER_RUN){
                if((MENU_CHILD == &NULL_MENU) || (MENU_CHILD == NULL)){
                    Menu_EnterCurrentItem();
                }else{
                    Menu_Navigate(MENU_CHILD);
                }
            }
            break;
        case OKLNG:
            break;
        case CANCEL:
            if((menu.menuMode == eParEditWnd) || (pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
                sysPars.sysSettings.chrBind = 0;
                Menu_putMessage("Cancelled", MSG_CNT);
                if(menu.menuMode == eParEditWnd){
                    if(lis3AxisCal.calState == ACCEL_CAL_WAIT){
                        lis3AxisCal.calState = ACCEL_CAL_CANCEL;
                    }else{
                        menu.navEvent = eBack;
                    }
                }else{
                    meas.chron.pellet = 0;
                    pellets.matchedSgnNum = 0;
                    pellets.pelStat = PELLET_OK;
                }
            }else if(power.mode == POWER_RUN){
                Menu_Navigate(MENU_PARENT);
            }
            break;
        case CLLNG:
            if((Menu_GetCurrentMenu() == &display) && (power.mode == POWER_RUN)){
                if((sysPars.sysSettings.clipEn != 0) && (meas.chron.clipCurrent != meas.chron.clipCapacity)){
                    meas.chron.clipCurrent = meas.chron.clipCapacity;
                    Menu_putMessage("Clip reloaded", MSG_CNT);
                }else if((meas.chron.statShots != 0) && (sysPars.sysSettings.dispMode == MODE_CHR)){
                    meas.chron.statShots = 0;
                    meas.chron.statSpeedsSum = 0;
                    meas.chron.statSdev = 0;
                    meas.chron.statMean = 0;
                    Menu_putMessage("Stats cleared", MSG_CNT);
                }else{
                    powerOff();
                }
            }else if(power.mode == POWER_STOP){
                powerOn();
            }else if(menu.menuMode == eDisplay){
                Menu_Navigate(&display);
            }
            break;
        default:
            menu.navEvent = eWait;
            break;
        }
        if(power.mode == POWER_RUN){
            hndlIRData();                                                       //Data received via IR channel
            if(sysPars.sysSettings.dispMode != MODE_CHR) trxAccData();          //Perform data transmition with accelerometer
            drawDisplay();                                                      //Perform display refresh
        }
    }
}

/*!****************************************************************************
* @brief    Redirect routine to edit required parameter
* @param    
* @retval   
*/
void parEditRedir(void){
    menu.parEditWnd.parFract = eNoFract;
    if(currMenuItem == &sclipc){                                             //Magazine capacity
        menu.menuMode = eParEditWnd;
        menu.parEditWnd.parBorderMax = PELLET_MAX;
        menu.parEditWnd.parBorderMin = PELLET_MIN;
        meas.chron.clipCapacity = parEdit(meas.chron.clipCapacity);
        meas.chron.clipCurrent = meas.chron.clipCapacity;
    }else if(currMenuItem == &sibrdr){                                       //Inclinometer roll level border
        menu.menuMode = eParEditWnd;
        menu.parEditWnd.parBorderMax = INC_BORDER_MAX;
        menu.parEditWnd.parBorderMin = INC_BORDER_MIN;
        meas.accRollBorder = parEdit(meas.accRollBorder);
    }else if(currMenuItem == &scbind){                                       //Bind new chronograph
        Menu_putMessage("Waiting...", MSG_CNT_BIND);
        meas.chron.chrBindCnt = MSG_CNT_BIND;
    }else if(currMenuItem == &scdist){                                       //Edit sensor distance
        menu.menuMode = eParEditWnd;
        menu.parEditWnd.parBorderMax = CHR_DIST_MAX;
        menu.parEditWnd.parBorderMin = CHR_DIST_MIN;
        meas.chron.sensDist = parEdit(meas.chron.sensDist);
        menu.parEditWnd.parFract = eTenths;
    }else if(currMenuItem == &siofcal){
        
    }else if(currMenuItem == &sigacal){
        accGainCal();
    }else if(currMenuItem == &spofft){
        menu.menuMode = eParEditWnd;
        menu.parEditWnd.parBorderMax = POWER_RUN_MAX;
        menu.parEditWnd.parBorderMin = POWER_RUN_MIN;
        power.uptimeSet = parEdit(power.uptimeSet);
        power.uptimeCurr = 0;
    }else if(currMenuItem == &sswrst){
        
    }
}

/*!****************************************************************************
* @brief    Menu parameters edit routine
* @param    
* @retval   
*/
int16_t parEdit(int16_t param){
    int16_t retVal = param;
    if(menu.menuMode == eParEditWnd){
        if(menu.navEvent == eSave){
            retVal = menu.parEditWnd.parValue;
            menu.menuMode = eMenu;
            return retVal;
        }else if(menu.navEvent == eBack){
            menu.menuMode = eMenu;
            return retVal;
        }
    }else{
        menu.parEditWnd.parValue = param;
        menu.menuMode = eParEditWnd;
    }
    return retVal;
}

/*!****************************************************************************
* @brief    Menu parameters routines
* @param    
* @retval   
*/
void modeEdit(void){
    if(currMenuItem == &mcommon)sysPars.sysSettings.dispMode = MODE_COM;
    else if(currMenuItem == &mchron)sysPars.sysSettings.dispMode = MODE_CHR;
    else if(currMenuItem == &mincline)sysPars.sysSettings.dispMode = MODE_INC;
    Menu_putMessage("OK", MSG_CNT);
    if(currMenuItem->Parent == &mode) Menu_Navigate(&display);
}

/*!****************************************************************************
* @brief    Redirect routine to edit required parameter
* @param    
* @retval   
*/
void drawDisplay(void){
    uint32_t val1, val2;
    uint8_t offs, len;
    char text[20], par[6], axis1[10], axis2[10], axis3[10], dir[2];
    ssd_clearVidBff();
    ssd_putBatt(meas.battery.battCharge, meas.battery.battChgStat);
    if(Menu_GetCurrentMenu() == &display){
        switch(sysPars.sysSettings.dispMode){
        case MODE_COM:
            drawHybrScr();
            break;
        case MODE_CHR:
            drawChrScr();
            break;
        case MODE_INC:
            drawIncScr();
            break;
        default:
            //ssdSettings.status = DISPLAY_REFRESH;
            ssd_clearVidBff();
            break;
        }
    }else{
        drawMenu();
    }
    //Edit function
    if(menu.navEvent != eWait || lis3AxisCal.calState != ACCEL_CAL_READY) parEditRedir();
    //Draw parameter edit box
    if(menu.menuMode == eParEditWnd){
        if(lis3AxisCal.calState != ACCEL_CAL_READY){
            switch(lis3AxisCal.calAxisState){
            case ACCEL_X_CAL_SAVE_NEG:
                strcpy(text, "1/2 X negative...");
                sprintf(dir, "%c", 25);
                break;
            case ACCEL_X_CAL_SAVE_POS:
                strcpy(text, "2/2 X positive...");
                sprintf(dir, "%c", 24);       
                break;
            case ACCEL_Y_CAL_SAVE_NEG:
                strcpy(text, "1/2 Y negative...");
                sprintf(dir, "%c", 27);
                break;
            case ACCEL_Y_CAL_SAVE_POS:
                strcpy(text, "2/2 Y positive...");
                sprintf(dir, "%c", 26);
                break;
            case ACCEL_Z_CAL_SAVE_NEG:
                strcpy(text, "1/2 Z negative...");
                sprintf(dir, "%c", 158);
                break;
            case ACCEL_Z_CAL_SAVE_POS:
                strcpy(text, "2/2 Z positive...");
                sprintf(dir, "%c", 4);
                break;
            default:
                break;
            }
            sprintf(axis1, "X:%d", lis3AxisCal.calAxisX);
            sprintf(axis2, "Y:%d", lis3AxisCal.calAxisY);
            sprintf(axis3, "Z:%d", lis3AxisCal.calAxisZ);
            ssd_putParBox(&text[0], PAR_BOX_ARROWS_DIS);
            //Axis raws
            len = strlen(axis1);
            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
            ssd_putString6x8(offs, 22, &axis1[0]);
            len = strlen(axis2);
            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
            ssd_putString6x8(offs, 32, &axis2[0]);
            len = strlen(axis3);
            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
            ssd_putString6x8(offs, 42, &axis3[0]);
            //Direction arrows
            ssd_putString6x8(20, 28, &dir[0]);
            ssd_putString6x8(101, 28, &dir[0]);
        }else{
            strcpy(text, currMenuItem->Text);
            ssd_putParBox(&text[0], PAR_BOX_ARROWS_EN);
            if(menu.parEditWnd.parFract != eNoFract){
                val1 = menu.parEditWnd.parValue/menu.parEditWnd.parFract;
                val2 = menu.parEditWnd.parValue%val1;
                sprintf(par, "%u.%u", val1, val2);
            }else{
                sprintf(par, "%u", menu.parEditWnd.parValue);
            }
            len = strlen(par);
            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
            ssd_putString6x8(offs, 28, &par[0]);
        }
    }else if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
        strcpy(text, "Choose pellet...");
        ssd_putParBox(&text[0], PAR_BOX_ARROWS_EN);
        sprintf(text, pellets.pelStrings[pellets.matchedSgnNum]);
        len = strlen(text);
        offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
        ssd_putString6x8(offs, 28, &text[0]);
    }
    //Chronograph binding
    if(meas.chron.chrBindCnt != 0){
        meas.chron.chrBindCnt--;
        if(meas.chron.chrBindCnt == 0) Menu_putMessage("Failed", MSG_CNT);
    }
    //Draw message
    ssd_putMessage();
    //Refresh GDDRAM
    ug2864_refresh();
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
