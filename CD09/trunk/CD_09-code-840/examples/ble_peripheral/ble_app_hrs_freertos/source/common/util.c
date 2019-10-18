/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     util.c
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

/*********************************************************************
* 宏定义
*/

/*********************************************************************
* 全局变量
*/

/*********************************************************************
* 本地静态变量
*/
						
/*********************************************************************
* 本地函数声明
*/



/****************************************************************
** Function name:           Util_AverageByUint8
** Descriptions:            求Uint8 类型的数据平均值
** input parameters:        无
** output parameters:       无
** Returned value:          返回平均值
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint8_t Util_AverageByUint8(uint8_t *input_data, uint16_t input_len)
{
    uint16_t i = 0;
    uint32_t average_value = 0;
    
    if(input_data == NULL || input_len == 0)
    {
        return 0;
    }

    for(i = 0; i < input_len; i++)
    {
        average_value += input_data[i];
    }
    
    average_value = average_value/input_len;
    
    return average_value;
}


/****************************************************************
** Function name:           Util_CheckBufIsZero
** Descriptions:            检测数组是否全为0
** input parameters:        无
** output parameters:       无
** Returned value:          返回0:数组不为0，1：数组全为0
** Created by:              WJ              
** Created Date:            2017-08-11
*****************************************************************/ 
uint8_t Util_CheckBufIsZero(uint8_t *input_data, uint16_t input_len)
{
    uint16_t i = 0;
    
    if(input_data == NULL || input_len == 0)
    {
        return 0;
    }

    for(i = 0; i < input_len; i++)
    {
        if(input_data[i] != 0x00)
        {
            return 0;
        }
    }    
    return 1;
}

/****************************************************************
** Function name:           App_CommonSort
** Descriptions:            排序（大到小）
** input parameters:        data:比较数组；len:比较长度
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Util_CommonSort(uint8_t *data, uint8_t len)
{
	uint8_t i = 0;
    uint8_t j = 0;
	uint8_t temp = 0;
    
	for(i = 0; i < len-1; i++)
    {
        for(j = i + 1; j < len; j++)
        {
            if(data[i] < data[j])
            {
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}


/****************************************************************
** Function name:           Util_Itoa
** Descriptions:            数字转字符串
** input parameters:        x:输入字符串；
** output parameters:       无
** Returned value:          返回字符串地址
** Created by:              WJ              
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
char* Util_Reverse(char *x)
{
    int len = strlen(x);
    int i = 0;
    int n = len / 2;
    char tem = 0;
    
    for (i = 0; i < n; i++)
    {
        tem = x[i];
        x[i] = x[len - 1 - i];
        x[len - 1 - i] = tem;
    }
    return x;
}

/****************************************************************
** Function name:           Util_Itoa
** Descriptions:            数字转字符串
** input parameters:        n:输入数值；
** output parameters:       s:输出字符串
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-12-07
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint16_t Util_Itoa(int n, char s[])
{
    int i = 0;
//    int j = 0;
    int sign = 0;
    
    if((sign = n) < 0)//记录符号
    {
        n = -n;//使n成为正数
    }
    
    do
    {
        s[i++] = n % 10 + '0';//取下一个数字
        n = n / 10;
    }
    while(n > 0);//删除该数字
    
    if(sign < 0)
    {
        s[i++]='-';
    }
    s[i] = '\0';

    Util_Reverse(s);   
	
    return i;
}

