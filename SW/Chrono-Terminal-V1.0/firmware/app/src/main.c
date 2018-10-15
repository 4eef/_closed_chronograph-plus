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
extern chron_type           chron;
extern stats_type           stats;
extern battery_type         battery;
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
    battery.battVltPts[0] = 4150;
    battery.battVltPts[1] = 4050;
    battery.battVltPts[2] = 3970;
    battery.battVltPts[3] = 3905;
    battery.battVltPts[4] = 3865;
    battery.battVltPts[5] = 3785;
    battery.battVltPts[6] = 3765;
    battery.battVltPts[7] = 3750;
    battery.battVltPts[8] = 3741;
    battery.battVltPts[9] = 3705;
    battery.battVltPts[10] = 3595;
    //Pellets database primary config
    for(i = 0; i < PELLET_DB_QTY; i++){
        sprintf(pellets.pel[i].name, "Default pellet %u", (i + 1));
        pellets.pel[i].wght = PELLET_WGH_MG10_MIN;
    }
    //Screen modes names
    strcpy(sysPars.modeTxts[0], "Hybrid");
    strcpy(sysPars.modeTxts[1], "Chronograph");
    strcpy(sysPars.modeTxts[2], "Inclinometer");
    //Initial parameters
    meas.accRollBorder = 5;
    meas.accPitchBorder = 90;
    chron.clipCapacity = PELLET_MIN;
    chron.sensDist = CHR_DIST_DEFAULT;
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
    Menu_pwrSw(ePwrOff);//ePwrOff/ePwrOn
    adcInit();
    
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
* @brief    Redirect routine to edit required parameter
*/
void drawDisplay(void){
    ssd_clearVidBff();
    ssd_putBatt(battery.battCharge, battery.battChgStat);
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

    if((pellets.irRxPelSgn.pelStat == PELLET_CONFIRM) || (pellets.irRxPelSgn.pelStat == PELLET_NEW)){
        pellets.irRxPelSgn.pelStat = PELLET_OK;
        Menu_putTxtParSelWnd(pellets.pel[0].name, &chron.pellet, PELLET_KNOWN_LIST);
    }
    //Chronograph binding
    if(chron.chrBindCnt != 0){
        chron.chrBindCnt--;
        if(chron.chrBindCnt == 0) Menu_putMessage("Failed", MSG_CNT);
    }
    //Draw message
    ssd_putMessage();
    //Refresh GDDRAM
    ug2864_refresh();
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
