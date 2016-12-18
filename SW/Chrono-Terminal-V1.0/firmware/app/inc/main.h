/*!****************************************************************************
* @file    main.h
* @author  4eef
* @version V1.0
* @date    07.08.2016
* @brief   --
*/
#ifndef main_H
#define main_H
#define SPS 800UL
#define Trc 0.005f
#define K (SPS*Trc)
#define accSamples 1
#define degRad 57.2958f
#define BUTTON_UP GP_SW2
#define BUTTON_DOWN GP_SW1
#define BUTTON_OK GP_SW4
#define BUTTON_CANCEL GP_SW3
#define MENU_POSITIONS 5
#define MENU_INTERVAL 10
#define MENU_START 12

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

/*!****************************************************************************
* User define
*/
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

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
    uint8_t     cntUp;
    uint8_t     cntDn;
    uint8_t     cntOK;
    uint8_t     cntCl;
}buttonCnts_type;

typedef struct{
    uint8_t     reserv  :2;
    uint8_t     parEdit :1;
    uint8_t     setDisp :1;
    uint8_t     incDisp :1;
    uint8_t     chrDisp :1;
    uint8_t     comDisp :1;
    uint8_t     magEn   :1;
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
    float       accRollBorder;
    float       accPitchBorder;
}meas_type;

typedef struct{
    uint16_t F;
    uint16_t H;
    uint16_t R;
    uint16_t Q;
    uint16_t X0;
    float P0;
    float Kgain;
    float Covariance;
    uint16_t State;
}kalman_type;

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void main(void);
extern void SystemClock_Config(void);
void displayRefresh(void);
void convtochar(void);
int16_t lpfx(int16_t data);
int16_t lpfy(int16_t data);
int16_t lpfz(int16_t data);
void trxAccData(void);
uint16_t Correct(uint16_t data);
float s16fNorm(int16_t val);
float tiltAngCalc(float A, float B, float C);
void drawMainScreen(void);
enum buttonValues getButtonState(void);
void drawMenu(void);
void menuParSel(void);
void menuParEdit(void);

#endif //main_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
