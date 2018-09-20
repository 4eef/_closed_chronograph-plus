/*!****************************************************************************
* @file    menuItems.h
* @author  4eef
* @version V1.0
* @date    20.09.2018, 4eef
* @brief   --
*/
#ifndef menuItems_H
#define menuItems_H

#include "MicroMenu.h"

extern sysPars_type         sysPars;

// Menu parameter items
//       Name           pPar1                   pPar2                   pPar3                   ConstPar1               ConstPar2               ConstPar3
MENU_PAR(scrmode,       sysPars.modeTxts[0],    &sysPars.dispMode,      NULL,                   SCR_MODES_NO,           LIST_STR_LEN,           NULL);


// Menu items                           
//        Name          Next            Previous        Parent          Child           ItemType        PrmtrDscr       Text
//Menu
MENU_ITEM(mode,         stats,          NULL_MENU,      NULL_MENU,      NULL_MENU,      eChooseFrmLst,  scrmode,        "Main screen");
MENU_ITEM(stats,        setts,          mode,           NULL_MENU,      NULL_MENU,      eItem,          NULL_PRM,       "Statistics");
MENU_ITEM(setts,        about,          stats,          NULL_MENU,      NULL_MENU,         eItem,          NULL_PRM,       "Settings");//sclipc
MENU_ITEM(about,        service,        setts,          NULL_MENU,      NULL_MENU,      eItem,          NULL_PRM,       "About");
MENU_ITEM(service,      NULL_MENU,      about,          NULL_MENU,      NULL_MENU,      eItem,          NULL_PRM,       "Service info");
////Menu/Settings
//MENU_ITEM(sclipc,       schrono,        NULL_MENU,      setts,          NULL_MENU,      NULL,           parEditInit,    "Clip capacity");
//MENU_ITEM(schrono,      sincline,       sclipc,         setts,          scdist,         NULL,           NULL,           "Chronograph");
//MENU_ITEM(sincline,     speldb,         schrono,        setts,          sibrdr,         NULL,           NULL,           "Inclinometer");
//MENU_ITEM(speldb,       spofft,         sincline,       setts,          spdbp1,         NULL,           NULL,           "Pellets database");
//MENU_ITEM(spofft,       sswrst,         speldb,         setts,          NULL_MENU,      NULL,           parEditInit,    "Power off timer");
//MENU_ITEM(sswrst,       NULL_MENU,      spofft,         setts,          NULL_MENU,      NULL,           parEditInit,    "Software reset");
////Menu/Settings/Chronograph
//MENU_ITEM(scdist,       scbind,         NULL_MENU,      schrono,        NULL_MENU,      NULL,           parEditInit,    "Sensor distance");
//MENU_ITEM(scbind,       NULL_MENU,      scdist,         schrono,        NULL_MENU,      NULL,           parEditInit,    "Bind");
////Menu/Settings/Inclinometer
//MENU_ITEM(sibrdr,       siofcal,        NULL_MENU,      sincline,       NULL_MENU,      NULL,           parEditInit,    "Roll graph border");
//MENU_ITEM(siofcal,      sigacal,        sibrdr,         sincline,       NULL_MENU,      NULL,           parEditInit,    "Offset calibration");
//MENU_ITEM(sigacal,      NULL_MENU,      siofcal,        sincline,       NULL_MENU,      NULL,           parEditInit,    "Gain calibration");
////Menu/Settings/Pellets database
//MENU_ITEM(spdbp1,       spdbp2,         NULL_MENU,      speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[1]);
//MENU_ITEM(spdbp2,       spdbp3,         spdbp1,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[2]);
//MENU_ITEM(spdbp3,       spdbp4,         spdbp2,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[3]);
//MENU_ITEM(spdbp4,       spdbp5,         spdbp3,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[4]);
//MENU_ITEM(spdbp5,       spdbp6,         spdbp4,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[5]);
//MENU_ITEM(spdbp6,       spdbp7,         spdbp5,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[6]);
//MENU_ITEM(spdbp7,       spdbp8,         spdbp6,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[7]);
//MENU_ITEM(spdbp8,       spdbp9,         spdbp7,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[8]);
//MENU_ITEM(spdbp9,       spdbp10,        spdbp8,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[9]);
//MENU_ITEM(spdbp10,      spdbp11,        spdbp9,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[10]);
//MENU_ITEM(spdbp11,      spdbp12,        spdbp10,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[11]);
//MENU_ITEM(spdbp12,      spdbp13,        spdbp11,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[12]);
//MENU_ITEM(spdbp13,      spdbp14,        spdbp12,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[13]);
//MENU_ITEM(spdbp14,      spdbp15,        spdbp13,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[14]);
//MENU_ITEM(spdbp15,      NULL_MENU,      spdbp14,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[15]);

