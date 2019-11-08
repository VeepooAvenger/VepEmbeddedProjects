/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  tp_manage.c
** Last modified Date:         2018-06-07
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  YX
** Createddate:                2018-06-07
** SYS_VERSION:                0.0.1
** Descriptions:               TP
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "tp_manage.h"
#include "nrf_delay.h"
#include "gui_thread.h"
#include "lcd_driver_tft.h"

//WJ.20180815.数据缓存大小
#define TP_DATA_CACHE_MAX_NUM   100

void zinitix_init(void);
void zinitix_resume(void);
void zinitix_suspend(uint16_t mode_data);
void zinitix_shutdown(void);
void zinitix_init(void);
void zinitix_ClearINT(void);


extern TP_ALL_DATA_T data_cache[TP_DATA_CACHE_MAX_NUM];
extern uint8_t simul_data_cache_num;

/*********************************************************************
* Macro Definition
*/
//YX20180611.增加长按状态识别标志
static uint8_t key_long_3s_flg = 0;
static uint8_t key_long_6s_flg = 0;

static uint8_t press_down_status = 0;   //TP按下的状态

static uint8_t tp_now_sleep_status = 0;         //TP当前休眠状态      0：休眠中、1：未休眠、2：TP关机

static uint8_t tp_i2c_status = 0;       //TP通信状态

static tp_param_t  tp_param_s;                  // TP相关参数

static uint8_t tp_clear_int_status = 0; //TP清除中断标志位的状态

/****************************************************************
** Function name:               Tp_GetI2cIntStatus
** Descriptions:                获取是否需要清除中断标志的状态
** input parameters:            无
** output parameters:           无
** Returned value:              0：没有使用中 1:正在使用中
** Created by:                  YX     
** Created Date:                2018-07-03
****************************************************************/
uint8_t Tp_GetI2cIntStatus(void)
{
    return tp_clear_int_status;
}

/****************************************************************
** Function name:               Tp_SetI2cIntStatus
** Descriptions:                设置是否需要清除中断标志的状态
** input parameters:            无
** output parameters:           无
** Returned value:              0：没有使用中 1:正在使用中
** Created by:                  YX     
** Created Date:                2018-07-03
****************************************************************/
uint8_t Tp_SetI2cIntStatus(uint8_t status)
{
    tp_clear_int_status = status;
    
    return tp_i2c_status;
}


/****************************************************************
** Function name:               Tp_GetI2cStatus
** Descriptions:                获取TP通信状态
** input parameters:            无
** output parameters:           无
** Returned value:              0：没有使用中 1:正在使用中
** Created by:                  YX     
** Created Date:                2018-07-03
****************************************************************/
uint8_t Tp_GetI2cStatus(void)
{
    return tp_i2c_status;
}

/****************************************************************
** Function name:               Tp_SetI2cStatus
** Descriptions:                设置TP通信状态
** input parameters:            无
** output parameters:           无
** Returned value:              0：没有使用中 1:正在使用中
** Created by:                  YX     
** Created Date:                2018-07-03
****************************************************************/
uint8_t Tp_SetI2cStatus(uint8_t status)
{
    if(Tp_GetI2cIntStatus())
    {
        Tp_SetI2cIntStatus(0);
        zinitix_ClearINT();
    }
    
    tp_i2c_status = status;
    
    return tp_i2c_status;
}

/****************************************************************
** Function name:               Tp_IsSleepWakeup
** Descriptions:                是否休眠唤醒
** input parameters:            无
** output parameters:           无
** Returned value:              0：不属于休眠唤醒 1:属于休眠唤醒（触摸唤醒）
** Created by:                  YX     
** Created Date:                2018-06-11
****************************************************************/
uint8_t Tp_IsSleepWakeup(void)
{
    if(tp_now_sleep_status == 0)
    {
        tp_now_sleep_status = 1;
//        tp_now_wakeup_status = 1;
        //YX.20180703.触摸唤醒TP，调用唤醒程序的接口
        Gui_ThreadAddKeyEvent(BUTTON_EVENT_CLICK, BUTTON_TYPE_TP);
        return 1;
    }
    return 0;//tp_now_wakeup_status;
}

/****************************************************************
** Function name:               Tp_GetPressDownSatus
** Descriptions:                获取TP的按下状态
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX     
** Created Date:                2018-06-11
****************************************************************/
uint8_t Tp_GetPressDownSatus(void)
{
    return press_down_status;
}

/****************************************************************
** Function name:               Tp_ClearInfo
** Descriptions:                清除TP的相关参数
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX     
** Created Date:                2018-06-07
****************************************************************/
void Tp_ClearInfo(void)
{
    //存在问题，如果清除了首次触摸的数据，暂时会有问题，后面解决
    memset(&tp_param_s,0,sizeof(tp_param_t));
    
//    memset(data_cache,0,sizeof(data_cache));
//    simul_data_cache_num = 0;
}

