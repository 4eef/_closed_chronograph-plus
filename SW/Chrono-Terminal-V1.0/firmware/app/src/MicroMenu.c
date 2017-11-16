/*!****************************************************************************
* @file    MicroMenu.c
* @author  4eef
* @version V1.0
* @date    09.12.2016, 4eef; Dean Camera, 2012
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "MicroMenu.h"

/*!****************************************************************************
* MEMORY
*/
menu_type                   menu;
extern power_type           power;

/** This is used when an invalid menu handle is required in
 *  a \ref MENU_ITEM() definition, i.e. to indicate that a
 *  menu has no linked parent, child, next or previous entry.
 */
Menu_Item_t const NULL_MENU = {0};

/** \internal
 *  Pointer to the generic menu text display function
 *  callback, to display the configured text of a menu item
 *  if no menu-specific display function has been set
 *  in the select menu item.
 */
//static void (*MenuWriteFunc)(const char* Text) = NULL;

/** \internal
 *  Pointer to the currently selected menu item.
 */
static Menu_Item_t* CurrentMenuItem = &NULL_MENU;

/*!****************************************************************************
* @brief    
*/
void Menu_run(eNavEvent_type navEvent){
    if(navEvent != eWait) power.uptimeCurr = 0;                                 //Reset uptime counter
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
        case eParEditWnd:
            Menu_parWndRun(navEvent);
            break;
        case eTxtParSelWnd:
            Menu_txtParSelWndRun(navEvent);
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
}

