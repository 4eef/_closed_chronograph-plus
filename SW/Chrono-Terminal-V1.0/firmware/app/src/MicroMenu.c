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
void Menu_commonRun(eNavEvent_type navEvent){
    
}

/*!****************************************************************************
* @brief    
*/
void Menu_listParse(Menu_Item_t* const NewMenu){
    Menu_Item_t *tmpItem, *tmpPrntItem;
    //Clear offset if passed to other level
    if((NewMenu == MENU_CHILD) || (NewMenu == MENU_PARENT)) menu.menuItems.wndOffs = 0;
    tmpItem = CurrentMenuItem;
    tmpPrntItem = tmpItem->Parent;
    strcpy(menu.menuItems.parent, tmpPrntItem->Text);
    //Get to the upper position of the menu list
    menu.menuItems.currItem = 0;
    while(1){
        menu.menuItems.currItem++;
        if(tmpItem->Previous == &NULL_MENU) break;
        tmpItem = tmpItem->Previous;
    }
    //Copy all strings on current level
    menu.menuItems.totItems = 0;
    while(1){
        strcpy(menu.menuItems.child[menu.menuItems.totItems], tmpItem->Text);
        menu.menuItems.totItems++;
        if(tmpItem->Next == &NULL_MENU) break;
        tmpItem = tmpItem->Next;
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
void Menu_putTxtWnd(void){
    
}

/*!****************************************************************************
* @brief    
*/
void Menu_putParWnd(eParType_type parType, eParFract_type parFract,
                    char *title, char *parUnits, char *parName,
                    int16_t parVal, int16_t brdMax, int16_t brdMin){
    //Set up menu mode
    menu.menuMode = eParEditWnd;
    //Copy parameters
    strcpy(menu.parEditWnd.title, title);
    strcpy(menu.parEditWnd.parUnits, parUnits);
    strcpy(menu.parEditWnd.parName, parName);
    menu.parEditWnd.parType = parType;
    menu.parEditWnd.parFract = parFract;
    menu.parEditWnd.parValue = parVal;
    menu.parEditWnd.parBorderMax = brdMax;
    menu.parEditWnd.parBorderMin = brdMin;
}

/*!****************************************************************************
* @brief    
*/
void Menu_rfrshParWnd(char *parName, int16_t parVal){
    if(menu.parEditWnd.parType == eNumber){
        menu.parEditWnd.parValue = parVal;
    }else{
        strcpy(menu.parEditWnd.parName, parName);
    }
}

/*!****************************************************************************
* @brief    
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
* @param    Pointer to the currently selected meny item
* @retval   
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
    if((NewMenu == &NULL_MENU) || (NewMenu == NULL)) return;
    CurrentMenuItem = NewMenu;
    Menu_listParse(NewMenu);
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
