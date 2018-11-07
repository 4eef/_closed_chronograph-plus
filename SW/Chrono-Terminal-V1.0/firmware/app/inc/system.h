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
#include "chrono.h"

/*!****************************************************************************
* User define
*/
#define CYC_OK                  0
#define CYC_BROKEN              1
#define SCR_MODES_NO            3

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
    uint32_t        shotsTotal;
    uint16_t        prefPellet;
    uint32_t        shotsPrefPellet;
    uint16_t        avgRoll;
    uint32_t        cycTotal;
    uint32_t        cycBroken;
}stats_type;

typedef struct{
    int16_t         accRoll;
    int16_t         accPitch;
    uint16_t        accRollBorder;
    uint16_t        accPitchBorder;
}meas_type;

typedef struct{
    char            modeTxts[SCR_MODES_NO][MENU_STR_LEN_MAX];
    uint8_t         tmp;//Alignment
    eDispMode_type  dispMode;
}sysPars_type;

/*!****************************************************************************
* Extern viriables
*/
extern sysPars_type         sysPars;

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
