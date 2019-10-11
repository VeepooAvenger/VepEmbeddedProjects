/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     bsp_led.c
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-07-28
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <string.h>
#include "app_error.h"
#include "board.h"
#include "bsp_led.h"

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
** Function name:           Led_Init
** Descriptions:            心率等初始化
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Led_Init(void)
{    
    PIN_LOW(LDO_EN_PIN);
    Led_SetGear(0);  
}

/****************************************************************
** Function name:           Led_SetGear
** Descriptions:            设置亮灯等级
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Led_SetGear(uint8_t gear)
{
    if(gear > BSP_LED_MAX_GEAR)
    {
        gear = 1;
    }
    
    switch(gear)
    {
    case 0:
        PIN_HIGH_RESISTANCE(GC1_PIN);
        PIN_HIGH_RESISTANCE(GC2_PIN);
        PIN_HIGH_RESISTANCE(GC3_PIN);
        break;
    case 1:
        PIN_HIGH_RESISTANCE(GC1_PIN);
        PIN_LOW(GC2_PIN);
        PIN_HIGH_RESISTANCE(GC3_PIN);
        break;
    case 2:
        PIN_HIGH_RESISTANCE(GC1_PIN);
        PIN_HIGH_RESISTANCE(GC2_PIN);
        PIN_LOW(GC3_PIN);    
        break;    
    case 3:
        PIN_HIGH_RESISTANCE(GC1_PIN);
        PIN_LOW(GC2_PIN);
        PIN_LOW(GC3_PIN);    
        break;        
    case 4:
        PIN_LOW(GC1_PIN);
        PIN_HIGH_RESISTANCE(GC2_PIN);
        PIN_HIGH_RESISTANCE(GC3_PIN);    
        break;        
    case 5:
        PIN_LOW(GC1_PIN);
        PIN_LOW(GC2_PIN);
        PIN_HIGH_RESISTANCE(GC3_PIN);    
        break;    
    case 6:
        PIN_LOW(GC1_PIN);
        PIN_HIGH_RESISTANCE(GC2_PIN);
        PIN_LOW(GC3_PIN);    
        break;    
    case 7:
        PIN_LOW(GC1_PIN);
        PIN_LOW(GC2_PIN);
        PIN_LOW(GC3_PIN);    
        break;  
        
    default:
        PIN_LOW(GC1_PIN);
        PIN_HIGH_RESISTANCE(GC2_PIN);
        PIN_HIGH_RESISTANCE(GC3_PIN);          
        break;
    }
}


/****************************************************************
** Function name:           Led_OpenByLevel
** Descriptions:            开灯
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Led_OpenByLevel(uint8_t leve)
{
    PIN_HIGH(LDO_EN_PIN); 
    Led_SetGear(leve);    
}

/****************************************************************
** Function name:           Led_CloseAll
** Descriptions:            关灯
** input parameters:          
** output parameters:
** Returned value:
** Created by:              WJ              
** Created Date:            2017-01-04
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Led_CloseAll(void)
{
    PIN_LOW(LDO_EN_PIN); 
    Led_SetGear(0);    
}

