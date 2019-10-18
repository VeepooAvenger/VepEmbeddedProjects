/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     hal_acc_calculate.c
** Last modified Date:   2017-08-24
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-24
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#include <math.h>
#include "FreeRTOS.h"
#include "timers.h"
#include "app_type.h"
#include "gesture_test.h"
#include "app_timer.h"
#include "hal_acc_calculate.h"
#include "timer_manage.h"
#include "pedometer.h"
#include "gui_page_display.h"
#include "bracelet_protocol.h"

#include "nowear_detection.h"
#include "battery.h"
#include "gui_thread.h"
#include "hal_acc.h"

#if APP_HALACC_ENABLED

#define ACC_SAMPLE_INTERVAL             20          //20ms采样间隔
#define ACC_SAMPLE_TIME_CNT             (50*60)     //1分钟采样次数

#define ACC_SAMPLE_MAX_CNT              9
#define ACC_SAMPLE_BUF_MAX_CNT          40

static TimerHandle_t m_acc_interval_timer;

volatile uint16_t acc_cnt = 0;

int16_t pxData[50] = {0x00,};
int16_t pyData[50] = {0x00,};
int16_t pzData[50] = {0x00,};
int16_t pvData[50] = {0x00,};

int16_t  acc_senddate[54] = {0};
int16_t  acc_savedate[54] = {0};

int16_t  acc_data_buf[ACC_SAMPLE_BUF_MAX_CNT][3] = {0x00,};         //最大缓存

static int16_t  acc_data[3] = {0,};   // dxc
//static uint8_t  gesture_mode = 0;

static uint8_t acc_startflag = 0;               //加速度200ms定时器开启标志
static uint8_t acc_init_flag = 0;               //加速度初始化标记
volatile static uint8_t acc_time_flag = 0;      //200ms定时器进入标记，用于处理定时器异常停止的问题
volatile static uint8_t acc_lock_state = 0;        //标记是否进入200ms数据处理定时器，资源互斥标记
volatile static uint8_t acc_error_cnt = 0;      //记录连续没有读取到数据的次数，5次后重置加速度

static int16_t acc_sampling_data[2][10] = {0x00,};  //加速度采样数据发送缓存
static uint16_t acc_sampling_cnt = 0;               //缓存数据个数

static uint8_t gesture_oled_flag = 0;           //关屏标志
static uint8_t last_gesture_mode = 0;           //亮屏的触发模式
static uint8_t gesture_oled_frist = 0;          //是否已经触发亮屏

//static uint8_t acc_test_state = 0;

extern system_flag_t system_flag_s;
extern system_info_t system_info_s;

void Acc_TimerStop(void);
/****************************************************************
** Function name:           Acc_ArrayShift
** Descriptions:			Remove the earliest data, add new data, first in first out
** input parameters:        data:更新数据
** output parameters:       无
** Returned value:          无
** Created by:              WJ
** Created Date:            2016-05-19
**---------------------------------------------------------------
** Modified by:
** Modified date:
**---------------------------------------------------------------
****************************************************************/
void Acc_ArrayShift(int16_t *data)
{	
    uint8_t i;

    for (i = 0; i < 49; i++)
    {
        pxData[i]  = pxData[i+1];
        pyData[i]  = pyData[i+1];
        pzData[i]  = pzData[i+1];
        pvData[i]  = pvData[i+1];
    }
    pxData[49]  = data[0];
    pyData[49]  = data[1];
    pzData[49]  = data[2];   
    pvData[49]  = data[3];
}

/*******************************************************************************
** Function name:           Acc_GestureOledFlagGet
** Descriptions:            获取翻腕亮屏标志位
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-27
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
uint8_t Acc_GestureOledFlagGet(void)
{
    return gesture_oled_flag && gesture_oled_frist;
}

/*******************************************************************************
** Function name:           Acc_GestureOledFlagClean
** Descriptions:            翻腕亮屏标志位清除
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-27
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Acc_GestureOledFlagClean(void)
{
    gesture_oled_flag = 0;
    last_gesture_mode = 0;
}

/*******************************************************************************
** Function name:           Acc_GestureDeal
** Descriptions:            姿态处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2016-09-27
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void Acc_GestureDeal(void)
{
    uint8_t gesture_mode = 0;   
    uint8_t gesture_oled = 0; 
    uint8_t oled_enable = 0;
    uint16_t turnwrist_min_time = 0x00;
    uint16_t turnwrist_max_time = 0x00;
    uint16_t current_time = 0x00;

    current_time = clock->hour*60 + clock->minutes;
    turnwrist_min_time = system_info_s.turnwrist_config.s_hour*60 + system_info_s.turnwrist_config.s_minute;
    turnwrist_max_time = system_info_s.turnwrist_config.e_hour*60 + system_info_s.turnwrist_config.e_minute;

    gesture_mode = Gesture_GetMode16bit();
    gesture_oled = Gesture_GetCloseScreenFlag16bit();
 
    if(((gesture_mode == 5 || gesture_mode == 6) && system_info_s.turnwrist_config.enable) )
    {        
        if(turnwrist_min_time <= turnwrist_max_time)
        {
            if((turnwrist_min_time <= current_time)&&(current_time <= turnwrist_max_time))
            {
                oled_enable = 1;
            } 
        }
        else
        {
            if((current_time >= turnwrist_min_time)||(current_time <= turnwrist_max_time))
            {
                oled_enable = 1;
            }
        }
        if(oled_enable == 1)
        {
            /* 如果主界面如果不在待机界面 */
            if(gesture_oled_flag == 0 && Gui_ThreadPostureToBrightScreen())
            {                 
                gesture_oled_flag = 1;                 
                last_gesture_mode = gesture_mode;               
            }
        }
    }
    
    if(gesture_oled && gesture_oled_flag && last_gesture_mode != 7)    //关屏幕
    {
        Gui_ThreadPostureToCloseScreen();
        gesture_oled_flag = 0;        //wJ.20170322.关屏幕后在清除
    }
}

