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
extern ssdVideoBff_type     ssdVideoBff;
extern accel_type           accel;
extern menu_type            menu;
extern kalman_type          kalman;
extern lpfPrim_type         lpfPrim;
extern IRRXData_type        IRRXData;
extern adcData_type         adcData;
extern ssdSettings_type     ssdSettings;
extern lis3AxisCal_type     lis3AxisCal;
extern power_type           power;
extern buttons_type         buttons;
extern meas_type            meas;
extern sysPars_type         sysPars;
extern pellets_type         pellets;
Menu_Item_t                 *CurrentMenuItem;

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
* @param    
* @retval   
*/
void main(void){
    uint32_t dist, sgn, spd0, spd1, spd2, spd3, spd4, val1, val2, val3, val4;
    uint8_t i, offs, len;
    char text[20], par[6], axis1[10], axis2[10], axis3[10], dir[2];
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
    meas.chron.clipCapacity = PELLETS_MIN;
    meas.chron.sensDist = CHR_DIST_DEFAULT;
    //ssdSettings.enable = DISPLAY_ENABLE;
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
        //Syncronize cycle
        sync();
//        meas.chron.speed0 = meas.stats.cycBroken;
        battCalc();                                                             //Calculate battery parameters
        CurrentMenuItem = Menu_GetCurrentMenu();
        //MicroMenu navigation
        switch(getButtonState()){
        case UP:
            if(menu.parEdit == PAR_EDIT_ENABLE){
                if(menu.parValue >= menu.parBorderMax){
                    menu.parValue = menu.parBorderMin;
                }else{
                    menu.parValue++;
                }
            }else if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
                if(pellets.matchedSgnNum >= PELLET_DB_NUM-1){
                    pellets.matchedSgnNum = PELLET_DB_NULL;
                }else{
                    pellets.matchedSgnNum++;
                }
            }else if(power.mode == POWER_RUN){
                Menu_Navigate(MENU_PREVIOUS);
                ssdSettings.status = DISPLAY_CLEAR;
            }
            break;
        case DOWN:
            if(menu.parEdit == PAR_EDIT_ENABLE){
                if(menu.parValue <= menu.parBorderMin){
                    menu.parValue = menu.parBorderMax;
                }else{
                    menu.parValue--;
                }
            }else if((pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
                if(pellets.matchedSgnNum <= PELLET_DB_NULL){
                    pellets.matchedSgnNum = PELLET_DB_NUM-1;
                }else{
                    pellets.matchedSgnNum--;
                }
            }else if(power.mode == POWER_RUN){
                Menu_Navigate(MENU_NEXT);
                ssdSettings.status = DISPLAY_CLEAR;
            }
            break;
        case OK:
            if((menu.parEdit == PAR_EDIT_ENABLE) || (pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
                ssd_putMessage("Saved", MSG_CNT);
                if(menu.parEdit == PAR_EDIT_ENABLE){
                    if(lis3AxisCal.calState == ACCEL_CAL_WAIT){
                        lis3AxisCal.calState = ACCEL_CAL_SAVE;
                    }else{
                        menu.parStat = PAR_SAVE;
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
                    ssdSettings.status = DISPLAY_CLEAR;
                }
            }
            break;
        case OKLNG:
            break;
        case CANCEL:
            if((menu.parEdit == PAR_EDIT_ENABLE) || (pellets.pelStat == PELLET_CONFIRM) || (pellets.pelStat == PELLET_NEW)){
                sysPars.sysSettings.chrBind = 0;
                ssd_putMessage("Cancelled", MSG_CNT);
                if(menu.parEdit == PAR_EDIT_ENABLE){
                    if(lis3AxisCal.calState == ACCEL_CAL_WAIT){
                        lis3AxisCal.calState = ACCEL_CAL_CANCEL;
                    }else{
                        menu.parStat = PAR_CANCEL;
                    }
                }else{
                    meas.chron.pellet = 0;
                    pellets.matchedSgnNum = 0;
                    pellets.pelStat = PELLET_OK;
                }
            }else if(power.mode == POWER_RUN){
                Menu_Navigate(MENU_PARENT);
                ssdSettings.status = DISPLAY_CLEAR;
            }
            break;
        case CLLNG:
            if((CurrentMenuItem == &display) && (power.mode == POWER_RUN)){
                if((sysPars.sysSettings.clipEn != 0) && (meas.chron.clipCurrent != meas.chron.clipCapacity)){
                    meas.chron.clipCurrent = meas.chron.clipCapacity;
                    ssd_putMessage("Clip reloaded", MSG_CNT);
                }else if((meas.chron.statShots != 0) && (sysPars.sysSettings.dispMode == MODE_CHR)){
                    meas.chron.statShots = 0;
                    meas.chron.statSpeedsSum = 0;
                    meas.chron.statSdev = 0;
                    meas.chron.statMean = 0;
                    ssd_putMessage("Stats cleared", MSG_CNT);
                }else{
                    powerOff();
                }
            }else if(power.mode == POWER_STOP){
                powerOn();
            }else if(menu.parEdit == PAR_EDIT_DISABLE){
                Menu_Navigate(&display);
                ssdSettings.status = DISPLAY_CLEAR;
            }
            break;
        default:
            menu.parStat = PAR_NONE;
            break;
        }
        if(power.mode == POWER_RUN){
            CurrentMenuItem = Menu_GetCurrentMenu();
            //Data received via IR channel
            if(IRRXData.rxState == IR_DATA_READY){
                sgn = (IRRXData.rxByte[IR_MAX_BYTES-1]) | (IRRXData.rxByte[IR_MAX_BYTES-2])<<8 | (IRRXData.rxByte[IR_MAX_BYTES-3])<<16 | (IRRXData.rxByte[IR_MAX_BYTES-4])<<24;
                if((sysPars.sysSettings.dispMode != MODE_INC) && ((meas.chron.chrSgntr == sgn) || (sysPars.sysSettings.chrBind != 0))){
                    power.uptimeCurr = 0;
                    if(sysPars.sysSettings.chrBind != 0){
                        sysPars.sysSettings.chrBind = 0;
                        meas.chron.chrSgntr = sgn;
                        ssd_putMessage("Binded", MSG_CNT);
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
                    //�alculate clip status
                    if(sysPars.sysSettings.clipEn != 0){
                        if(meas.chron.clipCurrent == 0){
                            meas.chron.clipCurrent = meas.chron.clipCapacity;
                            ssd_putMessage("Clip reloaded", MSG_CNT);
                        }else if(meas.chron.clipCurrent == 1){
                            ssd_putMessage("Replace clip", MSG_CNT);
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
                        }else{                                      //No existing pellet found
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
                                ssd_putMessage("Error pellet ID", MSG_CNT);
                                pellets.pelStat = PELLET_OK;
                                pellets.newSgnErrCnt = 0;
                                pellets.newSgnCnt = 0;
                                pellets.newSgnSum = 0;
                            }
                        }
                    }
                    //Statistics calculation
                    if(sysPars.sysSettings.dispMode == MODE_CHR){
                        if(meas.chron.statShots >= STAT_SHOTS_MAX){
                            ssd_putMessage("Buffer is full", MSG_CNT);
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
                IRRXData.rxState = IR_READY;                        //Release receiver
            }
            //Perform data transmition with accelerometer
            trxAccData();
            //Refresh video buffer
//            memset(ssdVideoBff.video, 0, sizeof(ssdVideoBff.video));
            ssd_putBatt(meas.battery.battCharge, meas.battery.battChgStat);
            //Draw screen
            if(ssdSettings.status == DISPLAY_CLEAR) memset(ssdVideoBff.video, 0, sizeof(ssdVideoBff.video));
            if(CurrentMenuItem == &display){
                drawMainScreen();
            }else{
                drawMenu();
            }
            ssdSettings.status = DISPLAY_OK;
            //Edit function
            if(menu.parStat != PAR_NONE || lis3AxisCal.calState != ACCEL_CAL_READY) parEditRedir();
            //Draw parameter edit box
            if(menu.parEdit == PAR_EDIT_ENABLE){
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
                    strcpy(text, CurrentMenuItem->Text);
                    ssd_putParBox(&text[0], PAR_BOX_ARROWS_EN);
                    if(menu.parFract != 0){
                        val1 = menu.parValue/menu.parFract;
                        val2 = menu.parValue%val1;
                        sprintf(par, "%u.%u", val1, val2);
                    }else{
                        sprintf(par, "%u", menu.parValue);
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
            //Draw message
            ssd_putMessage(NULL, NULL);
            if(menu.msgCnt != 0){
                menu.msgCnt--;
                if((sysPars.sysSettings.chrBind != 0) && (menu.msgCnt == 0)){
                    sysPars.sysSettings.chrBind = 0;
                    strcpy(menu.message, "Failed");
                    menu.msgCnt = MSG_CNT;
                }
                len = strlen(menu.message);
                ssd_putMsgBox(len);
                offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
                ssd_putString6x8(offs, 28, &menu.message[0]);
            }
            //Refresh GDDRAM
            //Add drawing flag if something on display is changed
            ug2864_refresh();
        }
    }
}

/*!****************************************************************************
* @brief    Redirect routine to edit required parameter
* @param    
* @retval   
*/
void parEditRedir(void){
    menu.parFract = FRACT_NOFRACT;
    if(CurrentMenuItem == &sclipc){                                             //Magazine capacity
        menu.parBorderMax = PELLETS_MAX;
        menu.parBorderMin = PELLETS_MIN;
        meas.chron.clipCapacity = parEdit(meas.chron.clipCapacity);
        meas.chron.clipCurrent = meas.chron.clipCapacity;
    }else if(CurrentMenuItem == &sibrdr){                                       //Inclinometer roll level border
        menu.parBorderMax = INC_BORDER_MAX;
        menu.parBorderMin = INC_BORDER_MIN;
        meas.accRollBorder = parEdit(meas.accRollBorder);
    }else if(CurrentMenuItem == &scbind){                                       //Bind new chronograph
        strcpy(menu.message, "Waiting...");
        menu.msgCnt = MSG_CNT_BIND;
        sysPars.sysSettings.chrBind = 1;
    }else if(CurrentMenuItem == &scdist){                                       //Edit sensor distance
        menu.parBorderMax = CHR_DIST_MAX;
        menu.parBorderMin = CHR_DIST_MIN;
        meas.chron.sensDist = parEdit(meas.chron.sensDist);
        menu.parFract = FRACT_TENTHS;
    }else if(CurrentMenuItem == &siofcal){
        
    }else if(CurrentMenuItem == &sigacal){
        accGainCal();
    }else if(CurrentMenuItem == &spofft){
        menu.parBorderMax = POWER_RUN_MAX;
        menu.parBorderMin = POWER_RUN_MIN;
        power.uptimeSet = parEdit(power.uptimeSet);
        power.uptimeCurr = 0;
    }else if(CurrentMenuItem == &sswrst){
        
    }
}

/*!****************************************************************************
* @brief    Menu parameters edit routine
* @param    
* @retval   
*/
int16_t parEdit(int16_t param){
    int16_t retVal = param;
    if(menu.parEdit == PAR_EDIT_ENABLE){
        if(menu.parStat == PAR_SAVE){
            retVal = menu.parValue;
            menu.parEdit = PAR_EDIT_DISABLE;
            return retVal;
        }else if(menu.parStat == PAR_CANCEL){
            menu.parEdit = PAR_EDIT_DISABLE;
            return retVal;
        }
    }else{
        menu.parValue = param;
        menu.parEdit = PAR_EDIT_ENABLE;
    }
    return retVal;
}

/*!****************************************************************************
* @brief    Menu parameters routines
* @param    
* @retval   
*/
void modeEdit(void){
    if(CurrentMenuItem == &mcommon)sysPars.sysSettings.dispMode = MODE_COM;
    else if(CurrentMenuItem == &mchron)sysPars.sysSettings.dispMode = MODE_CHR;
    else if(CurrentMenuItem == &mincline)sysPars.sysSettings.dispMode = MODE_INC;
    ssd_putMessage("OK", MSG_CNT);
    if(CurrentMenuItem->Parent == &mode) Menu_Navigate(&display);
}

/*!****************************************************************************
* @brief    Tilt angle calculation routine from Q16.16 fixed point format
* @param    X, Y, Z - values in radians (Q16.16)
* @retval   Angle in degrees (Q16.16)
*/
int32_t q16TiltCalc(int32_t X, int32_t Y, int32_t Z){
    int32_t tilt, Ysq, Zsq, sumSqYZ, div;
    //Prepare arguments
    Ysq = fix16_mul(Y, Y);
    Zsq = fix16_mul(Z, Z);
    sumSqYZ = fix16_add(Ysq, Zsq);
    //Divide-by-zero protection
    if(sumSqYZ == 0) sumSqYZ = 1;
    //Tilt calculation
    div = fix16_div(X, sumSqYZ);
    tilt = fix16_atan(div);
    return tilt;
}

/*!****************************************************************************
* @brief    Tilt angle calculation routine
* @param    X, Y, Z - normalized projections to axises
* @retval   Angle in degrees (float)
*/
float tiltAngCalc(float X, float Y, float Z){
    float tilt, Ysq, Zsq, sumSqYZ, div;
    //Prepare arguments
    Ysq = Y*Y;
    Zsq = Z*Z;
    sumSqYZ = Ysq + Zsq;
    //Divide-by-zero protection
    if(sumSqYZ == 0) sumSqYZ = 0.000000000000001;
    //Tilt calculation
    div = X/sumSqYZ;
    tilt = (atan(div))*degRad;
    return tilt;
}

/*!****************************************************************************
* @brief    Signed 16-bit variable normalizer
* @param    Signed 16-bit value
* @retval   Normalized (-1...1) float value
*/
float s16fNorm(int16_t val){
    return val/16383.;
}

/*!****************************************************************************
* @brief    Get tilt data from accelerometer readings
* @param    
* @retval   
*/
void trxAccData(void){
    uint8_t reg, numSamples = ACCEL_N_SAMPLES;
    int32_t X, Y, Z, tmpRoll, tmpPitch, tmpConv;
    //Settings
    lis3_write(0x10, accel.offsetX);                                            //Offsets
    lis3_write(0x11, accel.offsetY);
    lis3_write(0x12, accel.offsetZ);
    lis3_write(0x20, 0x87);                                                     //1600 Hz sample rate
    //Get the samples
    while(numSamples != 0){
        reg = lis3_read(0x27);                                                  //Check if data is ready
        if((reg & 0x3) != 0){
            lis3_getXYZ();                                                      //Get sampled data
            lis3AxisCal.calAxisX = accel.corrX = lpfAccPrim(&lpfPrim.x, ((int16_t)(accel.rawXL | (accel.rawXH << 8))));
            lis3AxisCal.calAxisY = accel.corrY = lpfAccPrim(&lpfPrim.y, ((int16_t)(accel.rawYL | (accel.rawYH << 8))));
            lis3AxisCal.calAxisZ = accel.corrZ = lpfAccPrim(&lpfPrim.z, ((int16_t)(accel.rawZL | (accel.rawZH << 8))));
            numSamples--;
        }
    }
    lis3_write(0x20, 0x00);                                                     //Off
    //Data filtering
    accel.corrX = kalmanAccCorr(&kalman.x, accel.corrX);
    accel.corrY = kalmanAccCorr(&kalman.y, accel.corrY);
    accel.corrZ = kalmanAccCorr(&kalman.z, accel.corrZ);
    //Axises scaling by calculated gain
    X = (accel.corrX * fix16_one) / accel.gainX;
    Y = (accel.corrY * fix16_one) / accel.gainY;
    Z = (accel.corrZ * fix16_one) / accel.gainZ;
    //Calculate values
//    if(X > fix16_one) X = fix16_one - 1; else if(X < -fix16_one) X = -fix16_one + 1;
//    if(Y > fix16_one) Y = fix16_one - 1; else if(Y < -fix16_one) Y = -fix16_one + 1;
//    if(Z > fix16_one) Z = fix16_one - 1; else if(Z < -fix16_one) Z = -fix16_one + 1;
//    X = fix16_asin(X);
//    Y = fix16_asin(Y);
//    Z = fix16_asin(Z);
//    X = fix16_sin(X);
//    Y = fix16_sin(Y);
//    Z = fix16_sin(Z);
    //Roll calculation
    tmpConv = Q16_RAD_DEG;
    tmpRoll = q16TiltCalc(Y, X, Z);
    tmpRoll = fix16_mul(tmpConv, tmpRoll);
    meas.accRoll = ((tmpRoll) * 100) >> 16;
    //Pitch calculation
    tmpPitch = q16TiltCalc(Z, X, Y);
    tmpPitch = fix16_mul(tmpConv, tmpPitch);
    meas.accPitch = ((tmpPitch) * 100) >> 16;
}

/*!****************************************************************************
* @brief    Fill video buffer with "menu"
* @param    
* @retval   
*/
void drawMenu(void){
    char arrow[2] = {26, 0};
    uint8_t i, pos, offs;
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
}

/*!****************************************************************************
* @brief    Fill video buffer with "main screen"
* @param    
* @retval   
*/
void drawMainScreen(void){
    static uint16_t prvSpd0, prvSpd1, prvSpd2, prvSpd3, prvSpd4, prvMagStat, prvRoll;
    static uint16_t prvPitch, prvEnrg, prvStatShots, prvMean, prvSDev, prvBrdr;
    char speed0[8], speed1[4], speed2[4], speed3[4], speed4[4], magStat[6], def[21];
    char rollAng[8], pitchAng[8], energy[8], statShots[4], mean[8], sdev[8], border[4];
    uint16_t val1, val2, ptch1, ptch2, roll1, roll2;
    //Display settings
    if(sysPars.sysSettings.dispMode == MODE_COM || sysPars.sysSettings.dispMode == MODE_CHR){
        ssd_putString6x8(0, 0, &pellets.pelStrings[meas.chron.pellet][0]);      //Detected pellet
        //Speed 0
        if((prvSpd0 != meas.chron.speed0) || (ssdSettings.status == DISPLAY_CLEAR)){
            prvSpd0 = meas.chron.speed0;
            val1 = meas.chron.speed0/FRACT_HUNDREDTHS;
            val2 = meas.chron.speed0-(val1*FRACT_HUNDREDTHS);
            if(val1 == 0) val2 = meas.chron.speed0;
            sprintf(speed0, "%u.%.2u%c", val1, val2, 47);
            ssd_putStrClr(0, 24, &speed0[0], 7, FONT_12x16);
        }
        //Speed 1
        if((prvSpd1 != meas.chron.speed1) || (ssdSettings.status == DISPLAY_CLEAR)){
            prvSpd1 = meas.chron.speed1;
            sprintf(speed1, "%u", meas.chron.speed1/100);
            ssd_putStrClr(98, 15, &speed1[0], 3, FONT_6X8);
        }
        //Speed 2
        if((prvSpd2 != meas.chron.speed2) || (ssdSettings.status == DISPLAY_CLEAR)){
            prvSpd2 = meas.chron.speed2;
            sprintf(speed2, "%u", meas.chron.speed2/100);
            ssd_putStrClr(98, 25, &speed2[0], 3, FONT_6X8);
        }
        //Speed 3
        if((prvSpd3 != meas.chron.speed3) || (ssdSettings.status == DISPLAY_CLEAR)){
            prvSpd3 = meas.chron.speed3;
            sprintf(speed3, "%u", meas.chron.speed3/100);
            ssd_putStrClr(98, 35, &speed3[0], 3, FONT_6X8);
        }
        //Magazine status/Speed4
        if((meas.chron.clipCapacity > 1) && (prvMagStat != meas.chron.clipCurrent) || (ssdSettings.status == DISPLAY_CLEAR)){
            prvMagStat = meas.chron.clipCurrent;
            sprintf(magStat, "%u/%u", meas.chron.clipCurrent, meas.chron.clipCapacity);
            ssd_putStrClr(92, 45, &magStat[0], 5, FONT_6X8);
        }else if((prvSpd4 != meas.chron.speed4) || (ssdSettings.status == DISPLAY_CLEAR)){
            prvSpd4 = meas.chron.speed4;
            sprintf(speed4, "%u", meas.chron.speed4/100);
            ssd_putStrClr(98, 45, &speed4[0], 3, FONT_6X8);
        }
        //Select the info to display
        if(sysPars.sysSettings.dispMode == MODE_COM){
            //Roll angle
            if(prvRoll != meas.accRoll){
                prvRoll = meas.accRoll;
                val1 = abs(meas.accRoll) / 10;
                roll1 = val1 / FRACT_TENTHS;
                roll2 = val1 - (roll1 * FRACT_TENTHS);
                if(roll1 == 0) roll2 = val1;
                sprintf(rollAng, "%c%u.%.1u%c", 226, roll1, roll2, 248);
                ssd_putStrClr(0, 40, &rollAng[0], 6, FONT_6X8);
                ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_LOW_Y, ROLL_LOW_HEIGHT);
            }
            //Pitch angle (add clearing of previous level)
            if(prvPitch != meas.accPitch){
                prvPitch = meas.accPitch;
                val1 = abs(meas.accPitch) / 10;
                ptch1 = val1 / FRACT_TENTHS;
                ptch2 = val1 - (ptch1*FRACT_TENTHS);
                if(ptch1 == 0) ptch2 = val1;
                sprintf(pitchAng, "%c%u.%.1u%c", 232, ptch1, ptch2, 248);
                ssd_putStrClr(42, 40, &pitchAng[0], 6, FONT_6X8);
                ssd_putPitchBar(meas.accPitch, meas.accPitchBorder);
            }
        }else{
            //Number of shots per measurement
            if((prvStatShots != meas.chron.statShots) || (ssdSettings.status == DISPLAY_CLEAR)){
                prvStatShots = meas.chron.statShots;
                sprintf(statShots, "%u", meas.chron.statShots);
                ssd_putStrClr(98, 55, &statShots[0], 3, FONT_6X8);
            }
            //Mean of current measurement
            if((prvMean != meas.chron.statMean) || (ssdSettings.status == DISPLAY_CLEAR)){
                prvMean = meas.chron.statMean;
                val1 = meas.chron.statMean/FRACT_HUNDREDTHS;
                val2 = meas.chron.statMean-(val1*FRACT_HUNDREDTHS);
                if(val1 == 0) val2 = meas.chron.statMean;
                sprintf(mean, "%c%u.%.2u", 230, val1, val2);
                ssd_putStrClr(0, 36, &mean[0], 7, FONT_6X8);
            }
            //Standard deviation
            if((prvSDev != meas.chron.statSdev) || (ssdSettings.status == DISPLAY_CLEAR)){
                prvSDev = meas.chron.statSdev;
                val1 = meas.chron.statSdev/FRACT_HUNDREDTHS;
                val2 = meas.chron.statSdev-(val1*FRACT_HUNDREDTHS);
                if(val1 == 0) val2 = meas.chron.statSdev;
                sprintf(sdev, "%c%u.%.2u", 229, val1, val2);
                ssd_putStrClr(49, 36, &sdev[0], 7, FONT_6X8);
            }
            //Calculated energy
            if((prvEnrg != meas.chron.statEnergy) || (ssdSettings.status == DISPLAY_CLEAR)){
                prvEnrg = meas.chron.statEnergy;
                val1 = meas.chron.statEnergy/FRACT_HUNDREDTHS;
                val2 = meas.chron.statEnergy-(val1*FRACT_HUNDREDTHS);
                sprintf(energy, "%u.%.2u%c", val1, val2, 58);
                if(val1 == 0) val2 = meas.chron.statEnergy;
                ssd_putStrClr(0, 56, &energy[0], 7, FONT_12x16);
            }
        }
    }else if(sysPars.sysSettings.dispMode == MODE_INC){
        //Roll angle
        if((prvRoll != meas.accRoll) || (ssdSettings.status == DISPLAY_CLEAR)){
            prvRoll = meas.accRoll;
            val1 = abs(meas.accRoll) / 10;
            roll1 = val1 / FRACT_TENTHS;
            roll2 = val1 - (roll1 * FRACT_TENTHS);
            if(roll1 == 0) roll2 = val1;
            sprintf(rollAng, "%c%u.%.1u%c", 59, roll1, roll2, 61);
            ssd_putStrClr(0, 8, &rollAng[0], 6, FONT_12x16);
            ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_HIGH_Y, ROLL_HIGH_HEIGHT);
        }
        //Pitch angle
        if((prvPitch != meas.accPitch) || (ssdSettings.status == DISPLAY_CLEAR)){
            prvPitch = meas.accPitch;
            val1 = abs(meas.accPitch) / 10;
            ptch1 = val1 / FRACT_TENTHS;
            ptch2 = val1 - (ptch1*FRACT_TENTHS);
            if(ptch1 == 0) ptch2 = val1;
            sprintf(pitchAng, "%c%u.%.1u%c", 232, ptch1, ptch2, 248);
            ssd_putStrClr(76, 0, &pitchAng[0], 6, FONT_6X8);
        }
        //Roll border
        if((prvBrdr != meas.accRollBorder) || (ssdSettings.status == DISPLAY_CLEAR)){
            prvBrdr = meas.accRollBorder;
            sprintf(border, "%c%u%c", 241, meas.accRollBorder, 248);                //Roll border
            ssd_putString6x8(76, 9, &border[0]);
        }
    }else{
        sprintf(def, "Error display config");                                   //Default line
        ssd_putString6x8(8, 0, &def[0]);
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
