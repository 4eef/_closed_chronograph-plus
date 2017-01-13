/*!****************************************************************************
* @file    main.h
* @author  4eef
* @version V1.0
* @date    07.08.2016
* @brief   --
*/
#ifndef main_H
#define main_H

/*!****************************************************************************
* Include
*/
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "clock.h"
#include "gpio.h"
#include "i2c.h"
#include "delay.h"
#include "ug2864.h"
#include "lis3.h"
#include "math.h"
#include "MicroMenu.h"
#include "filter.h"
#include "IR.h"

/*!****************************************************************************
* User define
*/
#define accSamples              1
#define degRad                  57.2958f
#define BUTTON_UP               GP_SW2
#define BUTTON_DOWN             GP_SW1
#define BUTTON_OK               GP_SW4
#define BUTTON_CANCEL           GP_SW3
#define MSG_CNT                 30
#define MSG_CNT_BIND            200
#define PAR_NONE                0
#define PAR_SAVE                1
#define PAR_CANCEL              2
#define PAR_EDIT_DISABLE        0
#define PAR_EDIT_ENABLE         1
#define PAR_UNSIGNED            0
#define PAR_SIGNED              1
#define MODE_COM                0
#define MODE_CHR                1
#define MODE_INC                2
#define PELLETS_MAX             99
#define PELLETS_MIN             1
#define INC_BORDER_MAX          90
#define INC_BORDER_MIN          1
#define MAG_DISABLE             0
#define MAG_ENABLE              1
#define CHR_DIST_DEFAULT        275
#define CHR_DIST_MPLY           100000
#define CHR_TCK_NS              125
#define CHR_DIST_MIN            100
#define CHR_DIST_MAX            999
#define CHR_SPD_MAX             65535
#define FRACT_TENTHS            10
#define FRACT_HUNDREDTHS        100
#define STAT_SHOTS_MAX          255
#define STAT_ENERGY_DIV_COEFF   10000

/*!****************************************************************************
* User enum
*/
enum buttonValues{
    NONE = 1,
    UP,
    DOWN,
    OK,
    CANCEL,
    HOME,
};

/*!****************************************************************************
* User typedef
*/
typedef struct{
    uint32_t    shotsTotal;
    uint16_t    prefPellet;
    uint32_t    shotsPPellet;
    uint16_t    avgRoll;
    uint32_t    uptime;
}stats_type;

typedef struct{
    uint8_t     reserv      :4;
    uint8_t     chrBind     :1;
    uint8_t     dispMode    :2;
    uint8_t     clipEn      :1;
}sysSettings_type;

typedef struct{
    uint16_t    pellet;
    uint8_t     clipCurrent;
    uint8_t     clipCapacity;
    uint16_t    speed0;
    uint16_t    speed1;
    uint16_t    speed2;
    uint16_t    speed3;
    uint16_t    speed4;
    uint16_t    pelSgntr;
    uint16_t    pelWeight;
    uint32_t    chrSgntr;
    uint16_t    sensDist;
    uint16_t    statSpeeds[STAT_SHOTS_MAX];
    uint32_t    statSpeedsSum;
    uint32_t    statDevsSum;
    uint16_t    statShots;
    uint16_t    statEnergy;
    uint16_t    statMean;
    uint16_t    statSdev;
}chron_type;

typedef struct{
    uint8_t     battCharge;
    uint16_t    battVolt;
    chron_type  chron;
    stats_type  stats;
    float       accRoll;
    float       accPitch;
    uint16_t    accRollBorder;
    uint16_t    accPitchBorder;
}meas_type;

typedef struct{
    uint8_t     cntUp;
    uint8_t     cntDn;
    uint8_t     cntOK;
    uint8_t     cntCl;
}buttonCnts_type;

/*!****************************************************************************
* Extern viriables
*/
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void main(void);
extern void SystemClock_Config(void);
void convtochar(void);
void trxAccData(void);
float s16fNorm(int16_t val);
float tiltAngCalc(float A, float B, float C);
void drawMainScreen(void);
enum buttonValues getButtonState(void);
void drawMenu(void);
void parEditRedir(void);
int16_t parEdit(int16_t param);
void modeEdit(void);

#endif //main_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
