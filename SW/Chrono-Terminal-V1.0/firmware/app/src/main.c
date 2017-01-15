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
extern IRRXData_type        IRRXData;
buttonCnts_type             buttonCnts;
Menu_Item_t                 *CurrentMenuItem;
meas_type                   meas;
sysSettings_type            sysSettings;
lis3Settings_type           lis3Settings;
ssdSettings_type            ssdSettings;
pellets_type                pellets;

//Menu implementation
//        Name          Next            Previous        Parent          Child           SelectFunc      EnterFunc       Text
//Main screen
MENU_ITEM(display,      NULL_MENU,      NULL_MENU,      NULL_MENU,      mode,           NULL,           NULL,           "Menu");
//Menu
MENU_ITEM(mode,         stats,          NULL_MENU,      display,        mcommon,        NULL,           NULL,           "Display mode");
MENU_ITEM(stats,        setts,          mode,           display,        NULL_MENU,      NULL,           NULL,           "Statistics");
MENU_ITEM(setts,        about,          stats,          display,        sclipc,         NULL,           NULL,           "Settings");
MENU_ITEM(about,        service,        setts,          display,        NULL_MENU,      NULL,           NULL,           "About");
MENU_ITEM(service,      NULL_MENU,      about,          display,        NULL_MENU,      NULL,           NULL,           "Service info");
//Menu/Display mode
MENU_ITEM(mcommon,      mchron,         NULL_MENU,      mode,           NULL_MENU,      NULL,           modeEdit,       "Common");
MENU_ITEM(mchron,       mincline,       mcommon,        mode,           NULL_MENU,      NULL,           modeEdit,       "Chronograph");
MENU_ITEM(mincline,     NULL_MENU,      mchron,         mode,           NULL_MENU,      NULL,           modeEdit,       "Inclinometer");
//Menu/Settings
MENU_ITEM(sclipc,       schrono,        NULL_MENU,      setts,          NULL_MENU,      NULL,           parEditRedir,   "Clip capacity");
MENU_ITEM(schrono,      sincline,       sclipc,         setts,          scdist,         NULL,           NULL,           "Chronograph");
MENU_ITEM(sincline,     sdisplay,       schrono,        setts,          sibrdr,         NULL,           NULL,           "Inclinometer");
MENU_ITEM(sdisplay,     ssleep,         sincline,       setts,          sdcntr,         NULL,           NULL,           "Display");
MENU_ITEM(ssleep,       swusrc,         sdisplay,       setts,          NULL_MENU,      NULL,           NULL,           "Sleep");
MENU_ITEM(swusrc,       NULL_MENU,      ssleep,         setts,          NULL_MENU,      NULL,           NULL,           "Wake-up source");
//Menu/Settings/Chronograph
MENU_ITEM(scdist,       scbind,         NULL_MENU,      schrono,        NULL_MENU,      NULL,           parEditRedir,   "Sensor distance");
MENU_ITEM(scbind,       NULL_MENU,      scdist,         schrono,        NULL_MENU,      NULL,           parEditRedir,   "Bind");
//Menu/Settings/Inclinometer
MENU_ITEM(sibrdr,       sical,          NULL_MENU,      sincline,       NULL_MENU,      NULL,           parEditRedir,   "Roll level border");
MENU_ITEM(sical,        NULL_MENU,      sibrdr,         sincline,       NULL_MENU,      NULL,           NULL,           "Calibration");
//Menu/Settings/Display
MENU_ITEM(sdcntr,       sdpoff,         NULL_MENU,      sdisplay,       NULL_MENU,      NULL,           NULL,           "Contrast");
MENU_ITEM(sdpoff,       NULL_MENU,      sdcntr,         sdisplay,       NULL_MENU,      NULL,           NULL,           "Power off timeout");

