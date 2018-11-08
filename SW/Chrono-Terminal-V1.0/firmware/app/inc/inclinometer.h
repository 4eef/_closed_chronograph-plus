/*!****************************************************************************
* @file    inclinometer.h
* @author  4eef
* @version V1.0
* @date    07.11.2018, 4eef
* @brief   --
*/
#ifndef inclinometer_H
#define inclinometer_H

/*!****************************************************************************
* Include
*/
#include <stdint.h>

/*!****************************************************************************
* User define
*/
#define INC_BORDER_MAX          90
#define INC_BORDER_MIN          1

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    int16_t         accRoll;
    int16_t         accPitch;
    uint16_t        accRollBorder;
    uint16_t        accPitchBorder;
}meas_type;

/*!****************************************************************************
* Extern viriables
*/

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void offsCalFunc(void);
void gainCalFunc(void);

#endif //inclinometer_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