/****************************************************************
** Function name:               Tp_GetResultStatus
** Descriptions:                解析TP结果
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX     
** Created Date:                2018-06-07
****************************************************************/
uint8_t Tp_GetResultStatus(tp_key_info* tp_key)
{
    if(tp_param_s.tp_result_status == 1)
    {
        //触摸按键有结果产生
        if(tp_param_s.tp_result_s.total_mode == TP_MODE_SLIDE
           || tp_param_s.tp_result_s.total_mode == TP_MODE_MARGIN_SLIDE)
        {
            //滑动模式
            if(tp_param_s.tp_result_s.total_direct.xy_direct == TP_X_SLIDE_STATE)
            {
                //X方向滑动
                if(tp_param_s.tp_result_s.total_direct.x_direct == TP_LEFT_SLIDE_STATE)
                {
                    //YX.20180817.滑动增加判断区域,减少误触
                    if(tp_param_s.tp_result_s.start_coord.x > LCD_MAX_ROW/4*3
                       && tp_param_s.tp_result_s.now_coord.x < LCD_MAX_ROW/4)
                    {
                        //左滑
                        tp_key->key = LEFT_SLIDE_STATE;
                        //获取滑动的距离
                        tp_key->slid_dist = tp_param_s.tp_result_s.total_move_dist.x_dist;
                        return LEFT_SLIDE_STATE;
                    }
                    else
                    {
                        return NULL_STATE;
                    }
                }
                else if(tp_param_s.tp_result_s.total_direct.x_direct == TP_RIGHT_SLIDE_STATE)
                {
                    //YX.20180817.滑动增加判断区域,减少误触
                    if(tp_param_s.tp_result_s.start_coord.x < LCD_MAX_ROW/4
                       && tp_param_s.tp_result_s.now_coord.x > LCD_MAX_ROW/4*3)
                    {
                        //右滑
                        tp_key->key = RIGHT_SLIDE_STATE;
                        //获取滑动的距离
                        tp_key->slid_dist = tp_param_s.tp_result_s.total_move_dist.x_dist;
                        return RIGHT_SLIDE_STATE;
                    }
                    else
                    {
                        return NULL_STATE;
                    }
                }
            }
            else
            {
                //Y方向滑动
                if(tp_param_s.tp_result_s.total_direct.y_direct == TP_UP_SLIDE_STATE)
                {
                    //YX.20180817.滑动增加判断区域,减少误触
                    if(tp_param_s.tp_result_s.start_coord.y > LCD_MAX_COLUMN/4*3
                       && tp_param_s.tp_result_s.now_coord.y < LCD_MAX_COLUMN/4)
                    {
                        //上滑
                        tp_key->key = UP_SLIDE_STATE;
                        //获取滑动的距离
                        tp_key->slid_dist = tp_param_s.tp_result_s.total_move_dist.y_dist;
                        return UP_SLIDE_STATE;
                    }
                    else
                    {
                        return NULL_STATE;
                    }
                }
                else if(tp_param_s.tp_result_s.total_direct.y_direct == TP_DOWN_SLIDE_STATE)
                {
                    //YX.20180817.滑动增加判断区域,减少误触
                    if(tp_param_s.tp_result_s.start_coord.y < LCD_MAX_COLUMN/4
                       && tp_param_s.tp_result_s.now_coord.y > LCD_MAX_COLUMN/4*3)
                    {
                        //下滑
                        tp_key->key = DOWN_SLIDE_STATE;
                        //获取滑动的距离
                        tp_key->slid_dist = tp_param_s.tp_result_s.total_move_dist.y_dist;
                        return DOWN_SLIDE_STATE;
                    }
                    else
                    {
                        return NULL_STATE;
                    }
                }
            }
        }
        else if(tp_param_s.tp_result_s.total_mode == TP_MODE_CLICK)
        {
            //单击
            tp_key->key = CLICK_STATE;
//            //获取按键一瞬间的地址
//            tp_key->point_info_s.x = tp_param_s.tp_result_s.start_coord.x;
//            tp_key->point_info_s.y = tp_param_s.tp_result_s.start_coord.y;
            //获取离开一瞬间的地址
            tp_key->point_info_s.x = tp_param_s.tp_result_s.now_coord.x;
            tp_key->point_info_s.y = tp_param_s.tp_result_s.now_coord.y;
            return CLICK_STATE;
        }
    }
    
    //清除当次操作的数据
    Tp_ClearInfo();
    
    tp_key->key = NULL_STATE;
    return NULL_STATE;
}
    
