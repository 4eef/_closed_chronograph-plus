/*!****************************************************************************
* @file    ssdGraph.c
* @author  4eef
* @version V1.0
* @date    28.08.2016
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "ssdGraph.h"

/*!****************************************************************************
* MEMORY
*/
const uint8_t menuScroll[4] = {
    0x7E, 0x81, 0x81, 0x7E
};

const uint8_t menuFolder[13] = {
    0xFF, 0xC1, 0xA1, 0x99, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8E, 0x48, 0x28, 0x18
};

const uint8_t ssdBatt[15] = {
    0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF, 0x3C,
};

const uint8_t chrgSym[6] = {
    0x1C, 0x27, 0xE4, 0x27, 0x9C, 0x81,
};

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
