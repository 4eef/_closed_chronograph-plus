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
menuItem_type               *currMenuItem;

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
    for(i = 1; i < PELLET_DB_QTY; i++){
        sprintf(pellets.pelStrings[i], "Default pellet %u", i);
    }
    //Screen modes names
    strcpy(sysPars.modeTxts[0], "Hybrid");
    strcpy(sysPars.modeTxts[1], "Chronograph");
    strcpy(sysPars.modeTxts[2], "Inclinometer");
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
    Menu_Navigate(&mode);
    //Initialize hardware
    initPeriphs();
    Menu_pwrSw(ePwrOff);
    //Debug
//    adcInit();
//    Menu_pwrOnOff(ePwrOn);
    
    while(1){
        currMenuItem = Menu_GetCurrentMenu();
        sync();                                                                 //Syncronize cycle
        //meas.chron.speed0 = meas.stats.cycBroken;                               //Debug
        battCalc();                                                             //Calculate battery parameters
        Menu_run(getNavEvent());                                                //Run menu processor eWait);//
        if(power.pwrMode == ePwrRun){
            hndlIRData();                                                       //Data received via IR channel
            if(sysPars.dispMode != eChronograph) trxAccData();                  //Perform data transmition with accelerometer
            drawDisplay();                                                      //Perform display refresh
        }
    }
}

/*!****************************************************************************
* @brief    Redirect routine to edit text string
*/
//void txtEditInit(void){
//    char title[20];
//    strcpy(title, "Edit name...");
//    if(currMenuItem == &spdbp1){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[1], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp2){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[2], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp3){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[3], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp4){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[4], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp5){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[5], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp6){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[6], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp7){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[7], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp8){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[8], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp9){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[9], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp10){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[10], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp11){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[11], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp12){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[12], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp13){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[13], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp14){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[14], PELLET_DB_STR_LEN - 1);
//    }else if(currMenuItem == &spdbp15){
//        Menu_putTxtEditWnd(title, pellets.pelStrings[15], PELLET_DB_STR_LEN - 1);
//    }
//}

/*!****************************************************************************
* @brief    Redirect routine to edit required parameter
*/
//void parEditInit(void){
//    menu.parEditWnd.parFract = eNoFract;
//    if(currMenuItem == &sclipc){                                             //Magazine capacity
//        Menu_putParWnd("Set capacity...", "pcs", eNoFract, &meas.chron.clipCapacity, &meas.chron.clipCurrent, PELLET_MAX, PELLET_MIN);
//    }else if(currMenuItem == &sibrdr){                                       //Inclinometer roll level border
//        Menu_putParWnd("Set border...", "deg", eNoFract, &meas.accRollBorder, NULL, INC_BORDER_MAX, INC_BORDER_MIN);
//    }else if(currMenuItem == &scbind){                                       //Bind new chronograph
//        Menu_putMessage("Waiting...", MSG_CNT_BIND);
//        meas.chron.chrBindCnt = MSG_CNT_BIND;
//    }else if(currMenuItem == &scdist){                                       //Edit sensor distance
//        Menu_putParWnd("Set distance...", "mm", eTenths, &meas.chron.sensDist, NULL, CHR_DIST_MAX, CHR_DIST_MIN);
//    }else if(currMenuItem == &siofcal){
//        
//    }else if(currMenuItem == &sigacal){
//        accGainCal();
//    }else if(currMenuItem == &spofft){
//        Menu_putParWnd("Set timer...", "min", eNoFract, &power.uptimeSet, NULL, POWER_RUN_MAX, POWER_RUN_MIN);
//        power.uptimeCurr = 0;
//    }else if(currMenuItem == &sswrst){
//        
//    }
//}

/*!****************************************************************************
* @brief    Menu parameters routines
*/
//void modeEdit(void){
//    if(currMenuItem == &mcommon) sysPars.dispMode = eHybrid;
//    else if(currMenuItem == &mchron) sysPars.dispMode = eChronograph;
//    else if(currMenuItem == &mincline) sysPars.dispMode = eInclinometer;
//    Menu_putMessage("OK", MSG_CNT);
//    menu.menuMode = eDisplay;
//}