/*!****************************************************************************
* @brief    Main function
* @param    
* @retval   
*/
void main(void){
    uint8_t i, offs, len;
    uint16_t val1, val2;
    uint32_t dist, sgn, spd0, spd1, spd2, spd3, spd4;
    char text[20], par[6];
    //Initialize hardware
    HAL_Init();
    SystemClock_Config();
    initGpios();
    MX_DMA_Init();
    MX_I2C1_Init();
    ug2864init();
    lis3init();
    initIRConfig();
    //Pellets database primary config
    strcpy(pellets.pelStrings[0], "<unknown pellet>");
    for(i = 1; i < PELLETS_DB_NUM; i++){
        sprintf(pellets.pelStrings[i], "Default pelet %u", i);
    }
    //Initial parameters
    meas.accRollBorder = 5;
    meas.accPitchBorder = 90;
    meas.chron.clipCapacity = PELLETS_MIN;
    meas.chron.sensDist = CHR_DIST_DEFAULT;
    kalman.F = 1;
    kalman.H = 1;
    kalman.R = 15;
    kalman.Q = 2;
    kalman.State = 0;
    kalman.Covariance = 0.1;
    //Navigate to an absolute menu item entry
    Menu_Navigate(&display);
    
    while(1){
        delay_us(25000);
        //MicroMenu navigation
        switch(getButtonState()){
        case UP:
            if(menu.parEdit == PAR_EDIT_ENABLE){
                if(menu.parValue <= menu.parBorderMin){
                    strcpy(menu.message, "Lower limit");
                    menu.msgCnt = MSG_CNT;
                }else{
                    menu.parValue--;
                }
            }else{
                Menu_Navigate(MENU_PREVIOUS);
            }
            break;
        case DOWN:
            if(menu.parEdit == PAR_EDIT_ENABLE){
                if(menu.parValue >= menu.parBorderMax){
                    strcpy(menu.message, "Higher limit");
                    menu.msgCnt = MSG_CNT;
                }else{
                    menu.parValue++;
                }
            }else{
                Menu_Navigate(MENU_NEXT);
            }
            break;
        case OK:
            if(menu.parEdit == PAR_EDIT_ENABLE){
                strcpy(menu.message, "Saved");
                menu.msgCnt = MSG_CNT;
                menu.parStat = PAR_SAVE;
            }else{
                if((MENU_CHILD == &NULL_MENU) || (MENU_CHILD == NULL)){
                    Menu_EnterCurrentItem();
                }else{
                    Menu_Navigate(MENU_CHILD);
                }
            }
            break;
        case CANCEL:
            if(menu.parEdit == PAR_EDIT_ENABLE){
                sysSettings.chrBind = 0;
                strcpy(menu.message, "Cancelled");
                menu.msgCnt = MSG_CNT;
                menu.parStat = PAR_CANCEL;
            }else{
                Menu_Navigate(MENU_PARENT);
            }
            break;
        case HOME:
            if(Menu_GetCurrentMenu() == &display){
                if((sysSettings.clipEn != 0) && (meas.chron.clipCurrent != meas.chron.clipCapacity)){
                    meas.chron.clipCurrent = meas.chron.clipCapacity;
                    strcpy(menu.message, "Clip replaced");
                    menu.msgCnt = MSG_CNT;
                }else if((meas.chron.statShots != 0) && (sysSettings.dispMode == MODE_CHR)){
                    meas.chron.statShots = 0;
                    meas.chron.statSpeedsSum = 0;
                    meas.chron.statSdev = 0;
                    meas.chron.statMean = 0;
                    strcpy(menu.message, "Stats cleared");
                    menu.msgCnt = MSG_CNT;
                }
            }else if(menu.parEdit == PAR_EDIT_DISABLE) Menu_Navigate(&display);
            break;
        default:
            menu.parStat = PAR_NONE;
            break;
        }
        CurrentMenuItem = Menu_GetCurrentMenu();
        trxAccData();
        //Magazine
        if(meas.chron.clipCapacity > 1){
            sysSettings.clipEn = 1;
        }else{
            sysSettings.clipEn = 0;
        }
        //Data received via IR channel
        if(IRRXData.rxState == IR_DATA_READY){
            sgn = (IRRXData.rxByte[IR_MAX_BYTES-1]) | (IRRXData.rxByte[IR_MAX_BYTES-2])<<8 | (IRRXData.rxByte[IR_MAX_BYTES-3])<<16 | (IRRXData.rxByte[IR_MAX_BYTES-4])<<24;
            if((sysSettings.dispMode != MODE_INC) && ((meas.chron.chrSgntr == sgn) || (sysSettings.chrBind != 0))){
                if(sysSettings.chrBind != 0){
                    sysSettings.chrBind = 0;
                    meas.chron.chrSgntr = sgn;
                    strcpy(menu.message, "Binded");
                    menu.msgCnt = MSG_CNT;
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
                if(sysSettings.clipEn != 0){
                    if(meas.chron.clipCurrent == 0){
                        meas.chron.clipCurrent = meas.chron.clipCapacity;
                        strcpy(menu.message, "Clip replaced");
                        menu.msgCnt = MSG_CNT;
                    }else if(meas.chron.clipCurrent == 1){
                        strcpy(menu.message, "Replace clip");
                        menu.msgCnt = MSG_CNT;
                    }
                    meas.chron.clipCurrent--;
                }
                //Pellet recognition by signature
                if(pellets.pelStat == PELLET_OK){
                    val1 = meas.chron.pelSgntr-meas.chron.pelSgntr/PELLET_SGN_TOLERANCE;
                    val2 = meas.chron.pelSgntr+meas.chron.pelSgntr/PELLET_SGN_TOLERANCE;
                    //Compare current signature with existing in database
                    if((pellets.pelSgntrs[meas.chron.pellet] >= val1) && (pellets.pelSgntrs[meas.chron.pellet] <= val2) && (pellets.matchedSgnNum != 0)){
                        pellets.newSgnCnt = 0;
                    }else if(pellets.matchedSgnNum != 0){
                        pellets.newSgnCnt++;
                        if(pellets.newSgnCnt > PELLET_CHANGE_THR){
                            pellets.newSgnCnt = 0;
                            pellets.matchedSgnNum = 0;
                            pellets.newSgn = meas.chron.pelSgntr;
                            for(i = 1; i < PELLETS_DB_NUM; i++){
                                if(pellets.pelSgntrs[i] >= val1 && pellets.pelSgntrs[i] <= val2){
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
                            pellets.newSgnCnt = 0;
                            pellets.newSgnSum = 0;
                        }
                        if(pellets.newSgnCnt > PELLET_NEW_SGN_THR){
                            pellets.newSgn = pellets.newSgnSum/pellets.newSgnCnt;
                            pellets.pelStat = PELLET_NEW;
                            pellets.newSgnCnt = 0;
                            pellets.newSgnSum = 0;
                        }else if(pellets.newSgnErrCnt > PELLET_NEW_SGN_BOUND){
                            pellets.pelStat = PELLET_ERR_NEW;
                            pellets.newSgnErrCnt = 0;
                            pellets.newSgnCnt = 0;
                            pellets.newSgnSum = 0;
                        }
                    }
                }
                //Statistics calculation
                if(sysSettings.dispMode == MODE_CHR){
                    if(meas.chron.statShots >= STAT_SHOTS_MAX){
                        strcpy(menu.message, "Buffer is full");
                        menu.msgCnt = MSG_CNT;
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
                        if((meas.chron.pellet != 0) && (meas.chron.pelWeight != 0)){
                            meas.chron.statEnergy = (((meas.chron.speed0*meas.chron.speed0)/STAT_ENERGY_DIV_COEFF)*meas.chron.pelWeight)/STAT_ENERGY_DIV_COEFF/2;
                        }
                    }
                }
            }
            IRRXData.rxState = IR_READY;                        //Release receiver
        }
        //Refresh video buffer
        memset(ssdVideoBff.video, 0, sizeof(ssdVideoBff.video));
        ssd_putBatt(meas.battCharge);
        //Draw screen
        if(CurrentMenuItem == &display){
            drawMainScreen();
        }else{
            drawMenu();
        }
        //Draw parameter edit box
        if(menu.parEdit == PAR_EDIT_ENABLE){
            strcpy(text, CurrentMenuItem->Text);
            ssd_putParBox(&text[0]);
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
        //Draw message
        if(menu.msgCnt != 0){
            menu.msgCnt--;
            if((sysSettings.chrBind != 0) && (menu.msgCnt == 0)){
                sysSettings.chrBind = 0;
                strcpy(menu.message, "Failed");
                menu.msgCnt = MSG_CNT;
            }
            len = strlen(menu.message);
            ssd_putMsgBox(len);
            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
            ssd_putString6x8(offs, 28, &menu.message[0]);
        }
        //Refresh GDDRAM
        ug2864_refresh();
    }
}

/*!****************************************************************************
* @brief    Redirect routine to edit required parameter
* @param    
* @retval   
*/
void parEditRedir(void){
    if(CurrentMenuItem == &sclipc){                             //Magazine capacity
        menu.parBorderMax = PELLETS_MAX;
        menu.parBorderMin = PELLETS_MIN;
        meas.chron.clipCapacity = parEdit(meas.chron.clipCapacity);
        meas.chron.clipCurrent = meas.chron.clipCapacity;
    }else if(CurrentMenuItem == &sibrdr){                       //Inclinometer roll level border
        menu.parBorderMax = INC_BORDER_MAX;
        menu.parBorderMin = INC_BORDER_MIN;
        meas.accRollBorder = parEdit(meas.accRollBorder);
    }else if(CurrentMenuItem == &scbind){                       //Bind new chronograph
        strcpy(menu.message, "Waiting...");
        menu.msgCnt = MSG_CNT_BIND;
        sysSettings.chrBind = 1;
    }else if(CurrentMenuItem == &scdist){                       //Edit sensor distance
        menu.parBorderMax = CHR_DIST_MAX;
        menu.parBorderMin = CHR_DIST_MIN;
        menu.parFract = FRACT_TENTHS;
        meas.chron.sensDist = parEdit(meas.chron.sensDist);
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
            menu.parFract = 0;
            return retVal;
        }else if(menu.parStat == PAR_CANCEL){
            menu.parEdit = PAR_EDIT_DISABLE;
            menu.parFract = 0;
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
    if(CurrentMenuItem == &mcommon)sysSettings.dispMode = MODE_COM;
    else if(CurrentMenuItem == &mchron)sysSettings.dispMode = MODE_CHR;
    else if(CurrentMenuItem == &mincline)sysSettings.dispMode = MODE_INC;
    strcpy(menu.message, "OK");
    menu.msgCnt = MSG_CNT;
}

/*!****************************************************************************
* @brief    Get button state routine
* @param    
* @retval   enum button value
*/
enum buttonValues getButtonState(void){
    enum buttonValues retVal;
    retVal = NONE;                                              //Default value
    //Sense the buttons
    if((gppin_get(BUTTON_UP) == 0) && (buttonCnts.cntUp <= 100)) buttonCnts.cntUp++;
    if((gppin_get(BUTTON_DOWN) == 0) && (buttonCnts.cntDn <= 100)) buttonCnts.cntDn++;
    if((gppin_get(BUTTON_OK) == 0) && (buttonCnts.cntOK <= 100)) buttonCnts.cntOK++;
    if((gppin_get(BUTTON_CANCEL) == 0) && (buttonCnts.cntCl <= 100)) buttonCnts.cntCl++;
    //Anti-bounce protection
    if((gppin_get(BUTTON_UP) != 0) && (buttonCnts.cntUp >= 2)){
        retVal = UP;
        buttonCnts.cntUp = 0;
        buttonCnts.cntDn = 0;
        buttonCnts.cntOK = 0;
        buttonCnts.cntCl = 0;
    }else 
    if((gppin_get(BUTTON_DOWN) != 0) && (buttonCnts.cntDn >= 2)){
        retVal = DOWN;
        buttonCnts.cntUp = 0;
        buttonCnts.cntDn = 0;
        buttonCnts.cntOK = 0;
        buttonCnts.cntCl = 0;
    }else 
    if((gppin_get(BUTTON_OK) != 0) && (buttonCnts.cntOK >= 2) &&  (buttonCnts.cntCl < 10)){
        retVal = OK;
        buttonCnts.cntUp = 0;
        buttonCnts.cntDn = 0;
        buttonCnts.cntOK = 0;
        buttonCnts.cntCl = 0;
    }else
    if((gppin_get(BUTTON_CANCEL) != 0) && (buttonCnts.cntCl >= 2)){
        retVal = CANCEL;
        buttonCnts.cntUp = 0;
        buttonCnts.cntDn = 0;
        buttonCnts.cntOK = 0;
        buttonCnts.cntCl = 0;
    }else 
    if((gppin_get(BUTTON_CANCEL) == 0) && (buttonCnts.cntCl >= 10)){//Button is hold
        retVal = HOME;
        buttonCnts.cntUp = 0;
        buttonCnts.cntDn = 0;
        buttonCnts.cntOK = 0;
        buttonCnts.cntCl = 0;
    }
    return retVal;
};

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
    float retVal;
    if(val >= 0){
        retVal = val/16383.;
    }else{
        retVal = val/16384.;
    }
    return retVal;
}

/*!****************************************************************************
* @brief    Get tilt data from accelerometer readings
* @param    
* @retval   
*/
void trxAccData(void){
    float X, Y, Z;
    uint8_t reg, tmp, numSamples = 1;
    accel.rawX = 0;
    accel.rawY = 0;
    accel.rawZ = 0;
    //Settings
    lis3_write(0x20, 0x87);                                     //800 Hz sample rate
    //Get the samples
    tmp = numSamples;
    while(tmp != 0){
        reg = lis3_read(0x27);                                  //Check if data is ready
        if((reg & 0x3) != 0){
            lis3_getXYZ();                                      //Get sampled data
            tmp--;
        }
    }
    lis3_write(0x20, 0x00);                                     //Off
    //Data filtering
    accel.corrX = lpfx(accel.rawX);
    accel.corrY = lpfy(accel.rawY);
    accel.corrZ = lpfz(accel.rawZ);
    //Normalize values
    X = s16fNorm(accel.corrX);//rawX);
    Y = s16fNorm(accel.corrY);//rawY);
    Z = s16fNorm(accel.corrZ);//rawZ);
    //Roll calculation
    meas.accRoll = tiltAngCalc(Y, X, Z);
    //Pitch calculation
    meas.accPitch = tiltAngCalc(Z, X, Y);
}

/*!****************************************************************************
* @brief    Fill video buffer with "menu"
* @param    
* @retval   
*/
void drawMenu(void){
    char arrow[2] = {26, 0};
    uint8_t i, pos, offs;
    ssd_putMenuFolder();                                        //Folder for parent position
    if(menu.totItems > MENU_POSITIONS) ssd_putMenuScroll(); //Put scroll bar
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
    ssd_putString6x8(0, (MENU_START+MENU_INTERVAL*(pos-1)), &arrow[0]);//Arrow to current position
    for(i = 0; i < MENU_POSITIONS; i++){
        ssd_putString6x8(8, MENU_START+MENU_INTERVAL*i, &menu.child[offs+i][0]);
    }
    //Edit function
    if(menu.parEdit == PAR_EDIT_ENABLE || menu.parStat != PAR_NONE) parEditRedir();
}

/*!****************************************************************************
* @brief    Fill video buffer with "main screen"
* @param    
* @retval   
*/
void drawMainScreen(void){
    char speed0[8], speed1[4], speed2[4], speed3[4], speed4[4], magStat[6], def[21];
    char rollAng[7], pitchAng[7], energy[8], currShots[4], mean[8], sdev[8], border[4];
    uint16_t val1, val2;
    sprintf(pitchAng, "%c%.1f%c", 232, fabs(meas.accPitch), 248);
    //Display settings
    if(sysSettings.dispMode == MODE_COM || sysSettings.dispMode == MODE_CHR){
        val1 = meas.chron.speed0/FRACT_HUNDREDTHS;
        val2 = meas.chron.speed0-(val1*FRACT_HUNDREDTHS);
        if(val1 == 0) val2 = meas.chron.speed0;
        sprintf(speed0, "%u.%.2u%c", val1, val2, 47);           //Speed 0
        sprintf(speed1, "%u", meas.chron.speed1/100);           //Speed 1
        sprintf(speed2, "%u", meas.chron.speed2/100);           //Speed 2
        sprintf(speed3, "%u", meas.chron.speed3/100);           //Speed 3
        if(sysSettings.clipEn == 1){                            //Magazine status/Speed4
            sprintf(magStat, "%u/%u", meas.chron.clipCurrent, meas.chron.clipCapacity);
            ssd_putString6x8(92, 45, &magStat[0]);
        }else{
            sprintf(speed4, "%u", meas.chron.speed4/100);
            ssd_putString6x8(98, 45, &speed4[0]);
        }
        ssd_putString6x8(0, 0, &pellets.pelStrings[meas.chron.pellet][0]);//Detected pellet
        ssd_putString12x16(0, 24, &speed0[0]);                  //Measured speed
        ssd_putString6x8(98, 15, &speed1[0]);                   //Previous measurements
        ssd_putString6x8(98, 25, &speed2[0]);
        ssd_putString6x8(98, 35, &speed3[0]);
        //Select the info to display
        if(sysSettings.dispMode == MODE_COM){
            sprintf(rollAng, "%c%.1f%c", 226, fabs(meas.accRoll), 248);
            ssd_putString6x8(0, 40, &rollAng[0]);
            ssd_putString6x8(42, 40, &pitchAng[0]);
            ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_LOW_Y, ROLL_LOW_HEIGHT);
            ssd_putPitchBar(meas.accPitch, meas.accPitchBorder);
        }else{
            sprintf(currShots, "%u", meas.chron.statShots);     //Number of shots per measurement
            val1 = meas.chron.statMean/FRACT_HUNDREDTHS;
            val2 = meas.chron.statMean-(val1*FRACT_HUNDREDTHS);
            if(val1 == 0) val2 = meas.chron.statMean;
            sprintf(mean, "%c%u.%.2u", 230, val1, val2);        //Mean of current measurement
            val1 = meas.chron.statSdev/FRACT_HUNDREDTHS;
            val2 = meas.chron.statSdev-(val1*FRACT_HUNDREDTHS);
            if(val1 == 0) val2 = meas.chron.statSdev;
            sprintf(sdev, "%c%u.%.2u", 229, val1, val2);        //Standard deviation
            val1 = meas.chron.statEnergy/FRACT_HUNDREDTHS;
            val2 = meas.chron.statEnergy-(val1*FRACT_HUNDREDTHS);
            if(val1 == 0) val2 = meas.chron.statEnergy;
            sprintf(energy, "%u.%.2u%c", val1, val2, 58);       //Calculated energy
            ssd_putString6x8(98, 55, &currShots[0]);
            ssd_putString6x8(0, 36, &mean[0]);
            ssd_putString6x8(49, 36, &sdev[0]);
            ssd_putString12x16(0, 56, &energy[0]);
        }
    }else if(sysSettings.dispMode == MODE_INC){
        sprintf(rollAng, "%c%.1f%c", 59, fabs(meas.accRoll), 61);//Roll angle
        sprintf(border, "%c%u%c", 241, meas.accRollBorder, 248);//Roll border
        ssd_putString12x16(0, 8, &rollAng[0]);
        ssd_putString6x8(76, 0, &pitchAng[0]);
        ssd_putString6x8(76, 9, &border[0]);
        ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_HIGH_Y, ROLL_HIGH_HEIGHT);
    }else{
        sprintf(def, "Error display config");                   //Default line
        ssd_putString6x8(8, 0, &def[0]);
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
