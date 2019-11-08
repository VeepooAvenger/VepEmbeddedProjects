/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     tp_gesture.c
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

//V1.2版本修改记录：
//日期：20180818 作者：YX
//1.优化滑动效果，必须跨区域才能产生滑动效果，减少误触的滑动
//（例如：右滑动，必须从左边区域开始，右边区域结束，才算是正常的滑动，不然算是误触）

//V1.1版本修改记录：
//日期：20180817 作者：YX
//1.优化按键和长按获取位置
//2.去掉一些没用的数据

//1.修改若丢失按下状态，也要能进入正常识别出触摸结果
//2.修改若丢失放开状态，也要能进入正常识别出触摸结果
//3.增加对数据的判断，若数据低于一定值（5个），则识别结果出错，防止误识别
//*/延后修改

//V1.0版本修改记录：
//日期：20180425 作者：YX
//1.初始版本，实现滑动、单击、长按等状态

#include "tp_gesture.h"
#include "tp_cal_data.h"

tp_gesture_info_t tp_rsult_data = {0};      //返回的结果数据

static uint32_t tp_data_hz = 0;             //数据的频率
static uint32_t tp_part_time = 0;           //平移手势和其他手势分隔时间
static uint32_t tp_data_cnt = 0;            //数据的个数
    
static uint16_t tp_area_max_x = 0;          //TP x方向最长长度
static uint16_t tp_area_max_y = 0;          //TP y方向最长长度
//static uint16_t tp_area_max_xy = 0;         //TP直线最长长度

static uint8_t tp_x_margin_per = 0;         //X方向的边缘百分比
static uint8_t tp_y_margin_per = 0;         //Y方向的边缘百分比
static uint8_t tp_margin_state = 0;         //边缘的状态,0:不是边缘触摸，1:上边缘触摸，2：下边缘触摸，4:左边缘触摸，8：右边缘触摸
    
static uint16_t tp_click_max_dist1 = 0;      //单击的最大距离
static uint16_t tp_click_max_dist2 = 0;      //单击的最大距离

static uint16_t tp_long_press_time1 = 3000; //长按时间1
static uint16_t tp_long_press_time2 = 6000; //长按时间2

static uint16_t tp_current_stage = 0;               //手势算法的流程阶段
static tp_stage_data_t tp_stage_data_s[100] = {0};  //每次处理出来的数据
static uint8_t tp_stage_data_len = 0;
static uint32_t tp_stage_data_pos = 0;

//YX.20180718.增加参数
//static uint32_t tp_no_data_cnt = 0;     //连续出现没有数据的次数


/****************************************************************
** Function name:       Tp_CalPointInMargin
** Descriptions:        计算点是否处于边缘区域
** input parameters:    coord:点的坐标
** output parameters: 	无
** Returned value:      无
** Created by:          YX
** Created Date:        2018-03-26
****************************************************************/
uint8_t Tp_CalPointInMargin(coord_info_t coord)
{
    uint8_t margin_result= 0;
    
    //左边缘
	if(Tp_SubAbs(coord.x,0) < tp_x_margin_per*tp_area_max_x/100)
    {
        margin_result |= TP_LEFT_MARGIN_STATE;
    }
    //右边缘
    if(Tp_SubAbs(coord.x,tp_area_max_x) < tp_x_margin_per*tp_area_max_x/100)
    {
        margin_result |= TP_RIGHT_MARGIN_STATE;
    }
    //下边缘
    if(Tp_SubAbs(coord.y,0) < tp_y_margin_per*tp_area_max_y/100)
    {
        margin_result |= TP_DOWN_MARGIN_STATE;
    }
    //上边缘
    if(Tp_SubAbs(coord.y,tp_area_max_y) < tp_y_margin_per*tp_area_max_y/100)
    {
        margin_result |= TP_UP_MARGIN_STATE;
    }

	return margin_result;
}

