/*!****************************************************************************
* @file    ug2864.c
* @author  4eef
* @version V1.0
* @date    14.08.2016
* @brief   --
*/

/*!****************************************************************************
* Include
*/
#include "ug2864.h"

/*!****************************************************************************
* MEMORY
*/
extern menu_type    menu;
ssdVideoBff_type    ssdVideoBff;
ssdSettings_type    ssdSettings;

/*!****************************************************************************
* @brief    Put box for text at the center of display
* @param    
* @retval   
*/
void ssd_putParBox(char *text, uint8_t enArrows){
    uint8_t i, j, x, y;
    char tmp[20], uArrow[2]={24, 0}, dArrow[2]={25, 0};
    //Parameters
    x = (SSD1306_LCDWIDTH - PAR_BOX_WIDTH)/2;
    y = (SSD1306_LCDHEIGHT - PAR_BOX_HEIGHT)/2;
    //Put box
    for(i = 0; i <= PAR_BOX_WIDTH; i++){
        for(j = 0; j <= PAR_BOX_HEIGHT; j++){
            if(j == 0 || j == PAR_BOX_HEIGHT || i == 0 || i == PAR_BOX_WIDTH){
                ssd_setpix(x+i, y+j, WHITE);
            }else{
                ssd_setpix(x+i, y+j, BLACK);
            }
        }
    }
    //Put text
    ssd_putString6x8(10, 10, text);
    if(enArrows == PAR_BOX_ARROWS_EN){
        ssd_putString6x8(46, 46, &uArrow[0]);
        ssd_putString6x8(76, 46, &dArrow[0]);
    }
    strcpy(tmp, "Back");
    ssd_putString6x8(10, 46, &tmp[0]);
    strcpy(tmp, "Save");
    ssd_putString6x8(94, 46, &tmp[0]);
}

/*!****************************************************************************
* @brief    Put box for text at the center of display
* @param    
* @retval   
*/
void ssd_putMsgBox(uint8_t len){
    uint8_t i, j, x, y, boxLength;
    //Parameters
    boxLength = len*6 + 8;
    x = SSD1306_LCDWIDTH/2 - boxLength/2;
    y = SSD1306_LCDHEIGHT/2 - MSG_BOX_HEIGHT/2 - 1;
    //Put box
    for(i = 0; i <= boxLength; i++){
        for(j = 0; j <= MSG_BOX_HEIGHT; j++){
            if(j == 0 || j == MSG_BOX_HEIGHT || i == 0 || i == boxLength){
                ssd_setpix(x+i, y+j, WHITE);
            }else{
                ssd_setpix(x+i, y+j, BLACK);
            }
        }
    }
}

/*!****************************************************************************
* @brief    Put roll progress bar and fill it
* @param    
* @retval   
*/
void ssd_putRollBar(float aabs, uint16_t border, uint8_t y, uint8_t hgt){
    uint8_t i, j, k, l, m, cStrt, cEnd, fYBias, fHgt;
    int16_t brel, angle, end, iaabs, iborder, izero;
    //Parameters calculation
    cStrt = 1;
    cEnd = hgt - cStrt - 1;
    fYBias = y + cStrt + 1;
    fHgt = cEnd - cStrt - 1;
    //Draw the pattern
    for(i = 0; i < SSD1306_LCDWIDTH; i++){
        k = i;
        //Pasting the one in the beginning
        if(i >= SSD1306_LCDWIDTH/2){
            k = i - SSD1306_LCDWIDTH/2;
        }
        //How frequent lines are draw
        if((k%ROLL_PERIOD == 0) || (k == 0)){
            for(j = 0; j < hgt; j++){
                l = (hgt/3)-1;
                m = ((hgt*2)/3)+1;
                //Draw pattern with center part highlighted
                if(i == (SSD1306_LCDWIDTH/2-1) || (i == (SSD1306_LCDWIDTH/2))){
                    l = cStrt;
                    m = cEnd;
                }
                //Draw pattern
                if((j == 0) || (j > l) && (j < m) || (j == hgt-1)){
                    ssd_setpix(i, y+j, WHITE);
                }
            }
        }
    }
    k = SSD1306_LCDWIDTH;                           //Progress bar center setting
    //Convert floats to sints
    iaabs = (int16_t)(aabs*PRECISION);
    iborder = (int16_t)(border*PRECISION);
    izero = 0;
    //Bias
    if(iaabs < izero) k -= 2;                       //Horizontal
    //Calculations
    brel = iborder - izero;                         //Calculation of normalized border value
    if(brel == 0) return;                           //If there is no gap between border and zero
    if(aabs >= izero){                              //Unsigned value of an angle
        angle = iaabs - izero;
    }else{
        angle = izero - iaabs;
    }
    if(angle > brel) angle = brel;                  //If border is leaped over
    end = (angle*SSD1306_LCDWIDTH/2)/brel;          //End of the line
    //Fill the line
    for(i = 0; i < end; i++){
        for(j = 0; j < fHgt; j++){
            if(iaabs >= izero){
                ssd_setpix(k/2 + i, j + fYBias, WHITE);
            }else{
                ssd_setpix(k/2 - i, j + fYBias, WHITE);
            }
        }
    }
}

