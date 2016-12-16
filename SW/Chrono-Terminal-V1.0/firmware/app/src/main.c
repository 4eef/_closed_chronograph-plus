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
extern ssdVideoBff_type ssdVideoBff;
meas_type               meas;
extern accel_type       accel;
kalman_type             kalman;
sysSettings_type        sysSettings;
extern menuStrs_type    menuStrs;
buttonCnts_type         buttonCnts;

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
MENU_ITEM(mgeneral,     mchron,         NULL_MENU,      mode,           NULL_MENU,      NULL,           NULL,           "General");
MENU_ITEM(mchron,       mincline,       mgeneral,       mode,           NULL_MENU,      NULL,           NULL,           "Chronograph");
MENU_ITEM(mincline,     NULL_MENU,      mchron,         mode,           NULL_MENU,      NULL,           NULL,           "Inclinometer");
//Menu/Settings
MENU_ITEM(smag,         schrono,        NULL_MENU,      setts,          NULL_MENU,      NULL,           NULL,           "Magazine");
MENU_ITEM(schrono,      sincline,       smag,           setts,          scdist,         NULL,           NULL,           "Chronograph");
MENU_ITEM(sincline,     sdisplay,       schrono,        setts,          sifilt,         NULL,           NULL,           "Inclinometer");
MENU_ITEM(sdisplay,     swusrc,         sincline,       setts,          sdcntr,         NULL,           NULL,           "Display");
MENU_ITEM(swusrc,       sdatime,        sdisplay,       setts,          NULL_MENU,      NULL,           NULL,           "Wake-up source");
MENU_ITEM(sdatime,      spsupply,       swusrc,         setts,          NULL_MENU,      NULL,           NULL,           "Date & time");
MENU_ITEM(spsupply,     NULL_MENU,      sdatime,        setts,          spdoff,         NULL,           NULL,           "Power supply");
//Menu/Settings/Chronograph
MENU_ITEM(scdist,       scir,           NULL_MENU,      schrono,        NULL_MENU,      NULL,           NULL,           "Sensor distance");
MENU_ITEM(scir,         scbind,         scdist,         schrono,        NULL_MENU,      NULL,           NULL,           "IR protocol");
MENU_ITEM(scbind,       NULL_MENU,      scir,           schrono,        NULL_MENU,      NULL,           NULL,           "Bind");
//Menu/Settings/Inclinometer
MENU_ITEM(sifilt,       sinsamp,        NULL_MENU,      sincline,       NULL_MENU,      NULL,           NULL,           "Filtering");
MENU_ITEM(sinsamp,      sical,          sifilt,         sincline,       NULL_MENU,      NULL,           NULL,           "Sampling");
MENU_ITEM(sical,        NULL_MENU,      sinsamp,        sincline,       NULL_MENU,      NULL,           NULL,           "Calibration");
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
    HAL_Init();
    SystemClock_Config();
    initGpios();
    MX_DMA_Init();
    MX_I2C1_Init();
    ug2864init();
    memset(ssdVideoBff.video, 0, sizeof(ssdVideoBff.video));
    ug2864_refresh();
    lis3init();
    //Debug info
    meas.speed0 = 240.5;
    meas.speed1 = 240;
    meas.speed2 = 230;
    meas.speed3 = 220;
    meas.speed4 = 210;
    meas.battCharge = 0;
    meas.accRollBorder = 5;
    meas.accPitchBorder = 90;
    meas.currPellets = 10;
    meas.numPellets = 10;
    sysSettings.magEn = 1;
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
            Menu_Navigate(MENU_PREVIOUS);
            break;
        case DOWN:
            Menu_Navigate(MENU_NEXT);
            break;
        case OK:
            if((MENU_CHILD == &NULL_MENU) || (MENU_CHILD == NULL)){
                Menu_EnterCurrentItem();
            }else{
                Menu_Navigate(MENU_CHILD);
            }
            break;
        case CANCEL:
            Menu_Navigate(MENU_PARENT);
            break;
        case HOME:
            Menu_Navigate(&display);
        default:
            break;
        }
        trxAccData();
        displayRefresh();
    }
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
* @brief    Write new data to video buffer and transmit
* @param    
* @retval   
*/
void displayRefresh(void){
    memset(ssdVideoBff.video, 0, sizeof(ssdVideoBff.video));    //Clear the buffer
    ssd_putBatt(meas.battCharge);                               //Battery symbol with charge percentage
    //Draw screen
    if(Menu_GetCurrentMenu() == &display){
        drawMainScreen();
        //drawAngles();
    }else{
        drawMenu();
    }
    //Refresh GDDRAM
    ug2864_refresh();
}

