/*****************************************************************
Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
File name:         bsp_led.h
Description: 
Author:             WJ
Version: 
Date:             2017-01-04
History: 
*****************************************************************/
#include "board.h"
#include "app_error.h"
#include <string.h>

/*********************************************************************
* Macro Definition
*/
#define BSP_LED_MAX_GEAR    7
/*********************************************************************
* Global Variables
*/

/*********************************************************************
* local functions
*/


/****************************************************************
** Function name:            Bsp_LED_Init
** Descriptions:            
** input parameters:          
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Bsp_LED_Init(void)
{
//    PIN_HIGH_RESISTANCE(GC1_PIN);  
}

/****************************************************************
** Function name:            Bsp_LED_Gear
** Descriptions:            
** input parameters:          
** output parameters:
** Returned value:
** Created by:                WJ              
** Created Date:               2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Bsp_LED_Gear(uint8_t gear)
{
//    if(gear > BSP_LED_MAX_GEAR)
//    {
//        gear = 4;
//    }
//    
//    switch(gear)
//    {
//    case 0:
//        PIN_HIGH_RESISTANCE(GC1_PIN);    
//        break;        
//    default:
//        PIN_LOW(GC1_PIN);     
////        PIN_HIGH(GC1_PIN);      
//        break;
//    }
    
}


void Bsp_LED_Open(uint8_t leve)
{
//    PIN_HIGH(LDO_EN_PIN); 
//    Bsp_LED_Gear(leve);    
}

void Bsp_LED_Close(void)
{
//    PIN_LOW(LDO_EN_PIN); 
//    Bsp_LED_Gear(0);    
}