void Acc_DataConversion12Bit(int16_t *data, uint16_t len)
{
    uint16_t i = 0;
    
    for(i = 0; i < len; i++)
    {
        data[i] = (data[i] >> 4);
    }
}


/*******************************************************************************
** Function name:           Acc_Calculate
** Descriptions:            计步、姿态计算
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX            
** Created Date:            2016-11-18
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Acc_Calculate(void)
{
    uint8_t ret = 0;
    static int16_t acc_count = 0;
    static uint8_t gesture_cnt = 0; 
    int16_t data[4] = {0,};
    move_index_t mig_move_index = {0x00,}; 
    ped_param_set_t pp_param_s = {10, 2};
    
    ret = HalAcc_ReadData(acc_data);	//
    if(!ret)
    {
        acc_error_cnt++;
        return;
    }
    
    acc_error_cnt = 0;

    if(system_flag_s.adc_type == 1)
    {
        if(acc_sampling_cnt + 3 > ACC_SAMPLE_MAX_CNT)
        {
            acc_sampling_cnt = 0;
        }
        acc_sampling_data[0][acc_sampling_cnt++] = acc_data[0];
        acc_sampling_data[0][acc_sampling_cnt++] = acc_data[1];
        acc_sampling_data[0][acc_sampling_cnt++] = acc_data[2];         
        if(acc_sampling_cnt >= ACC_SAMPLE_MAX_CNT)
        {
            acc_sampling_cnt = 0;
            memcpy(acc_sampling_data[1], acc_sampling_data[0], ACC_SAMPLE_MAX_CNT*2);   
            acc_sampling_data[1][9] = acc_cnt;
            Ble_SendDataViaClk((void *)acc_sampling_data[1], 20);
        }
    }             
    
    //1009加速度不需要再次处理
    //Acc_DataConversion12Bit(acc_data, 3);    //转成12Bit
    
    data[0] = acc_data[0];
    data[1] = acc_data[1];
    data[2] = acc_data[2];
    data[3] = (int16_t)sqrt(((float)(data[0] *data[0])) 
              + ((float)(data[1] *data[1])) + ((float)(data[2] *data[2]))); 
    
    Acc_ArrayShift(data);
    if(++gesture_cnt == 5)
    {
        gesture_cnt = 0;
        Gesture_Detection16bit(pxData, pyData, pzData, pvData, system_info_s.gesture_level);//5);//3);
        Acc_GestureDeal(); //根据姿态进行处理
    } 
    data[3] = 0;
    Pedometer_Process(&data[0], acc_count++, 8, pp_param_s);
    if(acc_count >= 25)
    {
        acc_count = 0;
    }
            
    //清除长时间佩戴
    Move_IdexGet(&mig_move_index);
    if((Get_NoWearStation() >= 2 && Get_NoWearStation() <= 4) && (mig_move_index.move_5s >= 3))
    {
        Vnwd_NoWearInit();
    }
}

/*******************************************************************************
** Function name:           Acc_TimeroutHandler
** Descriptions:            定时器中断处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
static void Acc_TimeroutHandler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);
    
    if(acc_lock_state == 1) return;
    acc_lock_state = 1;
    
    acc_time_flag = 1;
    
    if(nrf_gpio_pin_read(HAL_ACC_INT) && acc_cnt < ACC_SAMPLE_TIME_CNT - 120)  
	{    
        acc_cnt = ACC_SAMPLE_TIME_CNT;		//1分钟

        HalAcc_CleanInterrupt();
    } 
    
    if(!system_flag_s.sign_off && system_flag_s.power_state == POWER_NORMAL)  //关机,充电后不做数据处理
    {    
        Acc_Calculate(); 
    }   
    if(acc_cnt--)
    {
        if(acc_cnt == 0 || acc_cnt >= ACC_SAMPLE_TIME_CNT)
        {
            acc_cnt = 0;
            Acc_TimerStop();       
            
            //WJ.20180814.
            HalAcc_CleanInterrupt();
            HalAcc_PinInterruptInit();
        }
    }
    acc_lock_state = 0;
}

/****************************************************************
** Function name:           Acc_TimerCreate
** Descriptions:            定时器创建
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Acc_TimerCreate(void)
{   
    m_acc_interval_timer = xTimerCreate("ACC", ACC_SAMPLE_INTERVAL, pdTRUE, NULL, Acc_TimeroutHandler);	
}

/****************************************************************
** Function name:           Acc_TimerStart
** Descriptions:            定时器开启
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Acc_TimerStart(void)
{
    if(acc_startflag == 1)
    {
        return;
        
    }
    if (pdPASS != xTimerStart(m_acc_interval_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
       // APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
        
    acc_startflag = 1;
    acc_cnt = ACC_SAMPLE_TIME_CNT;
}

/****************************************************************
** Function name:           Acc_TimerStop
** Descriptions:            停止定时器
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Acc_TimerStop(void)
{
    xTimerStop(m_acc_interval_timer, 0);    
    
    acc_startflag = 0;
}

/*******************************************************************************
** Function name:           Acc_StartHandler
** Descriptions:            加速度事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WX             
** Created Date:            2016-11-17
**------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------
*******************************************************************************/
void Acc_StartHandler(void)
{   
//    static uint8_t last_acc_same_cnt = 0;

//    //定时器开启，并持续采样
//    if(acc_startflag == 1)
//	{


//	}
//	else
//	{
//        if(acc_time_flag == 0 && acc_startflag == 1)
//        {
//            if((++last_acc_same_cnt) >= 5)     //定时器异常停止工作，清除状态
//            {
//                Acc_TimerStop(); 
//                //acc初始化
//                HalAcc_Configuration();  
//                acc_startflag = 0;
//                acc_cnt = 0; 
//                last_acc_same_cnt = 0;          
//                acc_error_cnt = 0;
//            }
//        } 
//        acc_time_flag = 0;
//	}
}

