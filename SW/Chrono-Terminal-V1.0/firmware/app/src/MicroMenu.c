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
menuStrs_type           menuStrs;

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
    Menu_Item_t *tmpItem, *tmpParItem;
    uint8_t i;
    
    if((NewMenu == &NULL_MENU) || (NewMenu == NULL)) return;
    if((NewMenu == MENU_CHILD) || (NewMenu == MENU_PARENT)) menuStrs.off = 0;//Clear offset if passed to other level
    CurrentMenuItem = NewMenu;
    tmpItem = CurrentMenuItem;
    //Get to the upper position of the menu list
    i = 0;
    while(1){
        tmpParItem = tmpItem->Parent;
        i++;                                                    //Current menu item
        menuStrs.currItem = i;
        if(tmpItem->Previous == &NULL_MENU) break;
        tmpItem = tmpItem->Previous;
    }
    //Clear strings
    for(i = 0; i < 10; i++){
        memset(menuStrs.child[i], 0, sizeof(menuStrs.child[i]));
    }
    //Copy all menu listing
    strcpy(menuStrs.parent, tmpParItem->Text);                  //Parent text string
    i = 0;
    while(1){                                                   //Child text strings
        strcpy(menuStrs.child[i], tmpItem->Text);
        i++;
        menuStrs.totItems = i;                                  //Total menu elements
        if(tmpItem->Next == &NULL_MENU) break;
        tmpItem = tmpItem->Next;
    }
    
    //if(MenuWriteFunc) MenuWriteFunc(CurrentMenuItem->Text);
    
    void (*SelectCallback)(void) = CurrentMenuItem->SelectCallback;
    
    if(SelectCallback) SelectCallback();
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
    if ((CurrentMenuItem == &NULL_MENU) || (CurrentMenuItem == NULL))
        return;
    
    void (*EnterCallback)(void) = CurrentMenuItem->EnterCallback;
    
    if (EnterCallback)
        EnterCallback();
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
