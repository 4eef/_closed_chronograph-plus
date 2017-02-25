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
#include "stm32f0xx.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "clock.h"
#include "gpio.h"
#include "i2c.h"
#include "delay.h"
#include "ug2864.h"
#include "lis3.h"
#include "MicroMenu.h"
#include "filter.h"
#include "IR.h"
#include "adc.h"
#include "sync.h"
#include "power.h"
#include "buttons.h"
#include "battery.h"
#include "system.h"

/*!****************************************************************************
* User define
*/
#define accSamples              1
#define degRad                  57.2958f
#define MODE_COM                0
#define MODE_CHR                1
#define MODE_INC                2
#define PELLETS_MAX             99
#define PELLETS_MIN             1
#define INC_BORDER_MAX          90
#define INC_BORDER_MIN          1
#define MAG_DISABLE             0
#define MAG_ENABLE              1
#define CHR_DIST_DEFAULT        200
#define CHR_DIST_MPLY           100000
#define CHR_TCK_NS              125
#define CHR_DIST_MIN            100
#define CHR_DIST_MAX            999
#define CHR_SPD_MAX             65535
#define FRACT_NOFRACT           0
#define FRACT_TENTHS            10
#define FRACT_HUNDREDTHS        100
#define ACC_N_SAMPLES           1

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/

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
void convtochar(void);
void trxAccData(void);
float s16fNorm(int16_t val);
float tiltAngCalc(float A, float B, float C);
void drawMainScreen(void);
void drawMenu(void);
void parEditRedir(void);
int16_t parEdit(int16_t param);
void modeEdit(void);

#endif //main_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
