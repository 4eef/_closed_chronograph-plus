/*!****************************************************************************
* @file    chrono.h
* @author  4eef
* @version V1.0
* @date    07.11.2018, 4eef
* @brief   --
*/
#ifndef chrono_H
#define chrono_H

/*!****************************************************************************
* Include
*/
#include <stdint.h>
#include "IR.h"
#include "power.h"

/*!****************************************************************************
* User define
*/
#define PELLET_DB_QTY           15
#define PELLET_KNOWN_LIST       PELLET_DB_QTY - 1
#define PELLET_SGN_TOLERANCE    10
#define PELLET_CHANGE_THR       5
#define PELLET_NEW_SGN_THR      10
#define PELLET_NEW_SGN_BOUND    PELLET_NEW_SGN_THR*1.5
#define PELLET_OK               0
#define PELLET_CONFIRM          1
#define PELLET_NEW              2
#define PELLET_ERR_NEW          3
#define PELLET_MAX              99
#define PELLET_MIN              1
#define PELLET_WGH_MG10_MIN     10
#define PELLET_WGH_MG10_MAX     150
#define STAT_SHOTS_MAX          255
#define STAT_ENERGY_DIV_COEFF   10000

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    char            name[MENU_STR_LEN_MAX];
    uint16_t        wght;
    uint16_t        sgn;
}pellet_type;

typedef struct{
    uint16_t        newSgn;
    uint32_t        newSgnSum;
    uint8_t         newSgnCnt;
    uint8_t         newSgnErrCnt;
    uint8_t         pelStat;
}rxPelSgn_type;

typedef struct{
    pellet_type     pel[PELLET_DB_QTY];
    rxPelSgn_type   rxPelSgn;
    uint16_t        matchedSgnNum;
}pellets_type;

typedef struct{
    uint16_t        pellet;
    uint16_t        clipCurrent;
    uint16_t        clipCapacity;
    uint16_t        speed0;
    uint16_t        speed1;
    uint16_t        speed2;
    uint16_t        speed3;
    uint16_t        speed4;
    uint16_t        pelSgntr;
    uint32_t        chrSgntr;
    uint16_t        chrBindCnt;
    uint16_t        sensDist;
    uint16_t        statSpeeds[STAT_SHOTS_MAX];
    uint32_t        statSpeedsSum;
    uint32_t        statDevsSum;
    uint16_t        statShots;
    uint16_t        statEnergy;
    uint16_t        statMean;
    uint16_t        statSdev;
}chrono_type;

/*!****************************************************************************
* Extern viriables
*/
extern chrono_type          chrono;
extern pellets_type         pellets;

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void hndlIRData(void);

#endif //chrono_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