/*!****************************************************************************
* @brief    Calculate menu's progress bar position and put it
* @param    
* @retval   
*/
void ssd_putMenuScroll(void){
    uint8_t i, j, length, x, y, off;
    length = SCROLL_LINE_LENGTH;
    //Put the line
    x = SCROLL_LINE_X_OFF;
    y = SCROLL_Y_OFF;
    for(i = 0; i < length; i++){
        for(j = 0; j < SCROLL_LINE_WIDTH; j++){
            ssd_setpix((x + j), (y + i), WHITE);
        }
    }
    //Put scroll onto line
    x = SCROLL_X_OFF;
    off = (menu.currItem-1)*length/menu.totItems;   //Calculate scroll offset
    y = SCROLL_Y_OFF + off;
    for(i=0; i<sizeof(menuScroll); i++){
        for (j=0; j<8; j++){
            if(menuScroll[i] & (1 << j)){
                ssd_setpix(x+i, y+j, WHITE);
            }else{
                ssd_setpix(x+i, y+j, BLACK);
            }
        }
    }
}

/*!****************************************************************************
* @brief    Put pitch progress bar and fill it
* @param    
* @retval   
*/
void ssd_putPitchBar(float aabs, uint16_t border){
    uint8_t i, j, k;
    int16_t brel, angle, end, iaabs, iborder, izero;
    //Parameters calculation
    k = SSD1306_LCDHEIGHT;                          //Progress bar center setting
    //Convert floats to sints
    iaabs = (int16_t)(aabs*PRECISION);
    iborder = (int16_t)(border*PRECISION);
    izero = 0;
    //Biases
    if(iaabs > izero) k -= 2;                       //Vertical
    //Calculations
    brel = iborder - izero;                         //Calculation of normalized border value
    if(brel == 0) return;                           //If there is no gap between border and zero
    if(aabs >= izero){                              //Unsigned value of an angle
        angle = iaabs - izero;
    }else{
        angle = izero - iaabs;
    }
    if(angle > brel) angle = brel;                  //If border is leaped over
    end = (angle*PITCH_HEIGHT/2)/brel;             //End of the line
    //Fill the line
    for(i = 0; i < end; i++){
        for(j = 0; j < 4; j++){
            if(iaabs >= izero){
                ssd_setpix((PITCH_X + j), (k/2 - i), WHITE);
            }else{
                ssd_setpix((PITCH_X + j), (k/2 + i), WHITE);
            }
        }
    }
    //Draw the center
    for(i = 0; i < 2; i++){
        for(j = 0; j < 4; j++){
            ssd_setpix((PITCH_X + j), (SSD1306_LCDHEIGHT/2 - i), WHITE);
        }
    }
}

/*!****************************************************************************
* @brief    Put menu folder pattern on top corner of the display area
* @param    
* @retval   
*/
void ssd_putMenuFolder(void){
    uint8_t i, j, x = 0, y = 0;
    //Draw the folder
    for(i=0; i<sizeof(menuFolder); i++){
        for (j=0; j<8; j++){
            if(menuFolder[i] & (1 << j)){
                ssd_setpix(x+i, y+j, WHITE);
            }else{
                ssd_setpix(x+i, y+j, BLACK);
            }
        }
    }
}

/*!****************************************************************************
* @brief    Put battery into x, y position with percentage
* @param    
* @retval   
*/
void ssd_putBatt(uint8_t percentage){
    uint8_t i, j, x, y, fillBiasX, fillBiasY;
    x = BATT_X;
    y = BATT_Y;
    fillBiasX = x + 2;
    fillBiasY = y + 2;
    //Protection
    if(percentage > 100) percentage = 100;
    //Draw empty battery
    for(i=0; i<15; i++){
        for (j=0; j<8; j++){
            if(ssdBatt[i] & (1 << j)){
                ssd_setpix(x+i, y+j, WHITE);
            }else{
                ssd_setpix(x+i, y+j, BLACK);
            }
        }
    }
    //Fill the battery with according percentage
    for(i = 0; i < percentage/10; i++){
        for(j = 0; j < BATT_PIXELS; j++){
            ssd_setpix((fillBiasX + i), (fillBiasY + j), WHITE);
        }
    }
}

/*!****************************************************************************
* @brief    Put char of 12x16 font into x, y position
* @param    
* @retval   
*/
void ssd_putChar12x16(uint8_t x, uint8_t y, char c){
    uint8_t i, j;
    //Draw the char
    for(i=0; i<24; i++){
        for (j=0; j<8; j++){
            if(i%2 == 0){
                if(font12x16[c-46][i] & (1 << j)){
                    ssd_setpix(x+i/2, y+j, WHITE);
                }else{
                    ssd_setpix(x+i/2, y+j, BLACK);
                }
            }else{
                if(font12x16[c-46][i] & (1 << j)){
                    ssd_setpix(x+i/2, (y-8)+j, WHITE);
                }else{
                    ssd_setpix(x+i/2, (y-8)+j, BLACK);
                }
            }
        }
    }
}