/****************************************************************
** Function name:       Tp_CalTpPointInMargin
** Descriptions:        计算点是否处于边缘区域
** input parameters:    coord:Tp点的坐标
** output parameters: 	无
** Returned value:      无
** Created by:          YX
** Created Date:        2018-03-26
****************************************************************/
uint8_t Tp_CalTpPointInMargin(TP_COORD_DATA_T coord)
{
    coord_info_t coord_s = {0};
    coord_s.x = coord.x;
    coord_s.y = coord.y;
    
    return Tp_CalPointInMargin(coord_s);
}

/****************************************************************
** Function name:       Tp_CalTpTwoPointParam
** Descriptions:        计算两点一些参数，方向和距离
** input parameters:    coord_a:起始坐标，coord_b：结束坐标
** output parameters: 	direct：方向，dist：距离
** Returned value:      无
** Created by:          YX
** Created Date:        2018-03-26
****************************************************************/
void Tp_CalTpTwoPointParam(TP_COORD_DATA_T coord_a, TP_COORD_DATA_T coord_b, move_direct_info_t* direct, move_dist_info_t* dist)
{
    direct->x_direct = Tp_CompareTwoDataSize(coord_a.x,coord_b.x);
//    direct->y_direct = Tp_CompareTwoDataSize(coord_a.y,coord_b.y);
    direct->y_direct = Tp_CompareTwoDataSize(coord_b.y,coord_a.y);

    dist->x_dist = Tp_SubAbs(coord_a.x,coord_b.x);
    dist->y_dist = Tp_SubAbs(coord_a.y,coord_b.y);
    dist->xy_dist = Tp_CalTwoDataDist(coord_a,coord_b);

    direct->xy_direct = Tp_CompareTwoDataSize(dist->x_dist,dist->y_dist);
}

