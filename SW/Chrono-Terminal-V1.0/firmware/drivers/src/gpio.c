/*!****************************************************************************
* @file     gpio.c
* @author   Storozhenko Roman - D_EL, 14.08.2016 - 4eef
* @version  V1.0
* @date     20.07.2016
* @date     02.08.2016  fix set nAF
* @brief    gpio driver for stm32F0
*/

/*!****************************************************************************
* Include
*/
#include "gpio.h"

/*!****************************************************************************
* MEMORY
*/
pinMode_type   const pinsMode[] = {
/*0 */  makepin(GPIOB,  8,  floatingInput,              0,  0),     //ChrgStatus
/*1 */  makepin(GPIOF,  0,  alternateFunctionOpenDrain, 0,  1),     //I2C1_SDA
/*2 */  makepin(GPIOF,  1,  alternateFunctionOpenDrain, 0,  1),     //I2C1_SCL
/*3 */  makepin(GPIOA,  0,  outPushPull,                0,  0),     //Buzzer
/*4 */  makepin(GPIOA,  1,  floatingInput,              0,  0),     //LIS3_INT1
/*5 */  makepin(GPIOA,  2,  outPushPull,                0,  0),     //OLED_RES
/*6 */  makepin(GPIOA,  3,  inputWithPullUp,            0,  0),     //SW4
/*7 */  makepin(GPIOA,  4,  analogMode,                 0,  0),     //VBAT
/*8 */  makepin(GPIOA,  5,  alternateFunctionPushPull,  0,  2),     //IRR_Data
/*9 */  makepin(GPIOA,  6,  outPushPull,                0,  0),     //IRR_VCC
/*10*/  makepin(GPIOA,  7,  inputWithPullUp,            1,  0),     //SW1
/*11*/  makepin(GPIOB,  1,  inputWithPullUp,            1,  0),     //SW2
/*11*/  makepin(GPIOA,  11, floatingInput,              0,  0),     //USB_DM
/*11*/  makepin(GPIOA,  12, floatingInput,              0,  0),     //USB_DP
/*14*/  makepin(GPIOA,  13, inputWithPullUp,            1,  0),     //SW3
/*15*/  makepin(GPIOA,  14, inputWithPullUp,            1,  0),     //SW4
};
uint32_t pinNum = sizeof(pinsMode) / sizeof(pinMode_type);

/*!****************************************************************************
* InitAllGpio
*/
void initGpios(void){
//    RCC->APB2ENR    |= RCC_APB2ENR_AFIOEN;                      //AFIO
//#if (JTAG_OFF > 0)
//    AFIO->MAPR      |= AFIO_MAPR_SWJ_CFG_1; //JTAG
//#endif
    pinMode_type *pgpios;
    pinMode_type *pgpiosEnd;
    
    pgpios = (pinMode_type*)pinsMode;
    pgpiosEnd = pgpios + pinNum;
    
    while(pgpios < pgpiosEnd){
        gppin_init(pgpios->p, pgpios->npin, pgpios->mode, pgpios->iniState, pgpios->nAF);
        pgpios++;
    }
}

/*!****************************************************************************
*
*/
void gppin_init(GPIO_TypeDef *port, uint8_t npin, gpioMode_type mode, uint8_t iniState, uint8_t nAF){
    //Clock enable
    if(port == GPIOA)   RCC->AHBENR    |= RCC_AHBENR_GPIOAEN;
    if(port == GPIOB)   RCC->AHBENR    |= RCC_AHBENR_GPIOBEN;
    if(port == GPIOC)   RCC->AHBENR    |= RCC_AHBENR_GPIOCEN;
//    if(port == GPIOD)   RCC->AHBENR    |= RCC_AHBENR_GPIODEN;
//    if(port == GPIOE)   RCC->AHBENR    |= RCC_AHBENR_GPIOEEN;
    if(port == GPIOF)   RCC->AHBENR    |= RCC_AHBENR_GPIOFEN;
//    if(port == GPIOG)   RCC->AHBENR    |= RCC_AHBENR_GPIOGEN;
//    if(port == GPIOH)   RCC->AHBENR    |= RCC_AHBENR_GPIOHEN;
//    if(port == GPIOO)   RCC->AHBENR    |= RCC_AHBENR_GPIOOEN;
    
    if(iniState != 0){
        port->BSRR = (1<<npin);
    }
    else{
        port->BRR = (1<<npin);
    }
    
    /*
    * Clear bit field
    */
    port->MODER         &= ~(0x03 << (2 * npin));
    port->OTYPER        &= ~(1<<npin);
    port->PUPDR         &= ~(GPIO_RESERVED << (2*npin));
    port->AFR[npin / 8] &= ~(GPIO_AFRL_AFRL0_Msk << (4*(npin % 8)));
    
    switch(mode){
        case analogMode:
            port->MODER |= GPIO_ANALOG_MODE << (2*npin);
            break;
  
        case floatingInput:
            break;
            
        case inputWithPullUp:
            port->PUPDR |= GPIO_PULL_UP << (2*npin);
            break;
            
        case inputWithPullDown:
            port->PUPDR |= GPIO_PULL_DOWN << (2*npin);
            break;
            
        case outPushPull:
            port->MODER |= GPIO_GP_OUT << (2*npin);
            port->OTYPER |= GPIO_PUSH_PULL << npin;
            break;
            
        case outOpenDrain:
            port->MODER |= GPIO_GP_OUT << (2*npin);
            port->OTYPER |= GPIO_OPEN_DRAIN << npin;
            break;
            
       case alternateFunctionPushPull:
            port->MODER |= GPIO_AF_MODE << (2*npin);
            port->OTYPER |= GPIO_PUSH_PULL << npin;
            break;
            
        case alternateFunctionOpenDrain:
            port->MODER |= GPIO_AF_MODE << (2*npin);
            port->OTYPER |= GPIO_OPEN_DRAIN << npin;
            break;   
    }
    
    //Set number alternate function
    port->AFR[npin / 8] |= nAF << (4*(npin % 8));
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* D_EL *****/
