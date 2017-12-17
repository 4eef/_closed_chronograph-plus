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
#include "power.h"
#include "buttons.h"

/*!****************************************************************************
* User define
*/
#define MENU_POSITIONS          5
#define MENU_INTERVAL           10
#define MENU_START              12
#define MENU_STR_LEN_MAX        20
#define MENU_MSG_LEN_MAX        240
#define MENU_ITEMS_QTY_MAX      20
#define TXT_PAR_MIN_VAL         1
#define MSG_FOR_TIMER           2
#define MSG_CNT                 30
#define MSG_CNT_LONG            60
#define MSG_CNT_BIND            100
#define SYM_TERMINATOR_NO       0
#define SYM_SPACE_NO            32
#define SYM_POINT_NO            46
#define SYM_ZERO_NO             48
#define SYM_NINE_NO             57
#define SYM_ABIG_NO             65
#define SYM_ZBIG_NO             90
#define SYM_ASMALL_NO           97
#define SYM_ZSMALL_NO           122

/*!****************************************************************************
* User typedef
*/
typedef enum{
    ePwrOff = 0,
    ePwrOn
}ePwrState_type;

typedef enum{
    eOff = 0,
    eDisplay,
    eMenu,
    eParEditWnd,
    eTxtParSelWnd,
    eTxtEditWnd,
    eInfoWnd
}eMenuMode_type;

typedef enum{
    eNoFract = 1,
    eTenths = 10,
    eHundreds = 100
}eParFract_type;

typedef struct{
    bool            toDisplay;
    char            msgStr[MENU_STR_LEN_MAX];
    uint8_t         msgLen;
    uint8_t         msgCnt;
}message_type;

typedef struct{
    char            title[MENU_STR_LEN_MAX];
    char            parUnits[MENU_STR_LEN_MAX];
    uint16_t        parValue;
    uint16_t        parBorderMax;
    uint16_t        parBorderMin;
    uint16_t        *pParOrigin;
    uint16_t        *pParCopy;
    eParFract_type  parFract;
}parEditWnd_type;

typedef struct{
    char            title[MENU_STR_LEN_MAX];
    char            *pFirstPar;
    char            parText[MENU_STR_LEN_MAX];
    uint16_t        txtStrLen;
    uint16_t        currTxtPar;
    uint16_t        *pTxtParOrigin;
    uint16_t        qtyTxtPar;
}txtParSelWnd_type;

typedef struct{
    char            title[MENU_STR_LEN_MAX];
    char            string[MENU_STR_LEN_MAX];
    char            *pStrOrig;
    uint8_t         symPos;
    uint8_t         strMaxLen;
}txtEditWnd_type;

typedef struct{
    char            title[MENU_STR_LEN_MAX];
    char            string[MENU_MSG_LEN_MAX];
    uint8_t         totStrs;
    uint8_t         wndOffs;
}infoWnd_type;

typedef struct{
    char            parent[MENU_STR_LEN_MAX];
    char            child[MENU_STR_LEN_MAX][MENU_ITEMS_QTY_MAX];
    uint8_t         currItem;
    uint8_t         totItems;
    uint8_t         wndOffs;
}menuItems_type;

typedef struct{
    message_type        message;
    parEditWnd_type     parEditWnd;
    txtParSelWnd_type   txtParSelWnd;
    txtEditWnd_type     txtEditWnd;
    infoWnd_type        infoWindow;
    menuItems_type      menuItems;
    eMenuMode_type      menuMode;
    eMenuMode_type      menuPrevMode;
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
    char *Text; /**< Menu item text to pass to the menu display callback function */
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
void Menu_putParWnd(char *title, char *parUnits, eParFract_type parFract,
                    uint16_t *pParOrigin, uint16_t *pParCopy,
                    int16_t brdMax, int16_t brdMin);
void Menu_infoWndRun(eNavEvent_type navEvent);
void Menu_txtEditWndRun(eNavEvent_type navEvent);
void Menu_putTxtEditWnd(char *title, char *pStrOrig, uint8_t strMaxLen);
void Menu_txtParSelWndRun(eNavEvent_type navEvent);
void Menu_putTxtParSelWnd(char *title, char *pFirstPar, uint16_t *pTxtParOrigin,
                          uint16_t currTxtPar, uint16_t qtyTxtPar, uint16_t txtStrLen);
void Menu_navMenu(eNavEvent_type navEvent);
void Menu_navDisp(eNavEvent_type navEvent);
void Menu_navPwrOff(eNavEvent_type navEvent);
void Menu_pwrSw(ePwrState_type ePwrState);
void Menu_run(eNavEvent_type navEvent);

#endif //MicroMenu_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
