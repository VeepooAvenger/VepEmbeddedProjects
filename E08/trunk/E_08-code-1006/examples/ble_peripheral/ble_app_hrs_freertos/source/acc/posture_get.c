/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     posture_get.c
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:         统计每两秒的姿态数据，用于睡眠分析   
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include "string.h"
#include "gesture_test.h"

/*********************************************************************
* Macro Definition
*/

/*********************************************************************
* Global Variables
*/

//类型 从0到11分别是51-54,61-64,71-74;
const uint8_t  posture_type[12] = {51,52,53,54,61,62,63,64,71,72,73,74};    //统计姿态类型
static uint16_t posture_cnt[12] = {0};   //对应上面类型所出现的次数

static uint8_t sec_posture = 0;         //每秒的姿态
/*********************************************************************
* local functions
*/

/****************************************************************
** Function name:           Posture_GetSecPosture
** Descriptions:            获取每秒的姿态值
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              YX              
** Created Date:            2018-05-08
**----------------------------------------------------------------
*****************************************************************/
uint8_t Posture_GetSecPosture(void)
{
    return sec_posture;
}

/****************************************************************
** Function name:           Posture_Statistical
** Descriptions:            姿态统计，每两秒统计一次
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-06-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Posture_Statistical(void)
{
    static uint8_t posture_statistical = 0;
	uint8_t type = Gesture_GetGestureForSleep16bit();
    
    sec_posture = type;
    if(posture_statistical == 0)
    {
        posture_statistical = 1;
        return ;
    }
    posture_statistical = 0;
	switch (type)
	{
		case 51:
			posture_cnt[0]++;
			break;
		case 52:
			posture_cnt[1]++;
			break;
		case 53:
			posture_cnt[2]++;
			break;
		case 54:
			posture_cnt[3]++;
			break;
		case 61:
			posture_cnt[4]++;
			break;
		case 62:
			posture_cnt[5]++;
			break;
		case 63:
			posture_cnt[6]++;
			break;
		case 64:
			posture_cnt[7]++;
			break;
		case 71:
			posture_cnt[8]++;
			break;
		case 72:
			posture_cnt[9]++;
			break;
		case 73:
			posture_cnt[10]++;
			break;
		case 74:
			posture_cnt[11]++;
			break;
		
		default:
			break;
	}
}

/****************************************************************
** Function name:           Posture_StatisticalGet
** Descriptions:            获取当前统计的出现次数最多姿态的类型和次数，保存到五分钟数据中
** input parameters:        无
** output parameters:       max_type:姿态类型；max_cnt：姿态次数
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-06-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Posture_StatisticalGet(uint8_t *max_type, uint8_t *max_cnt)
{
    uint8_t     i = 0;
    uint8_t     temp_type = 0;
    uint16_t    temp_cnt = 0;
    
	temp_type = posture_type[0];
	temp_cnt = posture_cnt[0];
	for(i = 1; i < 12; i++)
	{
		if(temp_cnt < posture_cnt[i])
		{
			temp_type = posture_type[i];
			temp_cnt = posture_cnt[i];
		}
	}
    if(max_cnt != NULL)
    {
	    if(temp_cnt >= 255)
		{
		    *max_cnt = 254;
		}
		else
		{
            *max_cnt = temp_cnt & 0xFF;
		}
    }
    if(max_type != NULL)
    {
        *max_type = temp_type;
    }
}

/****************************************************************
** Function name:           Posture_StatisticalInit
** Descriptions:            清除当前统计的数据
** input parameters:        无  
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-06-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Posture_StatisticalInit(void)
{
	memset(posture_cnt, 0, sizeof(posture_cnt));
}


