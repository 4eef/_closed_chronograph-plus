/*!****************************************************************************
* @file    system.h
* @author  4eef
* @version V1.0
* @date    25.02.2017, 4eef
* @brief   --
*/
#ifndef system_H
#define system_H

/*!****************************************************************************
* Include
*/
#include "stm32f0xx.h"
#include "ug2864.h"
#include "lis3.h"

/*!****************************************************************************
* User define
*/
#define PELLET_DB_NULL          1
#define PELLET_DB_NUM           16
#define PELLET_SGN_TOLERANCE    10
#define PELLET_CHANGE_THR       5
#define PELLET_NEW_SGN_THR      10
#define PELLET_NEW_SGN_BOUND    PELLET_NEW_SGN_THR*1.5
#define PELLET_OK               0
#define PELLET_CONFIRM          1
#define PELLET_NEW              2
#define PELLET_ERR_NEW          3
#define PELLET_MAX              99
#define PELLET_MIN              1
#define STAT_SHOTS_MAX          255
#define STAT_ENERGY_DIV_COEFF   10000
#define BATT_CHG_ARR_PTS        11
#define BATT_CHG_VMPLY_COEFF    10
#define BATT_ADD_PERC_MIN       0
#define BATT_ADD_PERC_MAX       9
#define BATT_MSG_PERIOD         1200
#define BATT_PERC_HYST          3
#define BATT_PERC_FLAG_NONE     0
#define BATT_PERC_FLAG_INC      1
#define BATT_PERC_FLAG_DEC      2
#define BATT_VOLT_LOW           3400
#define CYC_OK                  0
#define CYC_BROKEN              1
#define MODE_COM                0
#define MODE_CHR                1
#define MODE_INC                2
#define FRACT_NOFRACT           0
#define FRACT_TENTHS            10
#define FRACT_HUNDREDTHS        100

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef enum{
    eHybrid = 0,
    eChronograph,
    eInclinometer
}eDispMode_type;

typedef struct{
    char            pelStrings[PELLET_DB_NUM][18];
    uint16_t        pelWghts[PELLET_DB_NUM];
    uint16_t        pelSgntrs[PELLET_DB_NUM];
    uint16_t        matchedSgnNum;
    uint16_t        newSgn;
    uint32_t        newSgnSum;
    uint8_t         newSgnCnt;
    uint8_t         newSgnErrCnt;
    uint8_t         pelStat;
}pellets_type;

typedef struct{
    uint32_t        shotsTotal;
    uint16_t        prefPellet;
    uint32_t        shotsPrefPellet;
    uint16_t        avgRoll;
    uint32_t        cycTotal;
    uint32_t        cycBroken;
}stats_type;

typedef struct{
    uint16_t        pellet;
    uint16_t        clipCurrent;
    uint16_t        clipCapacity;
    uint16_t        speed0;
    uint16_t        speed1;
    uint16_t        speed2;
    uint16_t        speed3;
    uint16_t        speed4;
    uint16_t        pelSgntr;
    uint32_t        chrSgntr;
    uint16_t        chrBindCnt;
    uint16_t        sensDist;
    uint16_t        statSpeeds[STAT_SHOTS_MAX];
    uint32_t        statSpeedsSum;
    uint32_t        statDevsSum;
    uint16_t        statShots;
    uint16_t        statEnergy;
    uint16_t        statMean;
    uint16_t        statSdev;
}chron_type;

typedef struct{
    uint8_t         battCharge;
    uint8_t         battChgStat;
    uint16_t        battVolt;
    uint16_t        battVltPts[BATT_CHG_ARR_PTS];
    uint16_t        battMsgPer;
}battery_type;

typedef struct{
    battery_type    battery;
    chron_type      chron;
    stats_type      stats;
    int16_t         accRoll;
    int16_t         accPitch;
    uint16_t        accRollBorder;
    uint16_t        accPitchBorder;
}meas_type;

typedef struct{
    eDispMode_type  dispMode;
}sysPars_type;

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void drawIncScr(void);
void drawChrScr(void);
void drawHybrScr(void);
void drawMenu(void);

#endif //system_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