/*!****************************************************************************
* @brief    Put string of 12x16 font chars into x, y position
* @param    
* @retval   
*/
void ssd_putString12x16(uint8_t x, uint8_t y, const char *s){
    while (*s){
        ssd_putChar12x16(x, y, *s);
        s++;
        x += 12;
    }
}

/*!****************************************************************************
* @brief    Put char of 6x8 font into x, y position
* @param    
* @retval   
*/
void ssd_putChar6x8(uint8_t x, uint8_t y, char c){
    uint8_t i, j;     
    //Draw the char
    for(i=0; i<6; i++){
        for (j=0; j<8; j++){
            if(font6x8[c][i] & (1 << j)){
                ssd_setpix(x+i, y+j, WHITE);
            }else{
                ssd_setpix(x+i, y+j, BLACK);
            }
        }
    }
} 

/*!****************************************************************************
* @brief    Put string of 6x8 font chars into x, y position
* @param    
* @retval   
*/
void ssd_putString6x8(uint8_t x, uint8_t y, const char *s){
    while (*s){
        ssd_putChar6x8(x, y, *s);
        s++;
        x += 6;
    }
}

/*!****************************************************************************
* @brief    Set pixel routine
* @param    
* @retval   
*/
void ssd_setpix(uint8_t x, uint8_t y, uint8_t color){   //0 - black, 1 - white, 2 - inverse
    switch (color){
        case WHITE:   ssdVideoBff.video[x+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
        case BLACK:   ssdVideoBff.video[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break; 
        case INVERSE: ssdVideoBff.video[x+ (y/8)*SSD1306_LCDWIDTH] ^=  (1 << (y&7)); break; 
    }
}

/*!****************************************************************************
* @brief    Initialization routine
* @param    
* @retval   
*/
void ug2864init(void){
    gppin_reset(GP_OLED_RES);                   //Perform the controller reset
    delay_us(10000);
    gppin_set(GP_OLED_RES);
    ug2864_com(0xae);                           //Turn off oled panel
    ug2864_com(0x00);                           //Set low column address
    ug2864_com(0x10);                           //Set high column address
    ug2864_com(0x40);                           //Set start line address
    ug2864_com(0x81);                           //Set contrast control register
    ug2864_com(0x80);
    ug2864_com(0xC8);                           //Set COM scan direction for remapped mode
    ug2864_com(0xA1);                           //Set segment remap 127 to 0
    ug2864_com(0x20);                           //Horizontal addressing mode
    ug2864_com(0);
    ug2864_com(0xa6);                           //Set normal display
    ug2864_com(0xa8);                           //Set multiplex ratio(1 to 64)
    ug2864_com(0x3f);                           //1/64 duty
    ug2864_com(0xd3);                           //Set display offset
    ug2864_com(0x00);                           //No offset
    ug2864_com(0xd5);                           //Set display clock divide ratio/oscillator frequency
    ug2864_com(0xf0);                           //Set divide ratio
    ug2864_com(0xd9);                           //Set pre-charge period (was - 0xD9)
    ug2864_com(0xf1);
    ug2864_com(0xda);                           //Set com pins hardware configuration
    ug2864_com(0x12);
    ug2864_com(0xdb);                           //Set VCOMH
    ug2864_com(0x40);
//    ug2864_com(0x8d);                           //Set Charge Pump enable
//    ug2864_com(0x14);                           //0x10 for disable
//    ug2864_com(0xaf);                           //Turn on oled panel
    ssdVideoBff.data = 0x40;
//    ug2864_refresh();
}

/*!****************************************************************************
* @brief    Send command
* @param    
* @retval   
*/
void ug2864_com(uint8_t com){
    uint8_t sadd = 0x78;
    uint8_t data[] = {0, com};
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CTx(sadd | 1, data, 2);
}

/*!****************************************************************************
* @brief    Send data to GDDRAM
* @param    
* @retval   
*/
void ug2864_refresh(void){
    uint8_t sadd = 0x78;
    if(ssdSettings.status != ssdSettings.enable){
        if(ssdSettings.enable == DISPLAY_ENABLE){
            ug2864_com(0x8D);
            ug2864_com(0x14);
            ug2864_com(0xAF);
            ssdSettings.enable = DISPLAY_ENABLE;
        }else{
            ug2864_com(0xAE);
            ug2864_com(0x8D);
            ug2864_com(0x10);
            ssdSettings.enable = DISPLAY_DISABLE;
        }
        ssdSettings.status = ssdSettings.enable;
    }
    if(ssdSettings.status == DISPLAY_ENABLE){
        while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
        I2CTx(sadd | 1, &ssdVideoBff.data, 1025);
    }
}
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
