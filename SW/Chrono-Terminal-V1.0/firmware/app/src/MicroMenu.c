/*!****************************************************************************
* @file    MicroMenu.c
* @author  4eef
* @version V1.0
* @date    09.12.2016
*/

/*!****************************************************************************
* Include
*/
#include "MicroMenu.h"

/*!****************************************************************************
* MEMORY
*/
menuItem_type               const NULL_MENU = {0};                              // Empty menu item
menuPrmtr_type              const NULL_PRM = {0};                               // Empty parameter description
menuItem_type               *currMenuItem = &NULL_MENU;
menu_type                   menu;

/*
TO DO List:
* Сделать ревизию на начало отсчета (0 или 1).
* Общий рефакторинг.
*/

/*!****************************************************************************
* @brief    
*/
void Menu_run(eNavEvent_type navEvent){
    if((navEvent != eWait) && (menu.pAnyBtnFunc != NULL)) menu.pAnyBtnFunc();
    switch(menu.menuMode){
        case eOff:
            Menu_navPwrOff(navEvent);
            break;
        case eDisplay:
            Menu_navDisp(navEvent);
            break;
        case eMenu:
            Menu_navMenu(navEvent);
            break;
        case eChooseFrmLstWnd:
            Menu_txtParSelWndRun(navEvent);
            break;
        case eParEditWnd:
            Menu_parWndRun(navEvent);
            break;
        case eTxtEditWnd:
            Menu_txtEditWndRun(navEvent);
            break;
        case eInfoWnd:
            Menu_infoWndRun(navEvent);
            break;
        default:
            break;
    }
    Menu_msgRun();
}

