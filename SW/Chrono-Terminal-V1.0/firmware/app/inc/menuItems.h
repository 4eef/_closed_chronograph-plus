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
extern pellets_type         pellets;
extern meas_type            meas;
extern chron_type           chron;
extern power_type           power;

// Menu parameter items
//       Name           Par1                    Par2                    Par3                    ConstPar1               ConstPar2               ConstPar3               Function
MENU_PAR(scrmode,       sysPars.modeTxts[0],    &sysPars.dispMode,      NULL,                   SCR_MODES_NO,           NULL,                   NULL,                   NULL);
MENU_PAR(sccedit,       "pcs",                  &chron.clipCapacity,    &chron.clipCurrent,     eNoFract,               PELLET_MAX,             PELLET_MIN,             NULL);
MENU_PAR(potedit,       "min",                  &power.uptimeSet,       NULL,                   eNoFract,               POWER_RUN_MAX,          POWER_RUN_MIN,          NULL);
MENU_PAR(sdedit,        "mm",                   &chron.sensDist,        NULL,                   eTenths,                CHR_DIST_MAX,           CHR_DIST_MIN,           NULL);
MENU_PAR(rgbedit,       "deg",                  &meas.accRollBorder,    NULL,                   eNoFract,               INC_BORDER_MAX,         INC_BORDER_MIN,         NULL);
MENU_PAR(pdb1edit,      "g",                    &pellets.pel[0].wght,   &pellets.pel[0].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb2edit,      "g",                    &pellets.pel[1].wght,   &pellets.pel[1].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb3edit,      "g",                    &pellets.pel[2].wght,   &pellets.pel[2].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb4edit,      "g",                    &pellets.pel[3].wght,   &pellets.pel[3].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb5edit,      "g",                    &pellets.pel[4].wght,   &pellets.pel[4].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb6edit,      "g",                    &pellets.pel[5].wght,   &pellets.pel[5].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb7edit,      "g",                    &pellets.pel[6].wght,   &pellets.pel[6].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb8edit,      "g",                    &pellets.pel[7].wght,   &pellets.pel[7].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb9edit,      "g",                    &pellets.pel[8].wght,   &pellets.pel[8].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb10edit,     "g",                    &pellets.pel[9].wght,   &pellets.pel[9].name,   eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb11edit,     "g",                    &pellets.pel[10].wght,  &pellets.pel[10].name,  eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb12edit,     "g",                    &pellets.pel[11].wght,  &pellets.pel[11].name,  eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb13edit,     "g",                    &pellets.pel[12].wght,  &pellets.pel[12].name,  eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb14edit,     "g",                    &pellets.pel[13].wght,  &pellets.pel[13].name,  eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);
MENU_PAR(pdb15edit,     "g",                    &pellets.pel[14].wght,  &pellets.pel[14].name,  eHundreds,              PELLET_WGH_MG10_MAX,    PELLET_WGH_MG10_MIN,    NULL);