//// Menu items
////        Name          Next            Previous        Parent          Child           SelectFunc      EnterFunc       Name
////Menu
//MENU_ITEM(mode,         stats,          NULL_MENU,      NULL_MENU,      mcommon,        NULL,           NULL,           "Main screen");
//MENU_ITEM(stats,        setts,          mode,           NULL_MENU,      NULL_MENU,      NULL,           NULL,           "Statistics");
//MENU_ITEM(setts,        about,          stats,          NULL_MENU,      sclipc,         NULL,           NULL,           "Settings");
//MENU_ITEM(about,        service,        setts,          NULL_MENU,      NULL_MENU,      NULL,           NULL,           "About");
//MENU_ITEM(service,      NULL_MENU,      about,          NULL_MENU,      NULL_MENU,      NULL,           NULL,           "Service info");
////Menu/Display mode
//MENU_ITEM(mcommon,      mchron,         NULL_MENU,      mode,           NULL_MENU,      NULL,           modeEdit,       "Hybrid");
//MENU_ITEM(mchron,       mincline,       mcommon,        mode,           NULL_MENU,      NULL,           modeEdit,       "Chronograph");
//MENU_ITEM(mincline,     NULL_MENU,      mchron,         mode,           NULL_MENU,      NULL,           modeEdit,       "Inclinometer");
////Menu/Settings
//MENU_ITEM(sclipc,       schrono,        NULL_MENU,      setts,          NULL_MENU,      NULL,           parEditInit,    "Clip capacity");
//MENU_ITEM(schrono,      sincline,       sclipc,         setts,          scdist,         NULL,           NULL,           "Chronograph");
//MENU_ITEM(sincline,     speldb,         schrono,        setts,          sibrdr,         NULL,           NULL,           "Inclinometer");
//MENU_ITEM(speldb,       spofft,         sincline,       setts,          spdbp1,         NULL,           NULL,           "Pellets database");
//MENU_ITEM(spofft,       sswrst,         speldb,         setts,          NULL_MENU,      NULL,           parEditInit,    "Power off timer");
//MENU_ITEM(sswrst,       NULL_MENU,      spofft,         setts,          NULL_MENU,      NULL,           parEditInit,    "Software reset");
////Menu/Settings/Chronograph
//MENU_ITEM(scdist,       scbind,         NULL_MENU,      schrono,        NULL_MENU,      NULL,           parEditInit,    "Sensor distance");
//MENU_ITEM(scbind,       NULL_MENU,      scdist,         schrono,        NULL_MENU,      NULL,           parEditInit,    "Bind");
////Menu/Settings/Inclinometer
//MENU_ITEM(sibrdr,       siofcal,        NULL_MENU,      sincline,       NULL_MENU,      NULL,           parEditInit,    "Roll graph border");
//MENU_ITEM(siofcal,      sigacal,        sibrdr,         sincline,       NULL_MENU,      NULL,           parEditInit,    "Offset calibration");
//MENU_ITEM(sigacal,      NULL_MENU,      siofcal,        sincline,       NULL_MENU,      NULL,           parEditInit,    "Gain calibration");
////Menu/Settings/Pellets database
//MENU_ITEM(spdbp1,       spdbp2,         NULL_MENU,      speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[1]);
//MENU_ITEM(spdbp2,       spdbp3,         spdbp1,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[2]);
//MENU_ITEM(spdbp3,       spdbp4,         spdbp2,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[3]);
//MENU_ITEM(spdbp4,       spdbp5,         spdbp3,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[4]);
//MENU_ITEM(spdbp5,       spdbp6,         spdbp4,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[5]);
//MENU_ITEM(spdbp6,       spdbp7,         spdbp5,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[6]);
//MENU_ITEM(spdbp7,       spdbp8,         spdbp6,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[7]);
//MENU_ITEM(spdbp8,       spdbp9,         spdbp7,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[8]);
//MENU_ITEM(spdbp9,       spdbp10,        spdbp8,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[9]);
//MENU_ITEM(spdbp10,      spdbp11,        spdbp9,         speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[10]);
//MENU_ITEM(spdbp11,      spdbp12,        spdbp10,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[11]);
//MENU_ITEM(spdbp12,      spdbp13,        spdbp11,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[12]);
//MENU_ITEM(spdbp13,      spdbp14,        spdbp12,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[13]);
//MENU_ITEM(spdbp14,      spdbp15,        spdbp13,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[14]);
//MENU_ITEM(spdbp15,      NULL_MENU,      spdbp14,        speldb,         NULL_MENU,      NULL,           txtEditInit,    pellets.pelStrings[15]);

#endif //menuItems_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
