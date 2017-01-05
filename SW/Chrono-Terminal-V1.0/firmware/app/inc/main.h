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
#define MSG_CNT                 20
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
#define PELLETS_MIN             2
#define INC_BORDER_MAX          90
#define INC_BORDER_MIN          1
#define MAG_DISABLE             0
#define MAG_ENABLE              1

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
    uint8_t     reserv      :3;
    uint8_t     dispMode    :2;
    uint8_t     magEn       :1;
}sysSettings_type;

typedef struct{
    uint16_t    pellet;
    uint8_t     currPellets;
    uint8_t     numPellets;
    uint16_t    shots;
    uint8_t     battCharge;
    uint16_t    battVolt;
    float       speed0;
    uint16_t    speed1;
    uint16_t    speed2;
    uint16_t    speed3;
    uint16_t    speed4;
    float       energy;
    float       mean;
    float       sdev;
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