/*!****************************************************************************
* @brief    Fill video buffer with "angles display"
* @param    
* @retval   
*/
void drawAngles(void){
    char rollAng[7], pitchAng[7], border[4];
    sprintf(rollAng, "%c%.1f%c", 59, fabs(meas.accRoll), 61); //Roll angle
    sprintf(pitchAng, "%c%.1f%c", 232, fabs(meas.accPitch), 248);//Pitch angle
    sprintf(border, "%c%.0f%c", 241, meas.accRollBorder, 248);//Roll border
    ssd_putString12x16(0, 8, &rollAng[0]);
    ssd_putString6x8(92, 10, &pitchAng[0]);
    ssd_putString6x8(0, 56, &border[0]);
    ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_HIGH_Y, ROLL_HIGH_HEIGHT);
}

/*!****************************************************************************
* @brief    Fill video buffer with "menu"
* @param    
* @retval   
*/
void drawMenu(void){
    char arrow[2] = {26, 0};
    uint8_t i, pos, off;
    ssd_putMenuFolder();                                        //Folder for parent position
    if(menuStrs.totItems > MENU_POSITIONS) ssd_putMenuScroll(); //Put scroll bar
    //Offset calculation
    off = menuStrs.off;
    if(((menuStrs.currItem - MENU_POSITIONS) > off) && (menuStrs.currItem > MENU_POSITIONS)){
        off = menuStrs.currItem - MENU_POSITIONS;
    }else if(((menuStrs.currItem - MENU_POSITIONS) < off) && ((off - (menuStrs.currItem - MENU_POSITIONS)) > (MENU_POSITIONS-1))){
        off--;
    }
    menuStrs.off = off;
    //Calculate arrow position
    pos = menuStrs.currItem - off;
    //Put strings
    ssd_putString6x8(14, 1, &menuStrs.parent[0]);
    ssd_putString6x8(0, (MENU_START+MENU_INTERVAL*(pos-1)), &arrow[0]);//Arrow to current position
    for(i = 0; i < MENU_POSITIONS; i++){
        ssd_putString6x8(8, MENU_START+MENU_INTERVAL*i, &menuStrs.child[off+i][0]);
    }
}

/*!****************************************************************************
* @brief    Fill video buffer with "main screen"
* @param    
* @retval   
*/
void drawMainScreen(void){
    char speed0[7], speed1[4], speed2[4], speed3[4], speed4[4], magStat[6], rollAng[7], pitchAng[7];
    //Convert all measured data to strings
    sprintf(speed0, "%.2f%c", meas.speed0, 47);                 //Speed 0
    sprintf(speed1, "%u", meas.speed1);                         //Speed 1
    sprintf(speed2, "%u", meas.speed2);                         //Speed 2
    sprintf(speed3, "%u", meas.speed3);                         //Speed 3
    if(sysSettings.magEn == 1){                                 //Magazine status/Speed4
        sprintf(magStat, "%u/%u", meas.currPellets, meas.numPellets);
    }else{
        sprintf(speed4, "%u", meas.speed4);
    }
    sprintf(rollAng, "%c%.1f%c", 226, fabs(meas.accRoll), 248); //Roll angle
    sprintf(pitchAng, "%c%.1f%c", 232, fabs(meas.accPitch), 248);//Pitch angle
    //Put all the strings to the buffer
    ssd_putString6x8(0, 0, pellets[meas.pellet]);               //Detected pellet
    ssd_putString12x16(0, 24, &speed0[0]);                      //Measured speed
    ssd_putString6x8(98, 15, &speed1[0]);                       //Previous measurements
    ssd_putString6x8(98, 25, &speed2[0]);
    ssd_putString6x8(98, 35, &speed3[0]);
    if(sysSettings.magEn == 1){                                 //Magazine status/Speed4
        ssd_putString6x8(92, 45, &magStat[0]);
    }else{
        ssd_putString6x8(98, 45, &speed4[0]);
    }
    ssd_putString6x8(0, 40, &rollAng[0]);                       //Roll angle
    ssd_putString6x8(42, 40, &pitchAng[0]);                     //Pitch angle
    ssd_putRollBar(meas.accRoll, meas.accRollBorder, ROLL_LOW_Y, ROLL_LOW_HEIGHT);//Roll bar
    ssd_putPitchBar(meas.accPitch, meas.accPitchBorder);        //Pitch bar
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