/*!****************************************************************************
* @brief    
*/
void Menu_navPwrOff(eNavEvent_type navEvent){
    switch(navEvent){
        case eBackLng:
            Menu_pwrSw(ePwrOn);
            break;
        default:
            break;
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_pwrSw(ePwrState_type ePwrState){
    if(ePwrState == ePwrOff){
        menu.menuMode = eOff;
        if(menu.pPwrOffFunc != NULL) menu.pPwrOffFunc();
    }else{
        menu.menuMode = eDisplay;
        if(menu.pPwrOnFunc != NULL) menu.pPwrOnFunc();
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_navDisp(eNavEvent_type navEvent){
    if(menu.confirm.show == true){
        Menu_exeConfirm(navEvent);
        return;
    }
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            if(menu.pBackBtnFunc != NULL) Menu_putConfirm(menu.pBackBtnFunc);
            break;
        case eBackLng:
            Menu_pwrSw(ePwrOff);
            break;
        case eUp:
            if(menu.pUpBtnFunc != NULL) Menu_putConfirm(menu.pUpBtnFunc);
            break;
        case eDown:
            if(menu.pDownBtnFunc != NULL) Menu_putConfirm(menu.pDownBtnFunc);
            break;
        case eOk:
            if(menu.pOkBtnFunc != NULL) Menu_putConfirm(menu.pOkBtnFunc);
            break;
        case eOkLng:
            menu.menuMode = eMenu;
            //Go to the root
            while(MENU_PARENT != &NULL_MENU){
                Menu_Navigate(MENU_PARENT);
            }
            //Navigate to first element in list
            while(MENU_PREVIOUS != &NULL_MENU){
                Menu_Navigate(MENU_PREVIOUS);
            }
            break;
        default:
            break;
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_navMenu(eNavEvent_type navEvent){
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            if(MENU_PARENT == &NULL_MENU){
                menu.menuMode = eDisplay;
            }else{
                Menu_Navigate(MENU_PARENT);
            }
            break;
        case eBackLng:
            menu.menuMode = eDisplay;
            break;
        case eUp:
            Menu_Navigate(MENU_PREVIOUS);
            break;
        case eDown:
            Menu_Navigate(MENU_NEXT);
            break;
        case eOk:
            if((MENU_ITEM_TYPE != eItem) && (MENU_PAR_DSCR != &NULL_PRM) && (MENU_PAR_DSCR != NULL)){
                Menu_setParEdit(navEvent);
            }else if((MENU_CHILD != &NULL_MENU) && (MENU_CHILD != NULL)){
                Menu_Navigate(MENU_CHILD);
            }
            break;
        case eOkLng:
            if((MENU_ITEM_TYPE != eItem) && (MENU_PAR_DSCR != &NULL_PRM) && (MENU_PAR_DSCR != NULL)){
                Menu_setParEdit(navEvent);
            }
            break;
        default:
            break;
    }
}

/*!****************************************************************************
* @brief    Put confirmation message on screen
*/
void Menu_putConfirm(void (*pFunc)(void)){
    menu.confirm.show = true;
    menu.confirm.pFunc = pFunc;
    Menu_putMsg("Confirm action?", MSG_LOCK);
}

/*!****************************************************************************
* @brief    Execute confirmation
*/
void Menu_exeConfirm(eNavEvent_type navEvent){
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            menu.confirm.show = false;
            Menu_msgClr();
            break;
        case eOk:
            menu.confirm.show = false;
            Menu_msgClr();
            menu.confirm.pFunc();
            break;
        default:
            break;
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_setParEdit(eNavEvent_type navEvent){
    switch(navEvent){
        case eOk:
            switch(MENU_ITEM_TYPE){
                case eChooseFrmLst:
                    Menu_putTxtParSelWnd(MENU_PAR_DSCR->pPar1, MENU_PAR_DSCR->pPar2, (MENU_PAR_DSCR->constPar1 - 1));
                    break;
                case eParEdit:
                    Menu_putParWnd(MENU_PAR_DSCR->pPar1, MENU_PAR_DSCR->pPar2, MENU_PAR_DSCR->pPar3,
                                   (eParFract_type)MENU_PAR_DSCR->constPar1, MENU_PAR_DSCR->constPar2, MENU_PAR_DSCR->constPar3);
                    break;
                case eParTxtEdit:
                    Menu_putParWnd(MENU_PAR_DSCR->pPar1, MENU_PAR_DSCR->pPar2, NULL,
                                   (eParFract_type)MENU_PAR_DSCR->constPar1, MENU_PAR_DSCR->constPar2, MENU_PAR_DSCR->constPar3);
                    break;
                case eInfoTxt:
                    Menu_putInfoWnd(MENU_PAR_DSCR->pPar1, MENU_PAR_DSCR->constPar1);
                    break;
                default:
                    break;
            }
            break;
        case eOkLng:
            switch(MENU_ITEM_TYPE){
                case eTxtEdit:
                    Menu_putTxtEditWnd(MENU_PAR_DSCR->pPar3);
                    break;
                case eParTxtEdit:
                    Menu_putTxtEditWnd(MENU_PAR_DSCR->pPar3);
                    break;
                case eFunc:
                    if(MENU_PAR_DSCR->pFunc != NULL){
                        MENU_PAR_DSCR->pFunc();
                    }else{
                        Menu_putMsg("No such function", MSG_CNT);
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_infoWndTxtSplit(void){
    bool textEnded = false;
    uint8_t i, newLine = 0, split = 0;
    //Copy string
    memset(menu.infoWindow.text, SYM_TERMINATOR_NO, MENU_MSG_LEN_MAX);
    strcpy(menu.infoWindow.text, menu.infoWindow.pString);
    //Get parameters
    if(menu.infoWindow.withPars == true){
        MENU_PAR_DSCR->pFunc();
    }
    //Split text to strings by setting terminator at end of strings
    while(1){
        //Find end of the string
        for(i = 0; i <= MENU_STR_LEN_MAX - 1; i++){
            if(menu.infoWindow.text[newLine + i] == SYM_SPACE_NO){
                split = i;
            }else if(menu.infoWindow.text[newLine + i] == SYM_LF_NO){
                split = i;
                break;
            }else if(menu.infoWindow.text[newLine + i] == SYM_TERMINATOR_NO){
                strcpy(menu.infoWindow.strings[menu.infoWindow.totStrs], &menu.infoWindow.text[newLine]);
                textEnded = true;
                break;
            }
        }
        //End of text
        if(textEnded == true) break;
        //Replace the space with terminator
        menu.infoWindow.text[newLine + split] = SYM_TERMINATOR_NO;
        //Copy the string
        strcpy(menu.infoWindow.strings[menu.infoWindow.totStrs], &menu.infoWindow.text[newLine]);
        //Modify position vars
        newLine += split + 1;
        if(menu.infoWindow.totStrs <= MENU_ITEMS_QTY_MAX){
            menu.infoWindow.totStrs++;
        }else{
            Menu_putMsg("Too much strings", MSG_CNT);
            menu.menuMode = menu.menuPrevMode;
            break;
        }
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_putInfoWnd(char *pString, bool withPars){
    //Check input parameters
    if(pString == NULL){
        Menu_putMsg("String ptr error", MSG_CNT);
        return;
    }else if(strlen(pString) >= MENU_MSG_LEN_MAX){
        Menu_putMsg("Msg is too long", MSG_CNT);
        return;
    }else if(strlen(pString) == 0){
        Menu_putMsg("String is empty", MSG_CNT);
        return;
    }else if((MENU_PAR_DSCR->pFunc == NULL) && (withPars == true)){
        Menu_putMsg("Func ptr error", MSG_CNT);
        return;
    }
    //Set parameters
    menu.menuPrevMode = menu.menuMode;
    menu.menuMode = eInfoWnd;
    strcpy(menu.infoWindow.title, MENU_ITEM_TEXT);
    menu.infoWindow.pString = pString;
    menu.infoWindow.withPars = withPars;
    menu.infoWindow.wndOffs = 0;
    menu.infoWindow.totStrs = 0;
    //Split text to strings
    Menu_infoWndTxtSplit();
}

/*!****************************************************************************
* @brief    
*/
void Menu_infoWndRun(eNavEvent_type navEvent){
    //Refresh strings contents
    if(menu.infoWindow.withPars == true) Menu_infoWndTxtSplit();
    //Navigate
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            menu.menuMode = menu.menuPrevMode;
            break;
        case eBackLng:
            break;
        case eUp:
            if((menu.infoWindow.totStrs >= (MENU_POSITIONS - 1)) && (menu.infoWindow.wndOffs > 0)){
                menu.infoWindow.wndOffs--;
            }
            break;
        case eDown:
            if((menu.infoWindow.totStrs >= (MENU_POSITIONS - 1)) && (menu.infoWindow.wndOffs < (menu.infoWindow.totStrs - (MENU_POSITIONS - 1)))){
                menu.infoWindow.wndOffs++;
            }
            break;
        case eOk:
            menu.menuMode = menu.menuPrevMode;
            break;
        case eOkLng:
            break;
        default:
            break;
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_putTxtParSelWnd(char *pFirstPar, uint16_t *pTxtParNumOrigin, uint16_t qtyTxtPar){
    //Set up menu mode
    menu.menuPrevMode = menu.menuMode;
    menu.menuMode = eChooseFrmLstWnd;
    //Copy parameters
    strcpy(menu.txtParSelWnd.title, MENU_ITEM_TEXT);
    strcpy(menu.txtParSelWnd.parText, (pFirstPar + (MENU_STR_LEN_MAX * (*pTxtParNumOrigin))));
    menu.txtParSelWnd.pFirstPar = pFirstPar;
    menu.txtParSelWnd.currTxtPar = *pTxtParNumOrigin;
    menu.txtParSelWnd.pTxtParNumOrigin = pTxtParNumOrigin;
    menu.txtParSelWnd.qtyTxtPar = qtyTxtPar;
}

/*!****************************************************************************
* @brief    
*/
void Menu_txtParSelWndRun(eNavEvent_type navEvent){
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            menu.menuMode = menu.menuPrevMode;
            Menu_putMsg("Cancelled", MSG_CNT);
            break;
        case eBackLng:
            break;
        case eUp:
            if(menu.txtParSelWnd.currTxtPar >= menu.txtParSelWnd.qtyTxtPar){
                menu.txtParSelWnd.currTxtPar = TXT_PAR_MIN_VAL;
            }else{
                menu.txtParSelWnd.currTxtPar++;
            }
            break;
        case eDown:
            if(menu.txtParSelWnd.currTxtPar <= TXT_PAR_MIN_VAL){
                menu.txtParSelWnd.currTxtPar = menu.txtParSelWnd.qtyTxtPar;
            }else{
                menu.txtParSelWnd.currTxtPar--;
            }
            break;
        case eOk:
            if(menu.txtParSelWnd.pTxtParNumOrigin != NULL){
                *menu.txtParSelWnd.pTxtParNumOrigin = menu.txtParSelWnd.currTxtPar;
                Menu_putMsg("Saved", MSG_CNT);
            }else{
                Menu_putMsg("Ptr. error", MSG_CNT);
            }
            menu.menuMode = menu.menuPrevMode;
            break;
        case eOkLng:
            break;
        default:
            break;
    }
    if((navEvent == eUp) || (navEvent == eDown)){
        strcpy(menu.txtParSelWnd.parText, (menu.txtParSelWnd.pFirstPar + (MENU_STR_LEN_MAX * menu.txtParSelWnd.currTxtPar)));
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_putTxtEditWnd(char *pStrOrig){
    //Set up menu mode
    menu.menuPrevMode = menu.menuMode;
    menu.menuMode = eTxtEditWnd;
    //Copy parameters
    strcpy(menu.txtEditWnd.title, MENU_ITEM_TEXT);
    memset(menu.txtEditWnd.string, SYM_TERMINATOR_NO, MENU_STR_LEN_MAX);
    strcpy(menu.txtEditWnd.string, pStrOrig);
    menu.txtEditWnd.pStrOrig = pStrOrig;
    menu.txtEditWnd.symPos = strlen(menu.txtEditWnd.string) - 1;
}

/*!****************************************************************************
* @brief    
*/
void Menu_txtEditWndRun(eNavEvent_type navEvent){
    uint8_t i;
    char prevSym;
    prevSym = menu.txtEditWnd.string[menu.txtEditWnd.symPos];
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            if(menu.txtEditWnd.string[menu.txtEditWnd.symPos] != SYM_TERMINATOR_NO){
                if(menu.txtEditWnd.string[menu.txtEditWnd.symPos + 1] != SYM_TERMINATOR_NO){
                    i = menu.txtEditWnd.symPos;
                    while(menu.txtEditWnd.string[i] != SYM_TERMINATOR_NO){
                        menu.txtEditWnd.string[i] = menu.txtEditWnd.string[i + 1];
                        i++;
                    }
                }else{
                    menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_TERMINATOR_NO;
                }
            }else{
                Menu_putMsg("String is empty", MSG_CNT);
            }
            if(menu.txtEditWnd.symPos != 0){
                menu.txtEditWnd.symPos--;
            }
            break;
        case eBackLng:
            menu.menuMode = menu.menuPrevMode;
            Menu_putMsg("Cancelled", MSG_CNT);
            break;
        case eUp:
            prevSym++;
            if(prevSym < SYM_SPACE_NO){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_SPACE_NO;
            }else if((prevSym < SYM_POINT_NO) && (prevSym > SYM_SPACE_NO)){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_POINT_NO;
            }else if((prevSym < SYM_ZERO_NO) && (prevSym > SYM_POINT_NO)){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_ZERO_NO;
            }else if((prevSym < SYM_ABIG_NO) && (prevSym > SYM_NINE_NO)){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_ABIG_NO;
            }else if((prevSym < SYM_ASMALL_NO) && (prevSym > SYM_ZBIG_NO)){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_ASMALL_NO;
            }else if(prevSym > SYM_ZSMALL_NO){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_SPACE_NO;
            }else{
                menu.txtEditWnd.string[menu.txtEditWnd.symPos]++;
            }
            break;
        case eDown:
            prevSym--;
            if(prevSym > SYM_ZSMALL_NO){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_ZSMALL_NO;
            }else if((prevSym > SYM_ZBIG_NO) && (prevSym < SYM_ASMALL_NO)){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_ZBIG_NO;
            }else if((prevSym > SYM_NINE_NO) && (prevSym < SYM_ABIG_NO)){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_NINE_NO;
            }else if((prevSym > SYM_POINT_NO) && (prevSym < SYM_ZERO_NO)){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_POINT_NO;
            }else if((prevSym > SYM_SPACE_NO) && (prevSym < SYM_POINT_NO)){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_SPACE_NO;
            }else if(prevSym < SYM_SPACE_NO){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_ZSMALL_NO;
            }else{
                menu.txtEditWnd.string[menu.txtEditWnd.symPos]--;
            }
            break;
        case eOk:
            if(!(((menu.txtEditWnd.symPos == 0) && (menu.txtEditWnd.string[menu.txtEditWnd.symPos] == SYM_TERMINATOR_NO)) ||
                 ((menu.txtEditWnd.symPos == 0) && (menu.txtEditWnd.string[menu.txtEditWnd.symPos] == SYM_SPACE_NO)))){
                if((menu.txtEditWnd.symPos >= (MENU_STR_LEN_MAX - 2)) ||
                   ((menu.txtEditWnd.string[menu.txtEditWnd.symPos - 1] == SYM_SPACE_NO) &&
                    (menu.txtEditWnd.string[menu.txtEditWnd.symPos] == SYM_SPACE_NO))){
                    menu.txtEditWnd.symPos = 0;
                }else{
                    menu.txtEditWnd.symPos++;
                    if(menu.txtEditWnd.string[menu.txtEditWnd.symPos] == SYM_TERMINATOR_NO){
                        menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_SPACE_NO;
                    }
                }
            }
            break;
        case eOkLng:
            i = strlen(menu.txtEditWnd.string) - 1;
            while(menu.txtEditWnd.string[i] == SYM_SPACE_NO){
                menu.txtEditWnd.string[i] = SYM_TERMINATOR_NO;
                i--;
            }
            if(strlen(menu.txtEditWnd.string) == 0){
                Menu_putMsg("String is empty", MSG_CNT);
            }else if(menu.txtEditWnd.pStrOrig != NULL){
                menu.menuMode = menu.menuPrevMode;
                strcpy(menu.txtEditWnd.pStrOrig, menu.txtEditWnd.string);
                Menu_putMsg("Saved", MSG_CNT);
            }else{
                Menu_putMsg("Ptr. error", MSG_CNT);
            }
            break;
        default:
            break;
    }
}

/*!****************************************************************************
* @brief    Configure menu to display a parameter edit window
*/
void Menu_putParWnd(char *parUnits, uint16_t *pParOrigin, uint16_t *pParCopy,
                    eParFract_type parFract, int16_t brdMax, int16_t brdMin){
    //Set up menu mode
    menu.menuPrevMode = menu.menuMode;
    menu.menuMode = eParEditWnd;
    //Copy parameters
    strcpy(menu.parEditWnd.title, MENU_ITEM_TEXT);
    strcpy(menu.parEditWnd.parUnits, parUnits);
    menu.parEditWnd.pParOrigin = pParOrigin;
    menu.parEditWnd.pParCopy = pParCopy;
    menu.parEditWnd.parFract = parFract;
    menu.parEditWnd.parValue = *pParOrigin;
    menu.parEditWnd.parBorderMax = brdMax;
    menu.parEditWnd.parBorderMin = brdMin;
}

/*!****************************************************************************
* @brief    Parameter edit window run function
*/
void Menu_parWndRun(eNavEvent_type navEvent){
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            menu.menuMode = menu.menuPrevMode;
            Menu_putMsg("Cancelled", MSG_CNT);
            break;
        case eBackLng:
            break;
        case eUp:
            if(menu.parEditWnd.parValue >= menu.parEditWnd.parBorderMax){
                menu.parEditWnd.parValue = menu.parEditWnd.parBorderMin;
            }else{
                menu.parEditWnd.parValue++;
            }
            break;
        case eDown:
            if(menu.parEditWnd.parValue <= menu.parEditWnd.parBorderMin){
                menu.parEditWnd.parValue = menu.parEditWnd.parBorderMax;
            }else{
                menu.parEditWnd.parValue--;
            }
            break;
        case eOk:
            if(menu.parEditWnd.pParOrigin != NULL){
                *menu.parEditWnd.pParOrigin = menu.parEditWnd.parValue;
                Menu_putMsg("Saved", MSG_CNT);
            }else{
                Menu_putMsg("Ptr. error", MSG_CNT);
            }
            if(menu.parEditWnd.pParCopy != NULL){
                *menu.parEditWnd.pParCopy = menu.parEditWnd.parValue;
            }
            menu.menuMode = menu.menuPrevMode;
            break;
        case eOkLng:
            break;
        default:
            break;
    }
}

/*!****************************************************************************
* @brief    Run message routine
*/
void Menu_msgRun(void){
    if((menu.message.show == true) && (menu.message.lock != true)){
        if(menu.message.msgCnt != 0){
            menu.message.msgCnt--;
        }else{
            menu.message.show = false;
        }
    }
}

/*!****************************************************************************
* @brief    Remove message from screen
*/
void Menu_msgClr(void){
    menu.message.lock = false;
    menu.message.show = false;
}

/*!****************************************************************************
* @brief    Put message on screen
*/
void Menu_putMsg(char *str, uint8_t msgCnt){
    if(str == NULL) return;
    strcpy(menu.message.msgStr, str);
    menu.message.msgLen = strlen(menu.message.msgStr);
    menu.message.msgCnt = msgCnt;
    if(msgCnt == 0){
        menu.message.lock = true;
    }else{
        menu.message.lock = false;
    }
    menu.message.show = true;
}

/*!****************************************************************************
* @brief    Retrieves the currently selected meny item
*/
menuItem_type* Menu_GetCurrentMenu(void){
	return currMenuItem;
}

/*!****************************************************************************
* @brief    Navigates to an absolute or relative menu entry, gets menu data to a structure
* @param    Pointer to the absolute menu item to select, or one of \ref MENU_PARENT,
*           \ref MENU_CHILD, \ref MENU_NEXT or \ref MENU_PREVIOUS for relative navigation.
* @retval   
*/
void Menu_Navigate(menuItem_type* const NewMenu){
    menuItem_type *tmpItem, *tmpPrntItem;
    if((NewMenu == &NULL_MENU) || (NewMenu == NULL)) return;
    //Clear offset if passed to other level
    if((NewMenu == MENU_CHILD) || (NewMenu == MENU_PARENT)) menu.menuItems.wndOffs = 0;
    //Save new menu item
    tmpItem = currMenuItem = NewMenu;
    //Parse menu list parameters
    //Menu title
    if(tmpItem->parent != &NULL_MENU){
        tmpPrntItem = tmpItem->parent;
        strcpy(menu.menuItems.parent, tmpPrntItem->text);
    }else{
        strcpy(menu.menuItems.parent, "Menu");
    }
    //Get to the upper position of the menu list
    menu.menuItems.currItem = 0;
    while(1){
        menu.menuItems.currItem++;
        if(tmpItem->previous == &NULL_MENU) break;
        tmpItem = tmpItem->previous;
    }
    //Window offset calculation
    if((menu.menuItems.currItem - menu.menuItems.wndOffs - MENU_POSITIONS) > 0){
        menu.menuItems.wndOffs = menu.menuItems.currItem - MENU_POSITIONS;
    }else if((menu.menuItems.wndOffs - menu.menuItems.currItem + 1) > 0){
        menu.menuItems.wndOffs = menu.menuItems.currItem - 1;
    }
    //Copy all strings on current level
    menu.menuItems.totItems = 0;
    while(1){
        strcpy(menu.menuItems.child[menu.menuItems.totItems], tmpItem->text);
        menu.menuItems.totItems++;
        if(tmpItem->next == &NULL_MENU) break;
        tmpItem = tmpItem->next;
    }
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