/****************************************************************
** Function name:               Tp_OperationCal
** Descriptions:                tp数据计算函数
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX     
** Created Date:                2018-06-07
****************************************************************/
uint8_t Tp_OperationCal(tp_key_info* tp_key)
{
    uint8_t error_code = 0;
    uint8_t ret = NULL_STATE;
    tp_gesture_info_t *p_result_s = Tp_GetGesture();
//    for(uint8_t i = 0; i < 3; i++)
//    {
//        if(p_data_manage.tp_param_s.tp_data_status == 1)//正在使用数据
//        {
//            nrf_delay_ms(1);
//        }
//        else
//        {
//            break;
//        }
//    }
//    
//    p_data_manage.tp_param_s.tp_data_status = 1;
    
    if(tp_param_s.tp_init == 0)//还没有调用初始化
    {
        tp_param_s.tp_init = 1;
        //YX20180611.增加长按状态识别标志
        key_long_3s_flg = 1;
        key_long_6s_flg = 1;
        press_down_status = 0;
        Tp_GestureInit(80, 240, 240, 0, 0, 1000, 70);
        //80   数据产生的频率
        //240   X方向的宽度
        //240   Y方向的宽度
        //0     X方向的边缘百分比
        //0     Y方向的边缘百分比
        //1000  一次滑动的最长时间，超过这个时间则不被识别为滑动
        //70    滑动的最小距离，超过这个距离才会被识别为滑动效果,(由于边缘区域会有40个点左右跳动，会产生滑动的误差,对于长按影响比较)
    }
//    if(simul_data_cache_num > 6)
//    {
        error_code = Tp_GestureProcess(data_cache,simul_data_cache_num,0);
        if(error_code == 2)
        {
            //出现误操作的TP状态
            Tp_ClearInfo();
            return ret;
        }
        simul_data_cache_num = 0;
//    }
    
    
    tp_param_s.tp_result_s = *p_result_s;

    if(p_result_s->tp_state == 2)
    {    
        if(tp_param_s.tp_result_s.tp_state == 2)
        {
            //本次触摸完成
            tp_param_s.tp_result_status = 1;
            tp_param_s.tp_init = 0;
            //YX.20180607.暂时放这里处理，后期会根据页面支持情况进行修改，获取出不同的触摸结果
            ret = Tp_GetResultStatus(tp_key);
        }
    
    }
    else if(p_result_s->total_mode == TP_MODE_LONG_PRESS_3S && key_long_3s_flg == 1)
    {
        key_long_3s_flg = 0;
        //3s按键产生
        tp_key->key = LONG_PRESS_3S_STATE;
//        //获取按键一瞬间的地址
//        tp_key->point_info_s.x = tp_param_s.tp_result_s.start_coord.x;
//        tp_key->point_info_s.y = tp_param_s.tp_result_s.start_coord.y;
        //获取到达3s一瞬间的地址
        tp_key->point_info_s.x = tp_param_s.tp_result_s.now_coord.x;
        tp_key->point_info_s.y = tp_param_s.tp_result_s.now_coord.y;
    }
    else if(p_result_s->total_mode == TP_MODE_LONG_PRESS_6S && key_long_6s_flg == 1)
    {
        key_long_6s_flg = 0;
        //6s按键产生
        tp_key->key = LONG_PRESS_6S_STATE;
//        //获取按键一瞬间的地址
//        tp_key->point_info_s.x = tp_param_s.tp_result_s.start_coord.x;
//        tp_key->point_info_s.y = tp_param_s.tp_result_s.start_coord.y;
        //获取到达6s一瞬间的地址
        tp_key->point_info_s.x = tp_param_s.tp_result_s.now_coord.x;
        tp_key->point_info_s.y = tp_param_s.tp_result_s.now_coord.y;
    }
    
    if(p_result_s->tp_state == 1)
    {
        //按下状态
        press_down_status = PRESS_DOWN_Y_STATE;
    }
    else
    {
        //未按下状态
        press_down_status = PRESS_DOWN_N_STATE;
    }
    
    return ret;
}

/****************************************************************
** Function name:               Tp_WakeUp
** Descriptions:                Tp唤醒
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX
** Created Date:                2018-06-07
****************************************************************/
void Tp_WakeUp(void)
{
    Tp_ClearInfo();
    //WJ.20180816.唤醒后不在需要处理（中断唤醒）
    if(tp_now_sleep_status == 1)
    {
        return;
    }
    //YX.20180703.增加状态来互斥TP中断的通信和线程中调用功能的通信
    while(Tp_GetI2cStatus() == 1)
    {
        nrf_delay_ms(1);
    }
    
    Tp_SetI2cStatus(1);
    zinitix_resume();
    Tp_SetI2cStatus(0);
    tp_now_sleep_status = 1; 
}

