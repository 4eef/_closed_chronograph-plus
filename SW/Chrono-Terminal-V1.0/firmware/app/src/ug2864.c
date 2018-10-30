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

/*!****************************************************************************
* @brief    Put text parameter select window at the center of display
*/
void ssd_putInfoTxtWnd(void){
    uint8_t i, j, x, y, pos, offs, numStrs;
    //Draw the folder
    x = 0;
    y = 0;
    for(i=0; i<sizeof(menuFolder); i++){
        for (j=0; j<8; j++){
            if(menuFolder[i] & (1 << j)){
                ssd_setpix(x+i, y+j, WHITE);
            }else{
                ssd_setpix(x+i, y+j, BLACK);
            }
        }
    }
    if((menu.infoWindow.totStrs + 1) > MENU_POSITIONS){
        //Put the line
        x = SCROLL_LINE_X_OFF;
        y = SCROLL_Y_OFF;
        for(i = 0; i < SCROLL_LINE_LENGTH; i++){
            for(j = 0; j < SCROLL_LINE_WIDTH; j++){
                ssd_setpix((x + j), (y + i), WHITE);
            }
        }
        //Put scroll onto line
        x = SCROLL_X_OFF;
        offs = (menu.infoWindow.wndOffs)*(SCROLL_LINE_LENGTH-8)/((menu.infoWindow.totStrs + 1) - MENU_POSITIONS);
        y = SCROLL_Y_OFF + offs;
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
    //Put strings
    ssd_putString6x8(14, 1, &menu.infoWindow.title[0]);
    if((menu.infoWindow.totStrs + 1) <= MENU_POSITIONS){
        numStrs = menu.infoWindow.totStrs + 1;
    }else{
        numStrs = MENU_POSITIONS;
    }
    for(i = 0; i < numStrs; i++){
        ssd_putString6x8(8, MENU_START+MENU_INTERVAL*i, &menu.infoWindow.strings[menu.infoWindow.wndOffs+i][0]);
    }
}

/*!****************************************************************************
* @brief    Put text parameter select window at the center of display
*/
void ssd_putTxtEditWnd(void){
    uint8_t len, offs;
    char uArrow[2]={24, 0}, dArrow[2]={25, 0}, bArrow[2]={27, 0}, fArrow[2]={26, 0}, symPtr[2]={223, 0};
    //Put title
    ssd_putString6x8(0, 0, menu.txtEditWnd.title);
    //Put text parameter
    len = strlen(menu.txtEditWnd.string);
    if(len == 0) len = 1;
    offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
    ssd_putString6x8(offs, 28, menu.txtEditWnd.string);
    ssd_putString6x8((offs + (menu.txtEditWnd.symPos*6)), 36, symPtr);
    //Buttons labels
    ssd_putString6x8(0, 56, bArrow);
    ssd_putString6x8(40, 56, uArrow);
    ssd_putString6x8(82, 56, dArrow);
    ssd_putString6x8(122, 56, fArrow);
}

/*!****************************************************************************
* @brief    Put text parameter select window at the center of display
*/
void ssd_putTxtParSelWnd(void){
    uint8_t len, offs;
    char uArrow[2]={24, 0}, dArrow[2]={25, 0}, tBack[6]={"Back"}, tSave[6]={"Save"};
    //Put title
    ssd_putString6x8(0, 0, menu.txtParSelWnd.title);
    //Put text parameter
    len = strlen(menu.txtParSelWnd.parText);
    offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
    ssd_putString6x8(offs, 28, menu.txtParSelWnd.parText);
    //Buttons labels
    ssd_putString6x8(0, 56, tBack);
    ssd_putString6x8(40, 56, uArrow);
    ssd_putString6x8(82, 56, dArrow);
    ssd_putString6x8(104, 56, tSave);
}

/*!****************************************************************************
* @brief    Put parameter select window at the center of display
*/
void ssd_putParWnd(void){
    uint16_t val1, val2;
    uint8_t len, offs;
    char tPar[20], uArrow[2]={24, 0}, dArrow[2]={25, 0}, tBack[6]={"Back"}, tSave[6]={"Save"};
    //Put title
    ssd_putString6x8(0, 0, menu.parEditWnd.title);
    //Put parameter
    if(menu.parEditWnd.parFract != eNoFract){
        val1 = menu.parEditWnd.parValue / menu.parEditWnd.parFract;
        val2 = menu.parEditWnd.parValue - (menu.parEditWnd.parFract * val1);
        if((menu.parEditWnd.parFract == eHundreds) && (val2 < 10)){
            sprintf(tPar, "%u.0%u ", val1, val2);
        }else{
            sprintf(tPar, "%u.%u ", val1, val2);
        }
    }else{
        sprintf(tPar, "%u ", menu.parEditWnd.parValue);
    }
    strcat(tPar, menu.parEditWnd.parUnits);
    len = strlen(tPar);
    offs = SSD1306_LCDWIDTH/2 - (len*6)/2;
    ssd_putString6x8(offs, 28, tPar);
    //Buttons labels
    ssd_putString6x8(0, 56, tBack);
    ssd_putString6x8(40, 56, uArrow);
    ssd_putString6x8(82, 56, dArrow);
    ssd_putString6x8(104, 56, tSave);
}

/*!****************************************************************************
* @brief    Put text message box at the center of display
*/
void ssd_putMessage(void){
    uint8_t i, j, x, y, boxLength, offs;
    if(menu.message.show == true){
        //Message box parameters
        boxLength = menu.message.msgLen*6 + 8;
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
        offs = SSD1306_LCDWIDTH/2 - (menu.message.msgLen*6)/2;
        ssd_putString6x8(offs, 28, &menu.message.msgStr[0]);
    }
}

/*!****************************************************************************
* @brief    Put roll progress bar and fill it
*/
void ssd_putRollBar(int16_t angle, uint16_t border, uint8_t y, uint8_t height){
    uint8_t i, j, k, l, m, centerStrt, centerEnd, periphStrt, periphEnd, fillYBias, fillHgt, fillEnd;
    uint16_t x, yy, absAngle;
    int16_t zero = 0;
    border *= PRECISION;
    //Calculations
    absAngle = abs(angle);
    if(absAngle > border) absAngle = border;
    fillEnd = (absAngle * (SSD1306_LCDWIDTH >> 1)) / border;
    //Parameters calculation
    centerStrt = 1;
    centerEnd = height - centerStrt - 1;
    periphStrt = (height / 3) - 1;
    periphEnd = ((height << 1) / 3) + 1;
    fillYBias = y + centerStrt + 1;
    fillHgt = (centerEnd - centerStrt - 1) >> 2;
    k = SSD1306_LCDWIDTH >> 1;                                              //Progress bar center setting
    //Erase area
    ssd_clrAreaBff(0, y, height, SSD1306_LCDWIDTH);
    //Fill the line
    for(i = 0; i < fillEnd; i++){
        for(j = 0; j < fillHgt; j++){
            yy = (j << 2) + fillYBias;
            if(angle >= zero){
                x = k + i;
                ssdVideoBff.video[x + ((yy >> 3) << SSD1306_LCDWIDTH_LSH)] |=  (0xF << (yy & 7));
            }else{
                x = k - i - 1;
                ssdVideoBff.video[x + ((yy >> 3) << SSD1306_LCDWIDTH_LSH)] |=  (0xF << (yy & 7));
            }
        }
    }
    //Draw the pattern
    for(i = 0; i < SSD1306_LCDWIDTH; i++){
        k = i;
        //Pasting the one in the beginning
        if(i >= (SSD1306_LCDWIDTH >> 1)) k = i - (SSD1306_LCDWIDTH >> 1);
        //Draw lines
        for(j = 0; j < height; j++){
            if((i == ((SSD1306_LCDWIDTH >> 1) - 1)) || (i == (SSD1306_LCDWIDTH >> 1))){
                l = centerStrt;
                m = centerEnd;
            }else{
                l = periphStrt;
                m = periphEnd;
            }
            //Draw pattern
            if((k%ROLL_PERIOD == 0) || (k == 0)){
                if((j == 0) || (j > l) && (j < m) || (j == height - 1)) ssdVideoBff.video[i + (((y + j) >> 3) << SSD1306_LCDWIDTH_LSH)] |=  (1 << ((y + j) & 7));
            }
        }
    }
}

/*!****************************************************************************
* @brief    Put pitch progress bar and fill it
*/
void ssd_putPitchBar(int16_t angle, uint16_t border){
    uint8_t i, j, k, fillEnd;
    uint16_t x, y, absAngle;
    int16_t zero = 0;
    k = SSD1306_LCDHEIGHT >> 1;                                                 //Progress bar center setting
    border *= PRECISION;
    //Calculations
    absAngle = abs(angle);
    if(absAngle > border) absAngle = border;
    fillEnd = (absAngle * (PITCH_HEIGHT >> 1)) / border;
    //Erase area
    ssd_clrAreaBff(PITCH_X, 8, PITCH_HEIGHT, PITCH_WIDTH);
    //Fill the line
    for(i = 0; i < fillEnd; i++){
        for(j = 0; j < 4; j++){
            x = PITCH_X + j;
            if(angle >= zero){
                y = k - i - 1;
                ssdVideoBff.video[x + (y >> 3) * SSD1306_LCDWIDTH] |=  (1 << (y & 7));
            }else{
                y = k + i;
                ssdVideoBff.video[x + (y >> 3) * SSD1306_LCDWIDTH] |=  (1 << (y & 7));
            }
        }
    }
    //Draw the center
    for(i = 0; i < 2; i++){
        y = k - i;
        for(j = 0; j < 4; j++){
            x = PITCH_X + j;
            ssdVideoBff.video[x + (y >> 3) * SSD1306_LCDWIDTH] |=  (1 << (y & 7));
        }
    }
}

/*!****************************************************************************
* @brief    Put battery into x, y position with percentage
*/
void ssd_putBatt(uint8_t percentage, uint8_t chrgStat){
    uint8_t i, j, x, y, fillBiasX, fillBiasY;
    x = BATT_X;
    y = BATT_Y;
    fillBiasX = x + 2;
    fillBiasY = y + 2;
    //Protection
    if(percentage > 100) percentage = 100;
    //Draw empty battery
    for(i = 0; i < sizeof(ssdBatt); i++){
        for (j = 0; j < 8; j++){
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
    //Put "charge" symbol
    if(chrgStat != 0){
        for(i = 0; i < sizeof(chrgSym); i++){
            for (j = 0; j < 8; j++){
                if(chrgSym[i] & (1 << j)){
                    ssd_setpix(x+i, y+j, WHITE);
                }else{
                    ssd_setpix(x+i, y+j, BLACK);
                }
            }
        }
    }
}

/*!****************************************************************************
* @brief    Put char of 12x16 font into x, y position
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
*/
void ssd_setpix(uint8_t x, uint8_t y, uint8_t color){   //0 - black, 1 - white, 2 - inverse
    switch(color){
        case WHITE:   ssdVideoBff.video[x + (y >> 3) * SSD1306_LCDWIDTH] |=  (1 << (y & 7)); break;
        case BLACK:   ssdVideoBff.video[x + (y >> 3) * SSD1306_LCDWIDTH] &= ~(1 << (y & 7)); break;
        case INVERSE: ssdVideoBff.video[x + (y >> 3) * SSD1306_LCDWIDTH] ^=  (1 << (y & 7)); break;
    }
}

/*!****************************************************************************
* @brief    Clear video buffer
*/
void ssd_clearVidBff(void){
    memset(ssdVideoBff.video, 0, sizeof(ssdVideoBff.video));
}

/*!****************************************************************************
* @brief    Clear area in buffer (byte in height)
*/
void ssd_clrAreaBff(uint8_t x, uint8_t y, uint8_t height, uint8_t width){
    uint8_t i, j, hgtByte;
    hgtByte = height >> 3;
    if((hgtByte << 3) < height) hgtByte++;
    for(i = 0; i < hgtByte; i++){                                               //Rows
        for(j = 0; j < width; j++){                                             //Columns
            ssdVideoBff.video[(x + j) + ((y + (i << 3)) >> 3) * SSD1306_LCDWIDTH] = 0;
        }
    }
}


/*!****************************************************************************
* @brief    Set sleep mode
*/
void ug2864off(void){
    ug2864_com(0xAE);
    ug2864_com(0x8D);
    ug2864_com(0x10);
}

/*!****************************************************************************
* @brief    Set display ON
*/
void ug2864on(void){
    ug2864_com(0x8D);
    ug2864_com(0x14);
    ug2864_com(0xAF);
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
    ssdVideoBff.data = 0x40;
    ug2864_refresh();
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
    while(I2C1->ISR & I2C_ISR_BUSY) __NOP();
    I2CTx(sadd | 1, &ssdVideoBff.data, 1025);
}
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
