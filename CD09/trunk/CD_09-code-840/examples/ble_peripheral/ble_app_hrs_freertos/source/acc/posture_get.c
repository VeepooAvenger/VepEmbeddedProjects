/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     posture_get.c
** Last modified Date:   2017-08-21
** Last SYS_VERSION:     0.0.0
** Descriptions:         ͳ��ÿ�������̬���ݣ�����˯�߷���   
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

//���� ��0��11�ֱ���51-54,61-64,71-74;
const uint8_t  posture_type[12] = {51,52,53,54,61,62,63,64,71,72,73,74};    //ͳ����̬����
static uint16_t posture_cnt[12] = {0};   //��Ӧ�������������ֵĴ���

/*********************************************************************
* local functions
*/


/****************************************************************
** Function name:           Posture_Statistical
** Descriptions:            ��̬ͳ�ƣ�ÿ����ͳ��һ��
** input parameters:        ��  
** output parameters:       ��
** Returned value:          ��
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
** Descriptions:            ��ȡ��ǰͳ�Ƶĳ��ִ��������̬�����ͺʹ��������浽�����������
** input parameters:        ��
** output parameters:       max_type:��̬���ͣ�max_cnt����̬����
** Returned value:          ��
** Created by:              WJ              
** Created Date:            2017-06-22
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Posture_StatisticalGet(uint8_t *max_type, uint8_t *max_cnt)
{
    uint8_t i = 0;
    
	*max_type = posture_type[0];
	*max_cnt = posture_cnt[0];
	for(i = 1; i < 12; i++)
	{
		if(*max_cnt < posture_cnt[i])
		{
			*max_type = posture_type[i];
			*max_cnt = posture_cnt[i];
		}
	}
    if(*max_cnt >= 255) *max_cnt = 254;
}

/****************************************************************
** Function name:           Posture_StatisticalInit
** Descriptions:            �����ǰͳ�Ƶ�����
** input parameters:        ��  
** output parameters:       ��
** Returned value:          ��
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


