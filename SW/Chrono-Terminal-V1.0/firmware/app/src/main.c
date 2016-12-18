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
kalman_type                 kalman;
buttonCnts_type             buttonCnts;
Menu_Item_t                 *CurrentMenuItem;
meas_type                   meas;
sysSettings_type            sysSettings;
lis3Settings_type           lis3Settings;
ssdSettings_type            ssdSettings;

//Known pellets database
const char *pellets[2] = {
    "<unknown pellet>",
    "JSB Exact 0.547",
};

//Menu implementation
//        Name          Next            Previous        Parent          Child           SelectFunc      EnterFunc       Text
//Main screen
MENU_ITEM(display,      NULL_MENU,      NULL_MENU,      NULL_MENU,      mode,           NULL,           NULL,           "Menu");
//Menu
MENU_ITEM(mode,         stats,          NULL_MENU,      display,        mgeneral,       NULL,           NULL,           "Display mode");
MENU_ITEM(stats,        setts,          mode,           display,        NULL_MENU,      NULL,           NULL,           "Statistics");
MENU_ITEM(setts,        about,          stats,          display,        smag,           NULL,           NULL,           "Settings");
MENU_ITEM(about,        NULL_MENU,      setts,          display,        service,        NULL,           NULL,           "About");
//Menu/Display mode
MENU_ITEM(mgeneral,     mchron,         NULL_MENU,      mode,           NULL_MENU,      NULL,           modeEdit,       "General");
MENU_ITEM(mchron,       mincline,       mgeneral,       mode,           NULL_MENU,      NULL,           modeEdit,       "Chronograph");
MENU_ITEM(mincline,     NULL_MENU,      mchron,         mode,           NULL_MENU,      NULL,           modeEdit,       "Inclinometer");
//Menu/Settings
MENU_ITEM(smag,         schrono,        NULL_MENU,      setts,          smcapacity,     NULL,           NULL,           "Magazine");
MENU_ITEM(schrono,      sincline,       smag,           setts,          scdist,         NULL,           NULL,           "Chronograph");
MENU_ITEM(sincline,     sdisplay,       schrono,        setts,          sifilt,         NULL,           NULL,           "Inclinometer");
MENU_ITEM(sdisplay,     swusrc,         sincline,       setts,          sdcntr,         NULL,           NULL,           "Display");
MENU_ITEM(swusrc,       sdatime,        sdisplay,       setts,          NULL_MENU,      NULL,           NULL,           "Wake-up source");
MENU_ITEM(sdatime,      spsupply,       swusrc,         setts,          NULL_MENU,      NULL,           NULL,           "Date & time");
MENU_ITEM(spsupply,     NULL_MENU,      sdatime,        setts,          spdoff,         NULL,           NULL,           "Power supply");
//Menu/Settings/Magazine
MENU_ITEM(smcapacity,   smon,           NULL_MENU,      smag,           NULL_MENU,      NULL,           parEditRedir,   "Capacity");
MENU_ITEM(smon,         smoff,          smcapacity,     smag,           NULL_MENU,      NULL,           modeEdit,       "Enable");
MENU_ITEM(smoff,        NULL_MENU,      smon,           smag,           NULL_MENU,      NULL,           modeEdit,       "Disable");
//Menu/Settings/Chronograph
MENU_ITEM(scdist,       scir,           NULL_MENU,      schrono,        NULL_MENU,      NULL,           NULL,           "Sensor distance");
MENU_ITEM(scir,         scbind,         scdist,         schrono,        NULL_MENU,      NULL,           NULL,           "IR protocol");
MENU_ITEM(scbind,       NULL_MENU,      scir,           schrono,        NULL_MENU,      NULL,           NULL,           "Bind");
//Menu/Settings/Inclinometer
MENU_ITEM(sifilt,       sibrdr,         NULL_MENU,      sincline,       NULL_MENU,      NULL,           NULL,           "Filtering");
MENU_ITEM(sibrdr,       sical,          sifilt,         sincline,       NULL_MENU,      NULL,           parEditRedir,   "Roll level border");
MENU_ITEM(sical,        NULL_MENU,      sibrdr,         sincline,       NULL_MENU,      NULL,           NULL,           "Calibration");
//Menu/Settings/Display
MENU_ITEM(sdcntr,       sdfreq,         NULL_MENU,      sdisplay,       NULL_MENU,      NULL,           NULL,           "Contrast");
MENU_ITEM(sdfreq,       NULL_MENU,      sdcntr,         sdisplay,       NULL_MENU,      NULL,           NULL,           "Frequency");
//Menu/Settings/Power supply
MENU_ITEM(spdoff,       spslp,          NULL_MENU,      spsupply,       NULL_MENU,      NULL,           NULL,           "Display off");
MENU_ITEM(spslp,        sppd,           spdoff,         spsupply,       NULL_MENU,      NULL,           NULL,           "Sleep");
MENU_ITEM(sppd,         NULL_MENU,      spslp,          spsupply,       NULL_MENU,      NULL,           NULL,           "Power off");
//Menu/About
MENU_ITEM(service,      NULL_MENU,      NULL_MENU,      about,          NULL_MENU,      NULL,           NULL,           "Service menu");

