/*!****************************************************************************
* @file    menuItems.h
* @author  4eef
* @version V1.0
* @date    20.09.2018, 4eef
* @brief   --
*/
#ifndef menuItems_H
#define menuItems_H

#include "microMenu.h"
#include "menuFunctions.h"
#include "system.h"
#include "chrono.h"
#include "inclinometer.h"

extern sysPars_type         sysPars;
extern pellets_type         pellets;
extern meas_type            meas;
extern chrono_type          chrono;
extern power_type           power;

// Menu parameter items
//       Name           Par1                    Par2                    Par3                    ConstPar1               ConstPar2               ConstPar3               Function
MENU_PAR(pstats,        (void *)&statsTxt,      NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   statsParser);
MENU_PAR(pabout,        NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   aboutParser);
MENU_PAR(psinfo,        NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   aboutParser);
MENU_PAR(pmode,         sysPars.modeTxts[0],    &sysPars.dispMode,      NULL,                   SCR_MODES_NO,           NULL,                   NULL,                   NULL);
MENU_PAR(ppot,          "min",                  &power.uptimeSet,       NULL,                   eNoFract,               POWER_RUN_MAX,          POWER_RUN_MIN,          NULL);
MENU_PAR(pswres,        NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL);
MENU_PAR(pcc,           "pcs",                  &chrono.clipCapacity,   &chrono.clipCurrent,    eNoFract,               PELLET_MAX,             PELLET_MIN,             NULL);
MENU_PAR(psd,           "mm",                   &chrono.sensDist,       NULL,                   eTenths,                CHR_DIST_MAX,           CHR_DIST_MIN,           NULL);
MENU_PAR(pchrbnd,       NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL);
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
MENU_PAR(prgb,          "deg",                  &meas.accRollBorder,    NULL,                   eNoFract,               INC_BORDER_MAX,         INC_BORDER_MIN,         NULL);
MENU_PAR(pioc,          NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL);
MENU_PAR(pigc,          NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL,                   NULL);

// Menu items                           
//        Name          Next            Previous        Parent          Child           ItemType        ParDscr         Text
//Menu
MENU_ITEM(rstats,       rsetts,         NULL_MENU,      NULL_MENU,      NULL_MENU,      eInfoTxt,       pstats,         "Statistics");
MENU_ITEM(rsetts,       rabout,         rstats,         NULL_MENU,      smode,          eItem,          NULL_PRM,       "Settings");
MENU_ITEM(rabout,       rservice,       rsetts,         NULL_MENU,      NULL_MENU,      eInfoTxt,       pabout,         "About");
MENU_ITEM(rservice,     NULL_MENU,      rabout,         NULL_MENU,      NULL_MENU,      eInfoTxt,       psinfo,         "Service info");
//Menu/Settings
MENU_ITEM(smode,        schrono,        NULL_MENU,      rsetts,         NULL_MENU,      eChooseFrmLst,  pmode,          "Main screen");
MENU_ITEM(schrono,      sincline,       smode,          rsetts,         scclipc,        eItem,          NULL_PRM,       "Chronograph");
MENU_ITEM(sincline,     spofft,         schrono,        rsetts,         sibrdr,         eItem,          NULL_PRM,       "Inclinometer");
MENU_ITEM(spofft,       swrst,          sincline,       rsetts,         NULL_MENU,      eParEdit,       ppot,           "Power off timer");
MENU_ITEM(swrst,        NULL_MENU,      spofft,         rsetts,         NULL_MENU,      eFunc,          pswres,         "Software reset");
//Menu/Settings/Chronograph
MENU_ITEM(scclipc,      scpeldb,        NULL_MENU,      schrono,        NULL_MENU,      eParEdit,       pcc,            "Clip capacity");
MENU_ITEM(scpeldb,      scdist,         scclipc,        schrono,        spdbp1,         eItem,          NULL_PRM,       "Pellets database");
MENU_ITEM(scdist,       scbind,         scpeldb,        schrono,        NULL_MENU,      eParEdit,       psd,            "Sensor distance");
MENU_ITEM(scbind,       NULL_MENU,      scdist,         schrono,        NULL_MENU,      eFunc,          pchrbnd,        "Bind chronograph");
//Menu/Settings/Chronograph/Pellets database
MENU_ITEM(spdbp1,       spdbp2,         NULL_MENU,      scpeldb,        NULL_MENU,      eParTxtEdit,    pdb1edit,       pellets.pel[0].name);
MENU_ITEM(spdbp2,       spdbp3,         spdbp1,         scpeldb,        NULL_MENU,      eParTxtEdit,    pdb2edit,       pellets.pel[1].name);
MENU_ITEM(spdbp3,       spdbp4,         spdbp2,         scpeldb,        NULL_MENU,      eParTxtEdit,    pdb3edit,       pellets.pel[2].name);
MENU_ITEM(spdbp4,       spdbp5,         spdbp3,         scpeldb,        NULL_MENU,      eParTxtEdit,    pdb4edit,       pellets.pel[3].name);
MENU_ITEM(spdbp5,       spdbp6,         spdbp4,         scpeldb,        NULL_MENU,      eParTxtEdit,    pdb5edit,       pellets.pel[4].name);
MENU_ITEM(spdbp6,       spdbp7,         spdbp5,         scpeldb,        NULL_MENU,      eParTxtEdit,    pdb6edit,       pellets.pel[5].name);
MENU_ITEM(spdbp7,       spdbp8,         spdbp6,         scpeldb,        NULL_MENU,      eParTxtEdit,    pdb7edit,       pellets.pel[6].name);
MENU_ITEM(spdbp8,       spdbp9,         spdbp7,         scpeldb,        NULL_MENU,      eParTxtEdit,    pdb8edit,       pellets.pel[7].name);
MENU_ITEM(spdbp9,       spdbp10,        spdbp8,         scpeldb,        NULL_MENU,      eParTxtEdit,    pdb9edit,       pellets.pel[8].name);
MENU_ITEM(spdbp10,      spdbp11,        spdbp9,         scpeldb,        NULL_MENU,      eParTxtEdit,    pdb10edit,      pellets.pel[9].name);
MENU_ITEM(spdbp11,      spdbp12,        spdbp10,        scpeldb,        NULL_MENU,      eParTxtEdit,    pdb11edit,      pellets.pel[10].name);
MENU_ITEM(spdbp12,      spdbp13,        spdbp11,        scpeldb,        NULL_MENU,      eParTxtEdit,    pdb12edit,      pellets.pel[11].name);
MENU_ITEM(spdbp13,      spdbp14,        spdbp12,        scpeldb,        NULL_MENU,      eParTxtEdit,    pdb13edit,      pellets.pel[12].name);
MENU_ITEM(spdbp14,      spdbp15,        spdbp13,        scpeldb,        NULL_MENU,      eParTxtEdit,    pdb14edit,      pellets.pel[13].name);
MENU_ITEM(spdbp15,      NULL_MENU,      spdbp14,        scpeldb,        NULL_MENU,      eParTxtEdit,    pdb15edit,      pellets.pel[14].name);
//Menu/Settings/Inclinometer
MENU_ITEM(sibrdr,       siofcal,        NULL_MENU,      sincline,       NULL_MENU,      eParEdit,       prgb,           "Roll graph border");
MENU_ITEM(siofcal,      sigacal,        sibrdr,         sincline,       NULL_MENU,      eFunc,          pioc,           "Offset calibration");
MENU_ITEM(sigacal,      NULL_MENU,      siofcal,        sincline,       NULL_MENU,      eFunc,          pigc,           "Gain calibration");

#endif //menuItems_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