/****************************************************************
** Function name:               Tp_Sleep
** Descriptions:                TP休眠
** input parameters:            sleep_mode：休眠的模式，模式不一样，唤醒的方式不一样
** output parameters:           无
** Returned value:              无
** Created by:                  YX
** Created Date:                2018-06-07
****************************************************************/
void Tp_Sleep(uint8_t sleep_mode)
{
    uint16_t mode_data = 0;
    switch(sleep_mode)
    {
        case SLEEP_MODE_1:
            mode_data = 0x1001;
            break;
        case SLEEP_MODE_2:
            mode_data = 0x2001;
            break;
        case SLEEP_MODE_3:
            mode_data = 0x4001;
            break;
        case SLEEP_MODE_4:
            mode_data = 0x8001;
            break;
        case SLEEP_MODE_5:
            mode_data = 0x0401;
            break;
        case SLEEP_MODE_6:
            mode_data = 0x0000;
            break;
        default:
            mode_data = 0x0000;
            break;
    }
    //YX.20180703.增加状态来互斥TP中断的通信和线程中调用功能的通信
    while(Tp_GetI2cStatus() == 1)
    {
        nrf_delay_ms(1);
    }
    Tp_SetI2cStatus(1);
    zinitix_suspend(mode_data);
    Tp_SetI2cStatus(0);
    
    tp_now_sleep_status = 0; 
}

/****************************************************************
** Function name:               Tp_ShutDown
** Descriptions:                TP关机
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX
** Created Date:                2018-06-07
****************************************************************/
void Tp_ShutDown(void)
{
    //YX.20180703.增加状态来互斥TP中断的通信和线程中调用功能的通信
    while(Tp_GetI2cStatus() == 1)
    {
        nrf_delay_ms(1);
    }
    Tp_SetI2cStatus(1);
    zinitix_shutdown();
    Tp_SetI2cStatus(0);
    
    Tp_ClearInfo();
    
    tp_now_sleep_status = 2;
}

/****************************************************************
** Function name:               Tp_Init
** Descriptions:                TP初始化
** input parameters:            mode: 1：上电初始化  2：唤醒失败重新初始化
** output parameters:           无
** Returned value:              无
** Created by:                  YX
** Created Date:                2018-06-07
****************************************************************/
void Tp_Init(uint8_t mode)
{
    memset(data_cache, 0, sizeof(data_cache));
    simul_data_cache_num = 0;
    zinitix_init();
    
    tp_now_sleep_status = 1;
}

/****************************************************************
** Function name:               Tp_GetKeyStatus
** Descriptions:                获取触摸按键的状态
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  YX
** Created Date:                2018-06-09
****************************************************************/
uint8_t Tp_GetKeyStatus(tp_key_info* tp_key)
{
    return Tp_OperationCal(tp_key);
}

/****************************************************************
** Function name:               Tp_SquareAreaDiscern
** Descriptions:                方形区域识别
** input parameters:            无
** output parameters:           无
** Returned value:              0:不在区域内  1:在区域内
** Created by:                  YX
** Created Date:                2018-06-09
****************************************************************/
//uint8_t Tp_SquareAreaDiscern(point_coord_t point, point_coord_t area_point, uint16_t wide, uint16_t high)
uint8_t Tp_SquareAreaDiscern(point_coord_t point, uint16_t area_point_x, uint16_t area_point_y, uint16_t wide, uint16_t high)
{
    uint8_t ret = 0;
    TP_COORD_DATA_T point_tmp ={0};
    area_square_info_t area_tmp = {0};
    
    point_tmp.x = point.x;
    point_tmp .y = point.y;
    area_tmp.point.x = area_point_x;
    area_tmp.point.y = area_point_y;
    area_tmp.wide = wide;
    area_tmp.high = high;
    
    ret = Tp_ComparePointInSquareArea(point_tmp, area_tmp);
    
    return ret;
}

/****************************************************************
** Function name:               Tp_CircleAreaDiscern
** Descriptions:                圆形区域识别
** input parameters:            无
** output parameters:           无
** Returned value:              0:不在区域内  1:在区域内
** Created by:                  YX
** Created Date:                2018-06-09
****************************************************************/
//uint8_t Tp_CircleAreaDiscern(point_coord_t point, point_coord_t area_point, uint16_t radius)
uint8_t Tp_CircleAreaDiscern(point_coord_t point, uint16_t area_point_x, uint16_t area_point_y, uint16_t radius)
{
    uint8_t ret = 0;
    
    TP_COORD_DATA_T point_tmp ={0};
    area_circle_info_t area_tmp = {0};
    
    point_tmp.x = point.x;
    point_tmp .y = point.y;
    area_tmp.point.x = area_point_x;
    area_tmp.point.y = area_point_y;
    area_tmp.radius = radius;
    
    ret = Tp_ComparePointInCircleArea(point_tmp, area_tmp);
    
    return ret;
}