/*!****************************************************************************
* @brief    Main function
* @param    
* @retval   
*/
void main(void){
    uint8_t offs, len;
    char text[20], par[6];
    //Initialize hardware
    HAL_Init();
    SystemClock_Config();
    initGpios();
    MX_DMA_Init();
    MX_I2C1_Init();
    ug2864init();
    lis3init();
    //Initial parameters
    meas.accRollBorder = 5;
    meas.accPitchBorder = 90;
    meas.numPellets = PELLETS_MIN;
    sysSettings.magEn = 0;
    sysSettings.dispMode = MODE_COM;
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
        switch (getButtonState()){
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
                strcpy(menu.message, "Cancelled");
                menu.msgCnt = MSG_CNT;
                menu.parStat = PAR_CANCEL;
            }else{
                Menu_Navigate(MENU_PARENT);
            }
            break;
        case HOME:
            if(menu.parEdit == PAR_EDIT_DISABLE) Menu_Navigate(&display);
            break;
        default:
            menu.parStat = PAR_NONE;
            break;
        }
        CurrentMenuItem = Menu_GetCurrentMenu();
        trxAccData();
        //Refresh video buffer with nre data and transmit it
        memset(ssdVideoBff.video, 0, sizeof(ssdVideoBff.video));
        ssd_putBatt(meas.battCharge);
        //Draw screen
        if(Menu_GetCurrentMenu() == &display){
            drawMainScreen();
        }else{
            drawMenu();
        }
        //Parameter edit box
        if(menu.parEdit == PAR_EDIT_ENABLE){
            strcpy(text, CurrentMenuItem->Text);
            ssd_putParBox(&text[0]);
            sprintf(par, "%d", menu.parValue);
            len = strlen(par);
            offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
            ssd_putString6x8(offs, 28, &par[0]);
        }
        //Message
        if(menu.msgCnt != 0){
            menu.msgCnt--;
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
    //Magazine capacity
    if(CurrentMenuItem == &smcapacity){
        menu.parBorderMax = PELLETS_MAX;
        menu.parBorderMin = PELLETS_MIN;
        meas.numPellets = parEdit(meas.numPellets);
    }else
    //Inclinometer roll level border
    if(CurrentMenuItem == &sibrdr){
        menu.parBorderMax = INC_BORDER_MAX;
        menu.parBorderMin = INC_BORDER_MIN;
        meas.accRollBorder = parEdit(meas.accRollBorder);
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
        }else
        if(menu.parStat == PAR_CANCEL){
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
    if(CurrentMenuItem == &mgeneral)sysSettings.dispMode = MODE_COM;
    else
    if(CurrentMenuItem == &mchron)sysSettings.dispMode = MODE_CHR;
    else
    if(CurrentMenuItem == &mincline)sysSettings.dispMode = MODE_INC;
    else
    if(CurrentMenuItem == &smon)sysSettings.magEn = MAG_ENABLE;
    else
    if(CurrentMenuItem == &smoff)sysSettings.magEn = MAG_DISABLE;
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
* @brief   Simplified Kalman filter
* @param    
* @retval   
*/
uint16_t Correct(uint16_t data){
    //time update - prediction
    kalman.X0 = kalman.F*kalman.State;
    kalman.P0 = kalman.F*kalman.Covariance*kalman.F + kalman.Q;

    //measurement update - correction
    kalman.Kgain = kalman.H*kalman.P0/(kalman.H*kalman.P0*kalman.H + kalman.R);
    kalman.State = (uint16_t)(kalman.X0 + kalman.Kgain*(data - kalman.H*kalman.X0));
    kalman.Covariance = (1 - kalman.Kgain*kalman.H)*kalman.P0;
    return kalman.State;
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
    char rollAng[7], pitchAng[7], energy[8], shots[4], mean[8], sdev[8], border[4];
    sprintf(pitchAng, "%c%.1f%c", 232, fabs(meas.accPitch), 248);
    //Display settings
    if(sysSettings.dispMode == MODE_COM || sysSettings.dispMode == MODE_CHR){
        sprintf(speed0, "%.2f%c", meas.speed0, 47);             //Speed 0
        sprintf(speed1, "%u", meas.speed1);                     //Speed 1
        sprintf(speed2, "%u", meas.speed2);                     //Speed 2
        sprintf(speed3, "%u", meas.speed3);                     //Speed 3
        if(sysSettings.magEn == 1){                             //Magazine status/Speed4
            sprintf(magStat, "%u/%u", meas.currPellets, meas.numPellets);
            ssd_putString6x8(92, 45, &magStat[0]);
        }else{
            sprintf(speed4, "%u", meas.speed4);
            ssd_putString6x8(98, 45, &speed4[0]);
        }
        ssd_putString6x8(0, 0, pellets[meas.pellet]);           //Detected pellet
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
            sprintf(shots, "%d", meas.shots);                   //Number of shots per measurement
            sprintf(mean, "%c%.2f", 230, meas.mean);            //Mean of current measurement
            sprintf(sdev, "%c%.2f", 229, meas.sdev);            //Standard deviation
            sprintf(energy, "%.2f%c", meas.energy, 58);         //Calculated energy
            ssd_putString6x8(98, 55, &shots[0]);
            ssd_putString6x8(0, 36, &mean[0]);
            ssd_putString6x8(48, 36, &sdev[0]);
            ssd_putString12x16(0, 56, &energy[0]);
        }
    }else if(sysSettings.dispMode == MODE_INC){
        sprintf(rollAng, "%c%.1f%c", 59, fabs(meas.accRoll), 61);//Roll angle
        sprintf(border, "%c%d%c", 241, meas.accRollBorder, 248);//Roll border
        ssd_putString12x16(0, 8, &rollAng[0]);
        ssd_putString6x8(76, 0, &pitchAng[0]);
        ssd_putString6x8(76, 9, &border[0]);
        ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_HIGH_Y, ROLL_HIGH_HEIGHT);
    }else{
        sprintf(def, "Error display config");                   //Default line
        ssd_putString6x8(8, 0, &def[0]);
    }
}

/*!****************************************************************************
* @brief    Damp the value with LPF
* @param    
* @retval   
*/
int16_t lpfx(int16_t data){
    static int32_t Dacc = 0;
    static int16_t Dout = 0;
    int16_t Din = data;
    
    Dacc = Dacc + Din - Dout;
    Dout = Dacc/(int16_t)K;
    
    return Dout;
}
int16_t lpfy(int16_t data){
    static int32_t Dacc = 0;
    static int16_t Dout = 0;
    int16_t Din = data;
    
    Dacc = Dacc + Din - Dout;
    Dout = Dacc/(int16_t)K;
    
    return Dout;
}
int16_t lpfz(int16_t data){
    static int32_t Dacc = 0;
    static int16_t Dout = 0;
    int16_t Din = data;
    
    Dacc = Dacc + Din - Dout;
    Dout = Dacc/(int16_t)K;
    
    return Dout;
}
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
