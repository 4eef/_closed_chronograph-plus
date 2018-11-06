/*!****************************************************************************
* @file    buttons.h
* @author  4eef
* @version V1.0
* @date    25.02.2017, 4eef
* @brief   --
*/
#ifndef buttons_H
#define buttons_H

/*!****************************************************************************
* Include
*/
#include "gpio.h"
#include "microMenu.h"

/*!****************************************************************************
* User define
*/
#define BUTTON_ENABLE           0
#define BUTTON_DISABLE          1
#define BUTTON_UP               GP_SW2
#define BUTTON_DOWN             GP_SW4
#define BUTTON_OK               GP_SW3
#define BUTTON_CANCEL           GP_SW1
#define BUTTON_SHORT            1
#define BUTTON_LOCKED           2
#define BUTTON_LONG             20
#define BUTTON_MAX              100

/*!****************************************************************************
* User typedef
*/
typedef struct{
    uint8_t         cntUp;
    uint8_t         cntDn;
    uint8_t         cntOK;
    uint8_t         cntCl;
}btnCnts_type;

typedef struct{
    btnCnts_type    cnts;
    uint8_t         upLck;
    uint8_t         dnLck;
    uint8_t         okLck;
    uint8_t         clLck;
}buttons_type;

/*!****************************************************************************
* Prototypes for the functions
*/
eNavEvent_type getNavEvent(void);
void btnCntsClr(void);

#endif //buttons_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
