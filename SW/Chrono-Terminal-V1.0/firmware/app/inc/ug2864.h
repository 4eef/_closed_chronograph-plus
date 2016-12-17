/*!****************************************************************************
* @file    ug2864.h
* @author  4eef
* @version V1.0
* @date    14.08.2016
* @brief   --
*/
#ifndef ug2864_H
#define ug2864_H

/*!****************************************************************************
* Include
*/
#include "stm32f0xx.h"
#include "i2c.h"
#include "gpio.h"
#include "delay.h"
#include "ssdFonts.h"
#include "ssdGraph.h"
#include "MicroMenu.h"

/*!****************************************************************************
* User define
*/
#define BLACK                           0
#define WHITE                           1
#define INVERSE                         2
#define SSD1306_LCDWIDTH                128
#define SSD1306_LCDHEIGHT               64
#define PRECISION                       100
#define SCROLL_X_OFF                    124
#define SCROLL_LINE_X_OFF               125
#define SCROLL_Y_OFF                    10
#define SCROLL_LINE_WIDTH               2
#define SCROLL_LINE_LENGTH              52
#define ROLL_PERIOD                     7
#define ROLL_HIGH_HEIGHT                46
#define ROLL_HIGH_Y                     18
#define ROLL_LOW_HEIGHT                 8
#define ROLL_LOW_Y                      56
#define PITCH_X                         124
#define PITCH_HEIGHT                    44
#define BATT_PIXELS                     4
#define BATT_X                          113
#define BATT_Y                          0

/*!****************************************************************************
* User enum
*/

/*!****************************************************************************
* User typedef
*/
typedef struct{
    uint8_t     data;
    uint8_t     video[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8];
}ssdVideoBff_type;

/*!****************************************************************************
* Extern viriables
*/
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

/*!****************************************************************************
* Macro functions
*/

/*!****************************************************************************
* Prototypes for the functions
*/
void ug2864init(void);
void ug2864_com(uint8_t com);
void ug2864_refresh(void);
void ssd_putChar12x16(uint8_t x, uint8_t y, char c);
void ssd_putString12x16(uint8_t x, uint8_t y, const char *s);
void ssd_putChar6x8(uint8_t x, uint8_t y, char c);
void ssd_putString6x8(uint8_t x, uint8_t y, const char *s);
void ssd_setpix(uint8_t x, uint8_t y, uint8_t color);
void ssd_putBatt(uint8_t percentage);
void ssd_putPitchBar(float aabs, float border);
void ssd_putMenuFolder(void);
void ssd_putMenuScroll(void);
void ssd_putRollBar(float aabs, float border, uint8_t y, uint8_t hgt);

#endif //ug9616_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
