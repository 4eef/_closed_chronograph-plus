/*!****************************************************************************
* @file    MicroMenu.h
* @author  4eef
* @version V1.0
* @date    09.12.2016, 4eef; Dean Camera, 2012
* @brief   --
*/
#ifndef MicroMenu_H
#define MicroMenu_H

/*!****************************************************************************
* Include
*/
#include "stddef.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "ug2864.h"

/*!****************************************************************************
* User define
*/
#define MENU_POSITIONS          5
#define MENU_INTERVAL           10
#define MENU_START              12
#define PAR_NONE                0
#define PAR_SAVE                1
#define PAR_CANCEL              2
#define PAR_EDIT_DISABLE        0
#define PAR_EDIT_ENABLE         1
#define PAR_UNSIGNED            0
#define PAR_SIGNED              1
#define MSG_FOR_TIMER           2
#define MSG_CNT                 30
#define MSG_CNT_LONG            60
#define MSG_CNT_BIND            100

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef enum{
    eDisplay = 0,
    eMenu,
    eParEditWnd,
    eTxtEditWnd,
    eInfoWnd
}eMenuMode_type;

typedef enum{
    eNumber = 0,
    eText
}eParType_type;

typedef enum{
    eNoFract = 1,
    eTenths = 10,
    eHundreds = 100
}eParFract_type;

typedef enum{
    eWait = 0,
    eBack,
    eBackLng,
    eUp,
    eDown,
    eOk,
    eOkLng
}eNavEvent_type;

typedef struct{
    bool            toDisplay;
    char            msgStr[20];
    uint8_t         msgLen;
    uint8_t         msgCnt;
}message_type;

typedef struct{
    eParType_type   parType;
    char            title[20];
    char            parUnits[5];
    char            parText[20];
    uint16_t        parValue;
    uint16_t        parBorderMax;
    uint16_t        parBorderMin;
    uint16_t        *pParOrigin;
    eParFract_type  parFract;
}parEditWnd_type;

typedef struct{
    char            title[20];
    char            string[20];
    char            symbol;
    uint8_t         symPos;
}txtEditWnd_type;

typedef struct{
    char            title[20];
    char            string[240];
    uint8_t         totStrs;
    uint8_t         wndOffs;
}infoWnd_type;

typedef struct{
    char            parent[20];
    char            child[10][20];
    uint8_t         currItem;
    uint8_t         totItems;
    uint8_t         wndOffs;
}menuItems_type;

typedef struct{
    message_type    message;
    parEditWnd_type parEditWnd;
    txtEditWnd_type txtEditWnd;
    infoWnd_type    infoWindow;
    menuItems_type  menuItems;
    eMenuMode_type  menuMode;
    eMenuMode_type  menuPrevMode;
    eNavEvent_type  navEvent;
}menu_type;

/** Type define for a menu item. Menu items should be initialized via the helper
 *  macro \ref MENU_ITEM(), not created from this type directly in user-code.
 */
typedef const struct Menu_Item {
    const struct Menu_Item *Next; /**< Pointer to the next menu item of this menu item */
    const struct Menu_Item *Previous; /**< Pointer to the previous menu item of this menu item */
    const struct Menu_Item *Parent; /**< Pointer to the parent menu item of this menu item */
    const struct Menu_Item *Child; /**< Pointer to the child menu item of this menu item */
    void (*SelectCallback)(void); /**< Pointer to the optional menu-specific select callback of this menu item */
    void (*EnterCallback)(void); /**< Pointer to the optional menu-specific enter callback of this menu item */
    char Text[]; /**< Menu item text to pass to the menu display callback function */
} Menu_Item_t;

/*!****************************************************************************
* Extern viriables
*/
/** Null menu entry, used in \ref MENU_ITEM() definitions where no menu link is to be made. */
extern Menu_Item_t const NULL_MENU;

/*!****************************************************************************
* Macro functions
*/
/** Creates a new menu item entry with the specified links and callbacks.
 *
 *  \param[in] Name      Name of the menu entry, must be unique.
 *  \param[in] Next      Name of the next linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] Previous  Name of the previous linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] Parent    Name of the parent linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] Child     Name of the child linked menu item, or \ref NULL_MENU if no menu link.
 *  \param[in] SelectFunc  Function callback to execute when the menu item is selected, or \c NULL for no callback.
 *  \param[in] EnterFunc   Function callback to execute when the menu item is entered, or \c NULL for no callback.
 */
#define MENU_ITEM(Name, Next, Previous, Parent, Child, SelectFunc, EnterFunc, Text) \
    extern Menu_Item_t const Next;      \
    extern Menu_Item_t const Previous;  \
    extern Menu_Item_t const Parent;    \
    extern Menu_Item_t const Child;     \
    Menu_Item_t const Name = {&Next, &Previous, &Parent, &Child, SelectFunc, EnterFunc, Text}

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu parent. */
#define MENU_PARENT         Menu_GetCurrentMenu()->Parent

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the menu child. */
#define MENU_CHILD          Menu_GetCurrentMenu()->Child

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the next linked menu item. */
#define MENU_NEXT           Menu_GetCurrentMenu()->Next

/** Relative navigational menu entry for \ref Menu_Navigate(), to move to the previous linked menu item. */
#define MENU_PREVIOUS       Menu_GetCurrentMenu()->Previous

/*!****************************************************************************
* Prototypes for the functions
*/
Menu_Item_t* Menu_GetCurrentMenu(void);
void Menu_Navigate(Menu_Item_t* const NewMenu);
//void Menu_SetGenericWriteCallback(void (*WriteFunc)(const char* Text));
void Menu_EnterCurrentItem(void);
//void Generic_Write(const char* Text);
void Menu_listParse(Menu_Item_t* const NewMenu);
void Menu_putMessage(char *newStr, uint8_t newCnt);
void Menu_parWndRun(eNavEvent_type navEvent);
void Menu_putParWnd(eParType_type parType, eParFract_type parFract,
                    char *title, char *parUnits, char *parText,
                    uint16_t *pParOrigin, int16_t brdMax, int16_t brdMin);
void Menu_infoWndRun(eNavEvent_type navEvent);
void Menu_txtWndRun(eNavEvent_type navEvent);
void Menu_navRun(eNavEvent_type navEvent);
void Menu_run(void);

#endif //MicroMenu_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
