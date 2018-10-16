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
#include "power.h"
#include "buttons.h"

/*!****************************************************************************
* User define
*/
#define MENU_POSITIONS          5
#define MENU_INTERVAL           10
#define MENU_START              12
#define MENU_STR_LEN_MAX        19
#define MENU_MSG_LEN_MAX        240
#define MENU_ITEMS_QTY_MAX      20
#define TXT_PAR_MIN_VAL         0
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
    eChooseFrmLstWnd,
    eParEditWnd,
    eTxtEditWnd,
    eInfoWnd
}eMenuMode_type;

typedef enum{
    eNoFract = 1,
    eTenths = 10,
    eHundreds = 100
}eParFract_type;

typedef enum{
    eItem = 0,
    eChooseFrmLst,
    eParEdit,
    eTxtEdit,
    eParTxtEdit,
    eInfoTxt,
    eFunc
}eMenuItem_type;

typedef struct{
    bool            show;
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
    uint16_t        currTxtPar;
    uint16_t        *pTxtParNumOrigin;
    uint16_t        qtyTxtPar;
}txtParSelWnd_type;

typedef struct{
    char            title[MENU_STR_LEN_MAX];
    char            string[MENU_STR_LEN_MAX];
    char            *pStrOrig;
    uint8_t         symPos;
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

typedef const struct menuPrmtr{
    void            *pPar1;
    void            *pPar2;
    void            *pPar3;
    uint16_t        constPar1;
    uint16_t        constPar2;
    uint16_t        constPar3;
    void (*pFunc)(void);
}menuPrmtr_type;

typedef const struct menuItem{
    const struct menuItem   *next;      // Pointer to the next menu item of this menu item
    const struct menuItem   *previous;  // Pointer to the previous menu item of this menu item
    const struct menuItem   *parent;    // Pointer to the parent menu item of this menu item
    const struct menuItem   *child;     // Pointer to the child menu item of this menu item
    eMenuItem_type          eItemType;  // Menu item type
    const struct menuPrmtr  *parDscr; // Parameter description
    char                    *text;      // Menu item text to pass to the menu display callback function
}menuItem_type;

/*!****************************************************************************
* Extern viriables
*/
/** Null menu entry, used in \ref MENU_ITEM() definitions where no menu link is to be made. */
extern menuItem_type const NULL_MENU;
extern menuPrmtr_type const NULL_PRM;

/*!****************************************************************************
* Macro functions
*/
// Creates a new menu item entry with the specified links and callbacks
#define MENU_ITEM(name, next, previous, parent, child, itemType, parDscr, text) \
    extern menuItem_type const next;      \
    extern menuItem_type const previous;  \
    extern menuItem_type const parent;    \
    extern menuItem_type const child;     \
    extern menuPrmtr_type const parDscr;     \
    menuItem_type const name = {&next, &previous, &parent, &child, itemType, &parDscr, text}
    
#define MENU_PAR(name, pPar1, pPar2, pPar3, constPar1, constPar2, constPar3, pFunc) \
    menuPrmtr_type const name = {pPar1, pPar2, pPar3, constPar1, constPar2, constPar3, pFunc}
    
#define MENU_PARENT         Menu_GetCurrentMenu()->parent
#define MENU_CHILD          Menu_GetCurrentMenu()->child
#define MENU_NEXT           Menu_GetCurrentMenu()->next
#define MENU_PREVIOUS       Menu_GetCurrentMenu()->previous
#define ITEM_TYPE           Menu_GetCurrentMenu()->eItemType
#define PAR_DSCR            Menu_GetCurrentMenu()->parDscr

/*!****************************************************************************
* Prototypes for the functions
*/
menuItem_type* Menu_GetCurrentMenu(void);
void Menu_Navigate(menuItem_type* const NewMenu);
void Menu_listParse(menuItem_type* const NewMenu);
void Menu_putMessage(char *newStr, uint8_t newCnt);
void Menu_parWndRun(eNavEvent_type navEvent);
void Menu_putParWnd(char *parUnits, uint16_t *pParOrigin, uint16_t *pParCopy,
                    eParFract_type parFract, int16_t brdMax, int16_t brdMin);
void Menu_infoWndRun(eNavEvent_type navEvent);
void Menu_txtEditWndRun(eNavEvent_type navEvent);
void Menu_putTxtEditWnd(char *pStrOrig);
void Menu_txtParSelWndRun(eNavEvent_type navEvent);
void Menu_putTxtParSelWnd(char *pFirstPar, uint16_t *pTxtParNumOrigin, uint16_t qtyTxtPar);
void Menu_navMenu(eNavEvent_type navEvent);
void Menu_navDisp(eNavEvent_type navEvent);
void Menu_navPwrOff(eNavEvent_type navEvent);
void Menu_pwrSw(ePwrState_type ePwrState);
void Menu_run(eNavEvent_type navEvent);
void Menu_setParEdit(eNavEvent_type navEvent);

#endif //MicroMenu_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