//正常30ms计算一次(出现结束状态时,马上进行一次计算,防止两次操作出现一次计算中)
/****************************************************************
** Function name:       Tp_GestureProcess
** Descriptions:        手势处理函数
** input parameters:    tp_data:tp数据    len:长度
** output parameters: 	无
** Returned value:      无
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
//TP_COORD_DATA_T test_point[2] = {0};
//uint32_t test_len = 0;
uint32_t Tp_GestureProcess(TP_ALL_DATA_T * tp_data ,uint32_t len, uint8_t resv)
{
    uint32_t error_code = 0;
    uint16_t now_mode = 0;
    TP_COORD_DATA_T point_start = {0};
    TP_COORD_DATA_T last_point = {0};
    uint32_t click_max_dist = 0;
    
    tp_rsult_data.tp_state = 0;
	if(len == 0)
	{
		return 4;
	}
    if(tp_current_stage == 0)
    {
        if(tp_data[0].status != TP_START_STATE)
        {
            return 3;
        }
    }
    //上一次坐标(上一次的最后一个坐标，第一次进来，则是第一个点的坐标)
    if(tp_data_cnt != 0)
    {
        tp_rsult_data.last_coord = tp_rsult_data.now_coord;
    }
    else
    {
        tp_rsult_data.last_coord.x = tp_data[0].coord.x;
        tp_rsult_data.last_coord.y = tp_data[0].coord.y;
    }
    
    tp_data_cnt += len;
    tp_rsult_data.tp_state = 1;
    
    if(tp_data[0].status == TP_START_STATE)
    {
        //手势起始
        tp_current_stage |= 0x01;
        //是否处于边缘
        tp_margin_state = Tp_CalTpPointInMargin(tp_data[0].coord);
        if(tp_margin_state != 0)
        {
            tp_rsult_data.total_mode |= TP_MODE_MARGIN_SLIDE;
            tp_rsult_data.margin_state = tp_margin_state;
        }
        //起始坐标
        tp_rsult_data.start_coord.x = tp_data[0].coord.x;
        tp_rsult_data.start_coord.y = tp_data[0].coord.y;
    }
    
    if(tp_data[len-1].status == TP_STOP_STATE)
    {
        //手势结束
        tp_current_stage |= 0x80;
        tp_rsult_data.tp_state = 2;
//        return error_code;
    }
    
    last_point.x = tp_rsult_data.last_coord.x;
    last_point.y = tp_rsult_data.last_coord.y;
	//在单击范围内
    if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
    {
        click_max_dist = tp_click_max_dist1;
    }
    else
    {
        click_max_dist = tp_click_max_dist2;
    }
    
    if(Tp_CalTwoDataDist(last_point,tp_data[len-1].coord) <= click_max_dist)
    {
        now_mode |= TP_MODE_CLICK;
//		//单击的范围内
//		for(uint32_t i = 0; i < len-2; i++)
//		{
//			if(Tp_CalTwoDataDist(tp_data[i].coord[0],tp_data[i+1].coord[0]) > click_max_dist)
//			{
//                //出现超出单击范围的动作,应该作为滑动来处理
//                now_mode |= TP_MODE_SLIDE;
//                now_mode &= ~TP_MODE_CLICK;
//                break;
//			}
//		}
    }
    else
    {
//        test_point[0] = last_point;
//        test_point[1] = tp_data[len-1].coord;
//        test_len = len;
//        tp_rsult_data.now_mode = test_len;
        //滑动事件
        now_mode |= TP_MODE_SLIDE;
    }
    
    tp_rsult_data.now_mode = now_mode;
//    if(now_mode == TP_MODE_SLIDE)
//    {
//        //计算当次移动距离和滑动方向
//        Tp_CalTpTwoPointParam(tp_data[0].coord[0], tp_data[len-1].coord[0], &tp_rsult_data.now_direct, &tp_rsult_data.each_move_dist);
//    }
     
    if((tp_rsult_data.now_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE)
    {
        tp_rsult_data.total_mode |= TP_MODE_SLIDE;
        tp_rsult_data.total_mode &= ~TP_MODE_CLICK;
    }
    else
    {
        if((tp_rsult_data.total_mode & TP_MODE_SLIDE) != TP_MODE_SLIDE)
        {
            tp_rsult_data.total_mode |= TP_MODE_CLICK;
        }
    }

    if((tp_rsult_data.total_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE)
    {
        //计算当次移动距离和滑动方向
        Tp_CalTpTwoPointParam(tp_data[0].coord, tp_data[len-1].coord, &tp_rsult_data.now_direct, &tp_rsult_data.each_move_dist);
    }
    
    //连续小范围移动，达到滑动的最低距离的情况
    if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
    {
        last_point.x = tp_rsult_data.start_coord.x;
        last_point.y = tp_rsult_data.start_coord.y;
        if(Tp_CalTwoDataDist(last_point,tp_data[len-1].coord) > click_max_dist)
        {
//            test_point[0] = last_point;
//            test_point[1] = tp_data[len-1].coord;
//            test_len = len;
//            tp_rsult_data.now_mode = test_len;
            
            tp_rsult_data.total_mode |= TP_MODE_SLIDE;
            tp_rsult_data.total_mode &= ~TP_MODE_CLICK;
            //从开始到滑动触发的距离全部算法当次移动的距离
            Tp_CalTpTwoPointParam(last_point, tp_data[len-1].coord, &tp_rsult_data.now_direct, &tp_rsult_data.each_move_dist);
        }
    }
    
    if(tp_data_cnt * 1000/tp_data_hz > tp_part_time)
    {
        //达到平移的最低时间
        if((tp_rsult_data.total_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE)
        {
            tp_rsult_data.total_mode |= TP_MODE_TRANSLATION;
        }
    }
    
    //当前坐标(最后一个坐标)
    tp_rsult_data.now_coord.x = tp_data[len-1].coord.x;
    tp_rsult_data.now_coord.y = tp_data[len-1].coord.y;
    
    //存储当次计算结果
    tp_stage_data_s[tp_stage_data_pos%100].mode = tp_rsult_data.now_mode;
    tp_stage_data_s[tp_stage_data_pos%100].last_coord = tp_rsult_data.last_coord;
    tp_stage_data_s[tp_stage_data_pos%100].now_coord = tp_rsult_data.now_coord;
    tp_stage_data_s[tp_stage_data_pos%100].direct = tp_rsult_data.now_direct;
    tp_stage_data_s[tp_stage_data_pos%100].move_dist = tp_rsult_data.each_move_dist;
    tp_stage_data_pos++;
    tp_stage_data_len++;
    if(tp_stage_data_len > 100)tp_stage_data_len = 100;
    
    //计算总距离和总方向
    point_start.x = tp_rsult_data.start_coord.x;
    point_start.y = tp_rsult_data.start_coord.y;
    Tp_CalTpTwoPointParam(point_start, tp_data[len-1].coord, &tp_rsult_data.total_direct, &tp_rsult_data.total_move_dist);
    
    if((tp_rsult_data.total_mode & TP_MODE_LONG_PRESS_3S) != TP_MODE_LONG_PRESS_3S)
    {
        if(tp_data_cnt * 1000/tp_data_hz > tp_long_press_time1)
        {
            if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
            {
                //3S长按
                tp_rsult_data.total_mode = TP_MODE_LONG_PRESS_3S;
//                tp_rsult_data.total_mode &= ~TP_MODE_CLICK;
            }
        }
    }
        
    if((tp_rsult_data.total_mode & TP_MODE_LONG_PRESS_6S) != TP_MODE_LONG_PRESS_6S)
    {
        
        if(tp_data_cnt * 1000/tp_data_hz > tp_long_press_time2)
        {
            if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
            {
                //3S长按
                tp_rsult_data.total_mode = TP_MODE_LONG_PRESS_6S;
//                tp_rsult_data.total_mode &= ~TP_MODE_CLICK;
            }
        }
    }
    return error_code;
}

//获取手势结果
/****************************************************************
** Function name:       Tp_GetGesture
** Descriptions:        获取手势结果
** input parameters:    无
** output parameters: 	无
** Returned value:      手势结果结构体
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
tp_gesture_info_t * Tp_GetGesture(void)
{
    return &tp_rsult_data;
}

//传入支持的操作,使用方式待定
/****************************************************************
** Function name:       Tp_SetGestureParam
** Descriptions:        手势处理函数
** input parameters:    state:支持的手势
** output parameters: 	无
** Returned value:      返回手势数据
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
tp_result_t Tp_SetGestureParam(uint32_t state)
{
    tp_result_t result_s = {0};
    uint8_t margin_state = 0;
    uint8_t margin_x = 0;
    uint8_t margin_y = 0;
    
    if(state == 0 || tp_rsult_data.tp_state == 0)
    {
        return result_s;
    }
    if(tp_rsult_data.tp_state == 2)
    {
        //单击
        if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
        {
            if((state & TP_GEST_CLICK)== TP_GEST_CLICK)
            {
                result_s.mode = TP_MODE_CLICK;
                result_s.start_coord = tp_rsult_data.start_coord;
                result_s.stop_coord = tp_rsult_data.now_coord;
                return result_s;
            }
        }
    }
    
    //长按3s
    if((tp_rsult_data.total_mode & TP_MODE_LONG_PRESS_3S) == TP_MODE_LONG_PRESS_3S)
    {
        if((state & TP_GEST_LONG_PRESS_3S)== TP_GEST_LONG_PRESS_3S)//TP_GEST_LONG_PRESS_6S
        {
            result_s.mode = TP_MODE_LONG_PRESS_3S;
            result_s.start_coord = tp_rsult_data.start_coord;
            result_s.stop_coord = tp_rsult_data.now_coord;
            return result_s;
        }
    }
    
    //长按6s
    if((tp_rsult_data.total_mode & TP_MODE_LONG_PRESS_6S) == TP_MODE_LONG_PRESS_6S)
    {
        if((state & TP_GEST_LONG_PRESS_6S)== TP_GEST_LONG_PRESS_6S)//TP_GEST_LONG_PRESS_6S
        {
            result_s.mode = TP_MODE_LONG_PRESS_6S;
            result_s.start_coord = tp_rsult_data.start_coord;
            result_s.stop_coord = tp_rsult_data.now_coord;
            return result_s;
        }
    }
    
    if((state & TP_GEST_TRANSLATION)== TP_GEST_TRANSLATION)
    {
        //若是平移，则窗口跟着，坐标移动，不管方向和距离
        result_s.mode = TP_MODE_TRANSLATION;
        result_s.start_coord = tp_rsult_data.last_coord;
        result_s.stop_coord = tp_rsult_data.now_coord;
        return result_s;
    }
    else
    {
        if(((tp_rsult_data.total_mode & TP_MODE_MARGIN_SLIDE) == TP_MODE_MARGIN_SLIDE)
           && ((tp_rsult_data.total_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE))
        {
            if((state & TP_GEST_LEFT_MARGIN_SLIDE)== TP_GEST_LEFT_MARGIN_SLIDE)
            {
                if((tp_rsult_data.margin_state & TP_LEFT_MARGIN_STATE) == TP_LEFT_MARGIN_STATE)
                {
                    //查看是否是左边缘，查看X方向的移动方向和距离
                    margin_state |= 1;
                    margin_x = TP_LEFT_MARGIN_STATE;
                }
            }
            if((state & TP_GEST_RIGHT_MARGIN_SLIDE)== TP_GEST_RIGHT_MARGIN_SLIDE)
            {
                if((tp_rsult_data.margin_state & TP_RIGHT_MARGIN_STATE) == TP_RIGHT_MARGIN_STATE)
                {
                    //查看是否是右边缘，查看X方向的移动方向和距离
                    margin_state |= 1;
                    margin_x = TP_RIGHT_MARGIN_STATE;
                }
            }
            if((state & TP_GEST_UP_MARGIN_SLIDE)== TP_GEST_UP_MARGIN_SLIDE)
            {
                if((tp_rsult_data.margin_state & TP_UP_MARGIN_STATE) == TP_UP_MARGIN_STATE)
                {
                    //查看是否是上边缘，查看Y方向的移动方向和距离
                    margin_state |= 2;
                    margin_y = TP_UP_MARGIN_STATE;
                }
            }
            if((state & TP_GEST_DOWN_MARGIN_SLIDE)== TP_GEST_DOWN_MARGIN_SLIDE)
            {
                if((tp_rsult_data.margin_state & TP_DOWN_MARGIN_STATE) == TP_DOWN_MARGIN_STATE)
                {
                    //查看是否是下边缘，查看Y方向的移动方向和距离
                    margin_state |= 2;
                    margin_y = TP_DOWN_MARGIN_STATE;
                }
            }
            
            if((margin_state == 1) || ((margin_state == 3)&&(tp_rsult_data.now_direct.xy_direct == 1)))
            {
                result_s.margin = margin_x;
                result_s.mode = TP_MODE_SLIDE;
                result_s.direct = tp_rsult_data.now_direct.x_direct;
                result_s.dist = tp_rsult_data.each_move_dist.x_dist;
                result_s.start_coord = tp_rsult_data.last_coord;
                result_s.stop_coord = tp_rsult_data.now_coord;
                return result_s;
            }
            else if((margin_state == 2) || ((margin_state == 3)&&(tp_rsult_data.now_direct.xy_direct == 2)))
            {
                result_s.margin = margin_y;
                result_s.mode = TP_MODE_SLIDE;
                result_s.direct = tp_rsult_data.now_direct.y_direct;
                result_s.dist = tp_rsult_data.each_move_dist.y_dist;
                result_s.start_coord = tp_rsult_data.last_coord;
                result_s.stop_coord = tp_rsult_data.now_coord;
                return result_s;
            }

        }
        //不支持边缘滑动
        if(margin_state == 0)
        {
            //查看当前模式是否是滑动，不是的话，不做任何操作
            if((tp_rsult_data.total_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE)
            {
                if((state & TP_GEST_X_SLIDE)== TP_GEST_X_SLIDE)
                {
                    //查看X方向是否有移动，移动的方向和距离分别是多少
                    if(tp_rsult_data.now_direct.x_direct != TP_NULL_SLIDE_STATE)
                    {
                        result_s.margin = margin_x;
                        result_s.mode = TP_MODE_SLIDE;
                        result_s.direct = tp_rsult_data.now_direct.x_direct;
                        result_s.dist = tp_rsult_data.each_move_dist.x_dist;
                        result_s.start_coord = tp_rsult_data.last_coord;
                        result_s.stop_coord = tp_rsult_data.now_coord;
                        return result_s;
                    }
                }
                if((state & TP_GEST_Y_SLIDE)== TP_GEST_Y_SLIDE)
                {
                    //查看Y方向是否有移动，移动的方向和距离分别是多少
                    if(tp_rsult_data.now_direct.y_direct != TP_NULL_SLIDE_STATE)
                    {
                        result_s.margin = margin_y;
                        result_s.mode = TP_MODE_SLIDE;
                        result_s.direct = tp_rsult_data.now_direct.y_direct;
                        result_s.dist = tp_rsult_data.each_move_dist.y_dist;
                        result_s.start_coord = tp_rsult_data.last_coord;
                        result_s.stop_coord = tp_rsult_data.now_coord;
                        return result_s;
                    }
                }
            }
        }
    }
    return result_s;
}

//初始化(采集数据的频率:data_hz,边缘百分比x、y:margin_x、margin_y,平移手势和其他手势区分时间:time)
/****************************************************************
** Function name:       Tp_GestureInit
** Descriptions:        手势处理函数
** input parameters:    data_hz:数据的频率
                        max_x_coord:x方向的最大坐标
                        max_y_coord:y方向的最大坐标
                        margin_x:x方向的边缘百分比
                        margin_y:y方向的边缘百分比
                        time:平移手势与其他手势的区分时间
                        slide_min_dist:滑动最小距离（直线距离的比）
** output parameters: 	无
** Returned value:      无
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
uint32_t Tp_GestureInit(uint32_t data_hz, uint16_t max_x_coord, uint16_t max_y_coord, uint8_t margin_x, uint8_t margin_y, uint32_t time, uint32_t slide_min_dist)
{
//    coord_info_t a = {0};
//    coord_info_t b = {0};
    if(data_hz == 0 || max_x_coord == 0 || max_y_coord == 0 
       || slide_min_dist == 0)
    {
        return 0;
    }
    tp_current_stage = 0;
    tp_area_max_x = max_x_coord;
    tp_area_max_y = max_y_coord;
//    b.x = tp_area_max_x;
//    b.y = tp_area_max_y;
//    tp_area_max_xy = Tp_CalTwoPointDist(a, b);
    tp_click_max_dist1 = slide_min_dist;//tp_area_max_xy/slide_min_dist;
    tp_click_max_dist2 = slide_min_dist;//tp_click_max_dist1;
    tp_data_hz = data_hz;
    tp_part_time = time;
    tp_x_margin_per = margin_x;
    tp_y_margin_per = margin_y;
    tp_data_cnt = 0;
    memset(&tp_rsult_data, 0, sizeof(tp_gesture_info_t));
    tp_stage_data_len = 0;
    tp_stage_data_pos = 0;
    memset(&tp_stage_data_s, 0, sizeof(tp_stage_data_t));
    return 1;
}