/*!****************************************************************************
* @brief    
*/
void Menu_pwrOnOff(ePwrState_type ePwrState){
    if(ePwrState == ePwrOff){
        menu.menuMode = eOff;
        powerOff();
    }else{
        menu.menuMode = eDisplay;
        powerOn();
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_navPwrOff(eNavEvent_type navEvent){
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            break;
        case eBackLng:
            Menu_pwrOnOff(ePwrOn);
            break;
        case eUp:
            break;
        case eDown:
            break;
        case eOk:
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
void Menu_navDisp(eNavEvent_type navEvent){
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            break;
        case eBackLng:
            Menu_pwrOnOff(ePwrOff);
            break;
        case eUp:
            break;
        case eDown:
            break;
        case eOk:
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
        case eOkLng:
            chrSetsRst();
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
            if((MENU_CHILD == &NULL_MENU) || (MENU_CHILD == NULL)){
                Menu_EnterCurrentItem();
            }else{
                Menu_Navigate(MENU_CHILD);
            }
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
void Menu_putInfoWnd(void){
    
}

/*!****************************************************************************
* @brief    
*/
void Menu_infoWndRun(eNavEvent_type navEvent){
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            break;
        case eBackLng:
            break;
        case eUp:
            break;
        case eDown:
            break;
        case eOk:
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
void Menu_putTxtParSelWnd(char *title, char *pFirstPar, uint16_t *pTxtParOrigin,
                          uint16_t currTxtPar, uint16_t qtyTxtPar, uint16_t txtStrLen){
    //Set up menu mode
    menu.menuPrevMode = menu.menuMode;
    menu.menuMode = eTxtParSelWnd;
    //Copy parameters
    strcpy(menu.txtParSelWnd.title, title);
    strcpy(menu.txtParSelWnd.parText, (pFirstPar + (txtStrLen * (currTxtPar - 1))));
    menu.txtParSelWnd.pFirstPar = pFirstPar;
    menu.txtParSelWnd.txtStrLen = txtStrLen;
    menu.txtParSelWnd.currTxtPar = currTxtPar;
    menu.txtParSelWnd.pTxtParOrigin = pTxtParOrigin;
    menu.txtParSelWnd.qtyTxtPar = qtyTxtPar;
}

/*!****************************************************************************
* @brief    
*/
void Menu_txtParSelWndRun(eNavEvent_type navEvent){
    char *pFirstPar;
    uint8_t txtStrLen, currTxtPar;
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            menu.menuMode = menu.menuPrevMode;
            Menu_putMessage("Cancelled", MSG_CNT);
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
            if(menu.txtParSelWnd.pTxtParOrigin != NULL){
                *menu.txtParSelWnd.pTxtParOrigin = menu.txtParSelWnd.currTxtPar;
                Menu_putMessage("Saved", MSG_CNT);
            }else{
                Menu_putMessage("Ptr. error", MSG_CNT);
            }
            menu.menuMode = menu.menuPrevMode;
            break;
        case eOkLng:
            break;
        default:
            break;
    }
    if((navEvent == eUp) || (navEvent == eDown)){
        pFirstPar = menu.txtParSelWnd.pFirstPar;
        txtStrLen = menu.txtParSelWnd.txtStrLen;
        currTxtPar = menu.txtParSelWnd.currTxtPar;
        strcpy(menu.txtParSelWnd.parText, (pFirstPar + (txtStrLen * (currTxtPar - 1))));
    }
}

/*!****************************************************************************
* @brief    
*/
void Menu_putTxtEditWnd(char *title, char *pStrOrig){
    //Set up menu mode
    menu.menuPrevMode = menu.menuMode;
    menu.menuMode = eTxtEditWnd;
    //Copy parameters
    strcpy(menu.txtEditWnd.title, title);
    strcpy(menu.txtEditWnd.string, pStrOrig);
    menu.txtEditWnd.string[MENU_STR_LEN_MAX - 1] = 0;
    menu.txtEditWnd.pStrOrig = pStrOrig;
}

/*!****************************************************************************
* @brief    
*/
void Menu_txtEditWndRun(eNavEvent_type navEvent){
    uint8_t i;
    switch(navEvent){
        case eWait:
            break;
        case eBack:
            if(menu.txtEditWnd.symPos == 0){
                Menu_putMessage("Pos. error", MSG_CNT);
            }else{
                for(i = menu.txtEditWnd.symPos; i < (MENU_STR_LEN_MAX - 1); i++){
                    menu.txtEditWnd.string[i] = 0;
                }
                Menu_putMessage("Cleared", MSG_CNT);
            }
            break;
        case eBackLng:
            menu.menuMode = menu.menuPrevMode;
            Menu_putMessage("Cancelled", MSG_CNT);
            break;
        case eUp:
            if(menu.txtEditWnd.string[menu.txtEditWnd.symPos] >= SYM_ZSMALL_NO){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_SPACE_NO;
            }else{
                menu.txtEditWnd.string[menu.txtEditWnd.symPos]++;
            }
            break;
        case eDown:
            if(menu.txtEditWnd.string[menu.txtEditWnd.symPos] <= SYM_SPACE_NO){
                menu.txtEditWnd.string[menu.txtEditWnd.symPos] = SYM_ZSMALL_NO;
            }else{
                menu.txtEditWnd.string[menu.txtEditWnd.symPos]--;
            }
            break;
        case eOk:
            if(menu.txtEditWnd.symPos >= (MENU_STR_LEN_MAX - 1)){
                menu.txtEditWnd.symPos = 0;
            }else{
                menu.txtEditWnd.symPos++;
            }
            break;
        case eOkLng:
            if(menu.txtEditWnd.pStrOrig != NULL){
                strcpy(menu.txtEditWnd.pStrOrig, menu.txtEditWnd.string);
                Menu_putMessage("Saved", MSG_CNT);
            }else{
                Menu_putMessage("Ptr. error", MSG_CNT);
            }
            break;
        default:
            break;
    }
}

/*!****************************************************************************
* @brief    Configure menu to display a parameter edit window
*/
void Menu_putParWnd(char *title, char *parUnits, eParFract_type parFract,
                    uint16_t *pParOrigin, uint16_t *pParCopy,
                    int16_t brdMax, int16_t brdMin){
    //Set up menu mode
    menu.menuPrevMode = menu.menuMode;
    menu.menuMode = eParEditWnd;
    //Copy parameters
    strcpy(menu.parEditWnd.title, title);
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
            Menu_putMessage("Cancelled", MSG_CNT);
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
                Menu_putMessage("Saved", MSG_CNT);
            }else{
                Menu_putMessage("Ptr. error", MSG_CNT);
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
* @brief    Put message on screen
*/
void Menu_putMessage(char *newStr, uint8_t newCnt){
    if((newStr != NULL) && (newCnt != 0)){
        strcpy(menu.message.msgStr, newStr);
        menu.message.msgLen = strlen(menu.message.msgStr);
        menu.message.msgCnt = newCnt;
        menu.message.toDisplay = true;
    }
}

/*!****************************************************************************
* @brief    Retrieves the currently selected meny item
*/
Menu_Item_t* Menu_GetCurrentMenu(void){
	return CurrentMenuItem;
}

/*!****************************************************************************
* @brief    Navigates to an absolute or relative menu entry, gets menu data to a structure
* @param    Pointer to the absolute menu item to select, or one of \ref MENU_PARENT,
*           \ref MENU_CHILD, \ref MENU_NEXT or \ref MENU_PREVIOUS for relative navigation.
* @retval   
*/
void Menu_Navigate(Menu_Item_t* const NewMenu){
    Menu_Item_t *tmpItem, *tmpPrntItem;
    if((NewMenu == &NULL_MENU) || (NewMenu == NULL)) return;
    //Clear offset if passed to other level
    if((NewMenu == MENU_CHILD) || (NewMenu == MENU_PARENT)) menu.menuItems.wndOffs = 0;
    //Save new menu item
    tmpItem = CurrentMenuItem = NewMenu;
    //Parse menu list parameters
    //Menu title
    if(tmpItem->Parent != &NULL_MENU){
        tmpPrntItem = tmpItem->Parent;
        strcpy(menu.menuItems.parent, tmpPrntItem->Text);
    }else{
        strcpy(menu.menuItems.parent, "Menu");
    }
    //Get to the upper position of the menu list
    menu.menuItems.currItem = 0;
    while(1){
        menu.menuItems.currItem++;
        if(tmpItem->Previous == &NULL_MENU) break;
        tmpItem = tmpItem->Previous;
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
        strcpy(menu.menuItems.child[menu.menuItems.totItems], tmpItem->Text);
        menu.menuItems.totItems++;
        if(tmpItem->Next == &NULL_MENU) break;
        tmpItem = tmpItem->Next;
    }
//    if(MenuWriteFunc) MenuWriteFunc(CurrentMenuItem->Text);
//    void (*SelectCallback)(void) = CurrentMenuItem->SelectCallback;
//    if(SelectCallback) SelectCallback();
}

/*!****************************************************************************
* @brief    Configures the menu text write callback function, fired for all menu items. Within this callback
*           function the user should implement code to display the current menu text stored in \ref MENU_ITEM_STORAGE
*           memory space.
* @param    Pointer to a callback function to execute for each selected menu item.
* @retval   
*/
//void Menu_SetGenericWriteCallback(void (*WriteFunc)(const char* Text)){
//    MenuWriteFunc = WriteFunc;
//    Menu_Navigate(CurrentMenuItem);
//}

/*!****************************************************************************
* @brief    Enters the currently selected menu item, running its configured callback function (if any).
* @param    
* @retval   
*/
void Menu_EnterCurrentItem(void){
    if((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL)) return;
    
    void (*EnterCallback)(void) = CurrentMenuItem->EnterCallback;
    
    if(EnterCallback) EnterCallback();
}

/*!****************************************************************************
* @brief    Generic function to write the text of a menu
* @param    Text of the selected menu to write, in \ref MENU_ITEM_STORAGE memory space
* @retval   
*/
//void Generic_Write(const char* Text){
//    if (Text)
//        puts(Text);
//}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
