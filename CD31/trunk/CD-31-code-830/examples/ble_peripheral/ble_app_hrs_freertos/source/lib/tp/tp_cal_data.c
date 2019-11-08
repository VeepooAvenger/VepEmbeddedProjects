/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     tp_cal_data.c
** Last modified Date:   2018-03-23
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2018-03-23
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#include "tp_cal_data.h"
#include <math.h>

/****************************************************************
** Function name:       Tp_SubAbs
** Descriptions:        求两个数差的绝对值
** input parameters:    a,b
** output parameters: 	无
** Returned value:      绝对值
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
uint32_t Tp_SubAbs(uint32_t a, uint32_t b)
{
	if(a > b)
	{
		return a-b;
	}
	else
	{
		return b-a;
	}
}

/****************************************************************
** Function name:       Tp_CalTwoPointDist
** Descriptions:        求两个点的直线距离
** input parameters:    a:坐标 b:坐标
** output parameters: 	无
** Returned value:      返回a和b的直线距离
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
uint32_t Tp_CalTwoPointDist(coord_info_t a, coord_info_t b)
{
	return sqrt(Tp_SubAbs(a.x,b.x)*Tp_SubAbs(a.x,b.x)+Tp_SubAbs(a.y,b.y)*Tp_SubAbs(a.y,b.y));
}

/****************************************************************
** Function name:       Tp_CalTwoDataDist
** Descriptions:        求两个数据中点的直线距离
** input parameters:    a:数据a  b:数据b
** output parameters: 	无
** Returned value:      无
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
uint32_t Tp_CalTwoDataDist(TP_COORD_DATA_T a, TP_COORD_DATA_T b)
{
	coord_info_t tmp_a = {0};
	coord_info_t tmp_b = {0};
	tmp_a.x = a.x;
	tmp_a.y = a.y;
	tmp_b.x = b.x;
	tmp_b.y = b.y;
	return Tp_CalTwoPointDist(tmp_a,tmp_b);
}

/****************************************************************
** Function name:       Tp_CompareTwoDataSize
** Descriptions:        比较两点的大小
** input parameters:    a:数据a  b:数据b
** output parameters: 	无
** Returned value:      无
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
uint8_t Tp_CompareTwoDataSize(uint32_t a, uint32_t b)
{
	if(a > b)
    {
        return 1;
    }
    else if(a < b)
    {
        return 2;
    }
    
    return 0;
}

/****************************************************************
** Function name:       Tp_ComparePointInSquareArea
** Descriptions:        比较点是否在方形区域内
** input parameters:    point:点的信息  area_s:区域的信息
** output parameters: 	无
** Returned value:      1：在区域内  0：不在区域内
** Created by:          YX
** Created Date:        2018-04-25
****************************************************************/
uint8_t Tp_ComparePointInSquareArea(TP_COORD_DATA_T point, area_square_info_t area_s)
{
    if(area_s.wide == 0 || area_s.high == 0)
    {
        return 0;
    }
    
    if(point.x < area_s.point.x)
    {
        return 0;
    }
    
//    if(point.x > (area_s.point.x + area_s.wide - 1))
    if(point.x > (area_s.point.x + area_s.wide))
    {
        return 0;
    }
    
    if(point.y < area_s.point.y)
    {
        return 0;
    }
    
//    if(point.y > (area_s.point.y + area_s.high - 1))
    if(point.y > (area_s.point.y + area_s.high))
    {
        return 0;
    }
    
    return 1;

}


/****************************************************************
** Function name:       Tp_ComparePointInCircleArea
** Descriptions:        比较点是否在圆形区域内
** input parameters:    point:点的信息  area_s:区域的信息
** output parameters: 	无
** Returned value:      1：在区域内  0：不在区域内
** Created by:          YX
** Created Date:        2018-04-25
****************************************************************/
uint8_t Tp_ComparePointInCircleArea(TP_COORD_DATA_T point, area_circle_info_t area_s)
{
    coord_info_t tmp_a = {0};
	coord_info_t tmp_b = {0};
    uint16_t tmp_disp = 0;

    if(area_s.radius == 0)
    {
        return 0;
    }
    
    tmp_a.x = point.x;
	tmp_a.y = point.y;
	tmp_b.x = area_s.point.x;
	tmp_b.y = area_s.point.y;
    tmp_disp = Tp_CalTwoPointDist(tmp_a, tmp_b);
    
    if(tmp_disp > area_s.radius)
    {
        return 0;
    }
    
    return 1;

}