/*!****************************************************************************
* @brief    Redirect routine to edit required parameter
*/
void drawDisplay(void){
    ssd_clearVidBff();
    ssd_putBatt(meas.battery.battCharge, meas.battery.battChgStat);
    switch(menu.menuMode){
        case eDisplay:
            switch(sysPars.dispMode){
                case eHybrid:
                    drawHybrScr();
                    break;
                case eChronograph:
                    drawChrScr();
                    break;
                case eInclinometer:
                    drawIncScr();
                    break;
                default:
                    break;
            }
            break;
        case eMenu:
            drawMenu();
            break;
        case eChooseFrmLstWnd:
            ssd_putTxtParSelWnd();
            break;
        case eParEditWnd:
            ssd_putParWnd();
            break;
        case eTxtEditWnd:
            ssd_putTxtEditWnd();
            break;
        case eInfoWnd:
            break;
        default:
            break;
    }
    //Draw parameter edit box
//    if(menu.menuMode == eParEditWnd){
//        if(lis3AxisCal.calState != ACCEL_CAL_READY){
//            switch(lis3AxisCal.calAxisState){
//            case ACCEL_X_CAL_SAVE_NEG:
//                strcpy(text, "1/2 X negative...");
//                sprintf(dir, "%c", 25);
//                break;
//            case ACCEL_X_CAL_SAVE_POS:
//                strcpy(text, "2/2 X positive...");
//                sprintf(dir, "%c", 24);       
//                break;
//            case ACCEL_Y_CAL_SAVE_NEG:
//                strcpy(text, "1/2 Y negative...");
//                sprintf(dir, "%c", 27);
//                break;
//            case ACCEL_Y_CAL_SAVE_POS:
//                strcpy(text, "2/2 Y positive...");
//                sprintf(dir, "%c", 26);
//                break;
//            case ACCEL_Z_CAL_SAVE_NEG:
//                strcpy(text, "1/2 Z negative...");
//                sprintf(dir, "%c", 158);
//                break;
//            case ACCEL_Z_CAL_SAVE_POS:
//                strcpy(text, "2/2 Z positive...");
//                sprintf(dir, "%c", 4);
//                break;
//            default:
//                break;
//            }
//            sprintf(axis1, "X:%d", lis3AxisCal.calAxisX);
//            sprintf(axis2, "Y:%d", lis3AxisCal.calAxisY);
//            sprintf(axis3, "Z:%d", lis3AxisCal.calAxisZ);
//            ssd_putParBox(&text[0], PAR_BOX_ARROWS_DIS);
//            //Axis raws
//            len = strlen(axis1);
//            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
//            ssd_putString6x8(offs, 22, &axis1[0]);
//            len = strlen(axis2);
//            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
//            ssd_putString6x8(offs, 32, &axis2[0]);
//            len = strlen(axis3);
//            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
//            ssd_putString6x8(offs, 42, &axis3[0]);
//            //Direction arrows
//            ssd_putString6x8(20, 28, &dir[0]);
//            ssd_putString6x8(101, 28, &dir[0]);
//        }else{
//            strcpy(text, currMenuItem->Text);
//            ssd_putParBox(&text[0], PAR_BOX_ARROWS_EN);
//            if(menu.parEditWnd.parFract != eNoFract){
//                val1 = menu.parEditWnd.parValue/menu.parEditWnd.parFract;
//                val2 = menu.parEditWnd.parValue%val1;
//                sprintf(par, "%u.%u", val1, val2);
//            }else{
//                sprintf(par, "%u", menu.parEditWnd.parValue);
//            }
//            len = strlen(par);
//            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
//            ssd_putString6x8(offs, 28, &par[0]);
//        }
//    }else
    if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
        pellets.pelStat = PELLET_OK;
        Menu_putTxtParSelWnd(pellets.pelStrings[1], &meas.chron.pellet, PELLET_KNOWN_LIST, LIST_STR_LEN);
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

//        switch(getButtonState()){
//        case eUp:
//            if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
//                if(pellets.matchedSgnNum >= PELLET_DB_NUM-1){
//                    pellets.matchedSgnNum = PELLET_DB_NULL;
//                }else{
//                    pellets.matchedSgnNum++;
//                }
//            }
//            if(power.pwrMode == ePwrRun){
//                menu.navEvent = eUp;
//            }
//            break;
//        case eDown:
//            if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
//                if(pellets.matchedSgnNum <= PELLET_DB_NULL){
//                    pellets.matchedSgnNum = PELLET_DB_NUM-1;
//                }else{
//                    pellets.matchedSgnNum--;
//                }
//            }
//            if(power.pwrMode == ePwrRun){
//                menu.navEvent = eDown;
//            }
//            break;
//        case eOk:
//            if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
//                Menu_putMessage("Saved", MSG_CNT);
//                if(menu.menuMode == eParEditWnd){
//                    if(lis3AxisCal.calState == ACCEL_CAL_WAIT){
//                        lis3AxisCal.calState = ACCEL_CAL_SAVE;
//                    }
//                }else{
//                    pellets.pelSgntrs[pellets.matchedSgnNum] = pellets.newSgn;
//                    meas.chron.pellet = pellets.matchedSgnNum;
//                    pellets.pelStat = PELLET_OK;
//                }
//            }
//            if(power.pwrMode == ePwrRun){
//                menu.navEvent = eOk;
//            }
//            break;
//        case eOkLng:
//            if(power.pwrMode == ePwrRun){
//                menu.navEvent = eOkLng;
//            }
//            break;
//        case eBack:
//            if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
//                meas.chron.chrBindCnt = 0;
//                Menu_putMessage("Cancelled", MSG_CNT);
//                if(menu.menuMode == eParEditWnd){
//                    if(lis3AxisCal.calState == ACCEL_CAL_WAIT){
//                        lis3AxisCal.calState = ACCEL_CAL_CANCEL;
//                    }
//                }else{
//                    meas.chron.pellet = 0;
//                    pellets.matchedSgnNum = 0;
//                    pellets.pelStat = PELLET_OK;
//                }
//            }
//            if(power.pwrMode == ePwrRun){
//                menu.navEvent = eBack;
//            }
//            break;
//        case eBackLng:
//            if((menu.menuMode == eDisplay) && (power.pwrMode == ePwrRun)){
//                if((meas.chron.clipCapacity > 1) && (meas.chron.clipCurrent != meas.chron.clipCapacity)){
//                    meas.chron.clipCurrent = meas.chron.clipCapacity;
//                    Menu_putMessage("Clip reloaded", MSG_CNT);
//                }else if((meas.chron.statShots != 0) && (sysPars.dispMode == eChronograph)){
//                    meas.chron.statShots = 0;
//                    meas.chron.statSpeedsSum = 0;
//                    meas.chron.statSdev = 0;
//                    meas.chron.statMean = 0;
//                    Menu_putMessage("Stats cleared", MSG_CNT);
//                }else{
//                    powerOff();
//                }
//            }else if(power.pwrMode == ePwrStop){
//                powerOn();
//            }else
//            if(power.pwrMode == ePwrRun){
//                menu.navEvent = eBackLng;
//            }
//            break;
//        default:
//            menu.navEvent = eWait;
//            break;
//        }

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