/****************************************************************
** Function name:           Acc_Stop
** Descriptions:            加速度停止工作，休眠
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Acc_Stop(void)
{    
    //WJ.20180814.
    //if(acc_lock_state == 1) return;        
    while(acc_lock_state == 1);
    
    acc_lock_state = 1;
    
    if(acc_startflag == 1)
    {
        Acc_TimerStop();
    }            
    
    acc_cnt = 0;

    HalAcc_Sleep();
    HalAcc_PinHanderUnInit();
    acc_init_flag = 0;
    
    acc_lock_state = 0;    
}

/****************************************************************
** Function name:           Acc_Start
** Descriptions:            加速度配置初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Acc_Start(void)
{   
    if(acc_init_flag == 0)
    {
        HalAcc_Configuration();    
        acc_cnt = 0;    
        acc_startflag = 0;  
        acc_init_flag = 1; 
        
        //WJ.20180814
        HalAcc_PinInterruptInit();     
    }        
}

/****************************************************************
** Function name:           Acc_Init
** Descriptions:            加速度初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Acc_Init(void)
{ 
    if(acc_lock_state == 1) return;   
    
    acc_lock_state = 1;
    Acc_TimerStop();

    HalAcc_Init();
    acc_init_flag = 1;
    
    acc_lock_state = 0;
}

/****************************************************************
** Function name:           Acc_GetData
** Descriptions:            获取xyz轴数据
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Acc_GetData(uint16_t *data)
{
    data[0]  = (acc_data[0]);// & 0xFF;
    data[1]  = (acc_data[1]);// & 0xFF;
    data[2]  = (acc_data[2]);// & 0xFF;
    
//    HalAcc_GetData(data); 
}

/****************************************************************
** Function name:           Acc_Is_Move
** Descriptions:            加速度唤醒中
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Acc_Is_Move(void)
{
    if(acc_cnt >= ACC_SAMPLE_TIME_CNT - 120)//12)
    {
        return 1;
    }        
    return 0;
}

/****************************************************************
** Function name:           Acc_Is_Init
** Descriptions:            加速度已初始化
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Acc_Is_Init(void)
{
    if( acc_init_flag == 1)
    {
        return 1;
    }        
    return 0;    
}

/****************************************************************
** Function name:           Acc_SendConfig
** Descriptions:            发送加速度配置
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Acc_SendConfig(void)
{
//    uint8_t ble_ack[20] = {0xFF, 0x00, };
//    
//    if(acc_lock_state == 1) return;    
//    acc_lock_state = 1;
//    
//    Gsensor_ReadConfig(&ble_ack[1]);
//    Ble_SendDataViaBat(ble_ack, 20);
//    
//    acc_lock_state = 0;
}

/****************************************************************
** Function name:           Acc_RestConfig
** Descriptions:            重置加速度
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-28
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Acc_RestConfig(void)
{
    Acc_Init();
}

#endif  //APP_HALACC_ENABLED