// Menu items                           
//        Name          Next            Previous        Parent          Child           ItemType        ParDscr         Text
//Menu
MENU_ITEM(mode,         stts,           NULL_MENU,      NULL_MENU,      NULL_MENU,      eChooseFrmLst,  scrmode,        "Main screen");
MENU_ITEM(stts,         setts,          mode,           NULL_MENU,      NULL_MENU,      eInfoTxt,       NULL_PRM,       "Statistics");
MENU_ITEM(setts,        about,          stts,           NULL_MENU,      sclipc,         eItem,          NULL_PRM,       "Settings");
MENU_ITEM(about,        service,        setts,          NULL_MENU,      NULL_MENU,      eInfoTxt,       NULL_PRM,       "About");
MENU_ITEM(service,      NULL_MENU,      about,          NULL_MENU,      NULL_MENU,      eInfoTxt,       NULL_PRM,       "Service info");
//Menu/Settings
MENU_ITEM(sclipc,       schrono,        NULL_MENU,      setts,          NULL_MENU,      eParEdit,       sccedit,        "Clip capacity");
MENU_ITEM(schrono,      sincline,       sclipc,         setts,          scdist,         eItem,          NULL_PRM,       "Chronograph");
MENU_ITEM(sincline,     speldb,         schrono,        setts,          sibrdr,         eItem,          NULL_PRM,       "Inclinometer");
MENU_ITEM(speldb,       spofft,         sincline,       setts,          spdbp1,         eItem,          NULL_PRM,       "Pellets database");
MENU_ITEM(spofft,       sswrst,         speldb,         setts,          NULL_MENU,      eParEdit,       potedit,        "Power off timer");
MENU_ITEM(sswrst,       NULL_MENU,      spofft,         setts,          NULL_MENU,      eFunc,          NULL_PRM,       "Software reset");
//Menu/Settings/Chronograph
MENU_ITEM(scdist,       scbind,         NULL_MENU,      schrono,        NULL_MENU,      eParEdit,       sdedit,         "Sensor distance");
MENU_ITEM(scbind,       NULL_MENU,      scdist,         schrono,        NULL_MENU,      eFunc,          NULL_PRM,       "Bind");
//Menu/Settings/Inclinometer
MENU_ITEM(sibrdr,       siofcal,        NULL_MENU,      sincline,       NULL_MENU,      eParEdit,       rgbedit,        "Roll graph border");
MENU_ITEM(siofcal,      sigacal,        sibrdr,         sincline,       NULL_MENU,      eFunc,          NULL_PRM,       "Offset calibration");
MENU_ITEM(sigacal,      NULL_MENU,      siofcal,        sincline,       NULL_MENU,      eFunc,          NULL_PRM,       "Gain calibration");
//Menu/Settings/Pellets database
MENU_ITEM(spdbp1,       spdbp2,         NULL_MENU,      speldb,         NULL_MENU,      eParTxtEdit,    pdb1edit,       pellets.pel[0].name);
MENU_ITEM(spdbp2,       spdbp3,         spdbp1,         speldb,         NULL_MENU,      eParTxtEdit,    pdb2edit,       pellets.pel[1].name);
MENU_ITEM(spdbp3,       spdbp4,         spdbp2,         speldb,         NULL_MENU,      eParTxtEdit,    pdb3edit,       pellets.pel[2].name);
MENU_ITEM(spdbp4,       spdbp5,         spdbp3,         speldb,         NULL_MENU,      eParTxtEdit,    pdb4edit,       pellets.pel[3].name);
MENU_ITEM(spdbp5,       spdbp6,         spdbp4,         speldb,         NULL_MENU,      eParTxtEdit,    pdb5edit,       pellets.pel[4].name);
MENU_ITEM(spdbp6,       spdbp7,         spdbp5,         speldb,         NULL_MENU,      eParTxtEdit,    pdb6edit,       pellets.pel[5].name);
MENU_ITEM(spdbp7,       spdbp8,         spdbp6,         speldb,         NULL_MENU,      eParTxtEdit,    pdb7edit,       pellets.pel[6].name);
MENU_ITEM(spdbp8,       spdbp9,         spdbp7,         speldb,         NULL_MENU,      eParTxtEdit,    pdb8edit,       pellets.pel[7].name);
MENU_ITEM(spdbp9,       spdbp10,        spdbp8,         speldb,         NULL_MENU,      eParTxtEdit,    pdb9edit,       pellets.pel[8].name);
MENU_ITEM(spdbp10,      spdbp11,        spdbp9,         speldb,         NULL_MENU,      eParTxtEdit,    pdb10edit,      pellets.pel[9].name);
MENU_ITEM(spdbp11,      spdbp12,        spdbp10,        speldb,         NULL_MENU,      eParTxtEdit,    pdb11edit,      pellets.pel[10].name);
MENU_ITEM(spdbp12,      spdbp13,        spdbp11,        speldb,         NULL_MENU,      eParTxtEdit,    pdb12edit,      pellets.pel[11].name);
MENU_ITEM(spdbp13,      spdbp14,        spdbp12,        speldb,         NULL_MENU,      eParTxtEdit,    pdb13edit,      pellets.pel[12].name);
MENU_ITEM(spdbp14,      spdbp15,        spdbp13,        speldb,         NULL_MENU,      eParTxtEdit,    pdb14edit,      pellets.pel[13].name);
MENU_ITEM(spdbp15,      NULL_MENU,      spdbp14,        speldb,         NULL_MENU,      eParTxtEdit,    pdb15edit,      pellets.pel[14].name);

#endif //menuItems_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
