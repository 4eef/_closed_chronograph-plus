/*!****************************************************************************
* @file    buttons.c
* @author  4eef
* @version V1.0
* @date    25.02.2017, 4eef
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "buttons.h"

/*!****************************************************************************
* MEMORY
*/
extern power_type           power;
buttons_type                buttons;

/*!****************************************************************************
* @brief    Get button state routine (revise: use timer to eliminate bounce)
* @param    
* @retval   enum button value
*/
enum buttonValues getButtonState(void){
    //Sense the buttons
    if((gppin_get(BUTTON_UP) == 0) && (buttons.cnts.cntUp <= BUTTON_MAX)) buttons.cnts.cntUp++;
    if((gppin_get(BUTTON_DOWN) == 0) && (buttons.cnts.cntDn <= BUTTON_MAX)) buttons.cnts.cntDn++;
    if((gppin_get(BUTTON_OK) == 0) && (buttons.cnts.cntOK <= BUTTON_MAX) && (buttons.okLck == BUTTON_ENABLE)) buttons.cnts.cntOK++;
    if((gppin_get(BUTTON_CANCEL) == 0) && (buttons.cnts.cntCl <= BUTTON_MAX) && (buttons.clLck == BUTTON_ENABLE)) buttons.cnts.cntCl++;
    //Up
    if((gppin_get(BUTTON_UP) != 0) && (buttons.upLck == BUTTON_ENABLE) && (buttons.cnts.cntUp >= BUTTON_SHORT) && (buttons.cnts.cntUp < BUTTON_LONG)){
        btnCntsClr();
        return UP;
    }else if((gppin_get(BUTTON_UP) == 0) && (buttons.upLck == BUTTON_ENABLE) && (buttons.cnts.cntUp >= BUTTON_LONG)){
        btnCntsClr();
        buttons.upLck = BUTTON_DISABLE;                                         //Lock button
        return UP;
    }else if((gppin_get(BUTTON_UP) == 0) && (buttons.upLck == BUTTON_DISABLE) && (buttons.cnts.cntUp >= BUTTON_LOCKED)){
        btnCntsClr();
        return UP;
    }else if((gppin_get(BUTTON_UP) != 0) && (buttons.upLck == BUTTON_DISABLE)){
        buttons.upLck = BUTTON_ENABLE;
    }else
    //Down
    if((gppin_get(BUTTON_DOWN) != 0) && (buttons.dnLck == BUTTON_ENABLE) && (buttons.cnts.cntDn >= BUTTON_SHORT) && (buttons.cnts.cntDn < BUTTON_LONG)){
        btnCntsClr();
        return DOWN;
    }else if((gppin_get(BUTTON_DOWN) == 0) && (buttons.dnLck == BUTTON_ENABLE) && (buttons.cnts.cntDn >= BUTTON_LONG)){
        btnCntsClr();
        buttons.dnLck = BUTTON_DISABLE;
        return DOWN;
    }else if((gppin_get(BUTTON_DOWN) == 0) && (buttons.dnLck == BUTTON_DISABLE) && (buttons.cnts.cntDn >= BUTTON_LOCKED)){
        btnCntsClr();
        return DOWN;
    }else if((gppin_get(BUTTON_DOWN) != 0) && (buttons.dnLck == BUTTON_DISABLE)){
        buttons.dnLck = BUTTON_ENABLE;
    }else
    //Ok
    if((gppin_get(BUTTON_OK) != 0) && (buttons.okLck == BUTTON_ENABLE) && (buttons.cnts.cntOK >= BUTTON_SHORT)){
        btnCntsClr();
        return OK;
    }else if((gppin_get(BUTTON_OK) == 0) && (buttons.okLck == BUTTON_ENABLE) && (buttons.cnts.cntOK >= BUTTON_LONG)){
        btnCntsClr();
        buttons.okLck = BUTTON_DISABLE;
        return OKLNG;
    }else if((gppin_get(BUTTON_OK) != 0) && (buttons.okLck == BUTTON_DISABLE) && (buttons.cnts.cntOK < BUTTON_SHORT)){
        buttons.okLck = BUTTON_ENABLE;
    }else
    //Cancel
    if((gppin_get(BUTTON_CANCEL) != 0) && (buttons.clLck == BUTTON_ENABLE) && (buttons.cnts.cntCl >= BUTTON_SHORT) && (buttons.cnts.cntCl < BUTTON_LONG)){
        btnCntsClr();
        power.uptimeCurr = 0;                                                   //Reset uptime counter
        return CANCEL;
    }else if((gppin_get(BUTTON_CANCEL) == 0) && (buttons.clLck == BUTTON_ENABLE) && (buttons.cnts.cntCl >= BUTTON_LONG)){
        btnCntsClr();
        buttons.clLck = BUTTON_DISABLE;
        return CLLNG;
    }else if((gppin_get(BUTTON_CANCEL) != 0) && (buttons.clLck == BUTTON_DISABLE) && (buttons.cnts.cntCl < BUTTON_SHORT)){
        buttons.clLck = BUTTON_ENABLE;
    }
    return NONE;
};

/*!****************************************************************************
* @brief    Clear buttons counters
* @param    
* @retval   
*/
void btnCntsClr(void){
    buttons.cnts.cntUp = 0;
    buttons.cnts.cntDn = 0;
    buttons.cnts.cntOK = 0;
    buttons.cnts.cntCl = 0;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
