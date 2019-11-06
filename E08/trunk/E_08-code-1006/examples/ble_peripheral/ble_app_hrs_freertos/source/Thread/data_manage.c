/*************************************Copyright(c)***************************************************
**                                     Veepoo Tech
**                  
**                               COPYRIGHT(C)  2016-2019
**--------------------------------------FileInfo----------------------------------------------------
** FileName:                DataManage.c
** Last Modify Data:        2017-08-04
** Last SYS_VERSION:        XXXXXXXXX
** Descriptions:                     
**---------------------------------------------------------------------------------------------------
** CreatedBy:               GT
** CreateData:              2017-06-xx
** SystemVersion:             
** Description:             the  Data Manage  function 
**                          
**                          
**---------------------------------------------------------------------------------------------------
** Modify By:                       
** Modify Data:
** System Version:
** Modify Description:    

******************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SystemTime.h"
#include "data_manage.h"
#include "cpu_task_data_type.h"
#include "pedometer.h"
#include "api_flash.h"
#include "util.h"
#include "vph_solo.h"
#include "si1182_driver.h"


static data_manage_param  st_data_manage;

data_manage_param*  p_data_manage = &st_data_manage;


// 解析si1182的ADC数据的缓存
static uint8_t  si1182_adc_cache[SI1182_FIFO_BUFFER_SIZE + SI1182_FIFO_BUFFER_SIZE];
// 解析si1182的LD数据的缓存
static int8_t  si1182_ld_cache[SI1182_LD_BUFFER_SIZE + SI1182_LD_BUFFER_SIZE];
// si1182的ADC数据的缓存的上次剩余字节
static uint16_t si1182_adc_last_data_num = 0;
// si1182的adc的一次中断的数据记录
static uint16_t si1182_adc_data_num = 0;
// si1182的LD数据的缓存的上次剩余字节
static uint8_t si1182_ld_last_data_num = 0;
// si1182的adc的一次中断的数据记录
static uint8_t si1182_ld_data_num = 0;
// si1182的adc的数据记录
static uint8_t si1182_ld_data_allnum = 0;


/************************************************************************
** Function name:        DataManage_Init
** Descriptions:         data manage  init
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void DataManage_Init(void)
{
    uint8_t  local_day_id = (3 + p_flash_param->flash_index_s.block_index)%3;
    memset(&st_data_manage,0,sizeof(data_manage_param));
    
    p_data_manage->device_trace_param_s.decryption_ok = VPH_SoloShow()&0x01;
    
    // 计算手环端开机记步值
    st_data_manage.pedometer.power_on_step = Api_GetStepAllCount(0);
    st_data_manage.pedometer.step_history_count = p_flash_param->flash_index_s.daily.step_cnt[local_day_id];
    st_data_manage.pedometer.distance_history_count = p_flash_param->flash_index_s.daily.distance_cnt[local_day_id];
    st_data_manage.pedometer.calories_history_count = p_flash_param->flash_index_s.daily.calories_cnt[local_day_id]*100;
    
    // 脱手检测算法初始化
    st_data_manage.no_wear_measure_s.init = 1;
	
    // 开机标志置位
    st_data_manage.device_trace_param_s.sign_on = 1;
    
    st_data_manage.upload = UPLOAD_ADC_NULL_EVENT;
    st_data_manage.data_pool.ppg_send_read_pos = 0;	
    st_data_manage.data_pool.ppg_send_wait_len = 0;
	st_data_manage.data_pool.ecg_send_read_pos = 0;
    st_data_manage.data_pool.ecg_send_wait_len = 0;
    
    Ped_SetSportsInfo(p_flash_param->flash_index_s.daily.step_cnt[local_day_id], p_flash_param->flash_index_s.daily.calories_cnt[local_day_id]*100,
                      p_flash_param->flash_index_s.daily.distance_cnt[local_day_id]);
    Ped_SetInputPersonalInformation(p_flash_param->person_s.height, p_flash_param->person_s.weight,
                                    p_flash_param->person_s.gender, p_flash_param->person_s.age);
                                                                    
   //复位信息统计
   st_data_manage.power_statistics_s.rest_cnt = p_flash_param->flash_index_s.daily.rest_cnt[local_day_id] + 1;
}

/************************************************************************
** Function name:        DataManage_UpdateFIFOData
** Descriptions:         update FIFO Data
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void DataManage_UpdateFIFOData(int16_t* p_fifo,uint8_t len)
{
    uint8_t part_len = 0;
    uint8_t data_pos = st_data_manage.acc.data_w_pos%100;
    
    // 拷贝原始数据
    if(data_pos + len >= 100)
    {
        // 分两段数据缓存拷贝
        part_len = 100 - data_pos;
        for(uint8_t index = 0; index < part_len; index++)
        {
            st_data_manage.acc.x_axis_data[data_pos+index] = p_fifo[index*3];
            st_data_manage.acc.y_axis_data[data_pos+index] = p_fifo[index*3+1];
            st_data_manage.acc.z_axis_data[data_pos+index] = p_fifo[index*3+2];
        }
        
        for(uint8_t index = 0; index < len-part_len; index++)
        {
            st_data_manage.acc.x_axis_data[index] = p_fifo[(part_len+index)*3];
            st_data_manage.acc.y_axis_data[index] = p_fifo[(part_len+index)*3+1];
            st_data_manage.acc.z_axis_data[index] = p_fifo[(part_len+index)*3+2];
        }
        
        //更新数据写位置信息
        st_data_manage.acc.data_w_pos = len-part_len;
        st_data_manage.acc.data_w_pos = len-part_len;
    }
    else
    {
        //直接拷贝三轴数据
        for(uint8_t index = 0; index < len; index++)
        {
            st_data_manage.acc.x_axis_data[data_pos+index] = p_fifo[index*3];
            st_data_manage.acc.y_axis_data[data_pos+index] = p_fifo[index*3+1];
            st_data_manage.acc.z_axis_data[data_pos+index] = p_fifo[index*3+2];
        }
        
        //更新数据写位置信息
        st_data_manage.acc.data_w_pos += len;
    }
    
    // 更新其他位置信息
    st_data_manage.acc.adc_send_len += len;
    st_data_manage.acc.gesture_wait_send_len += len;
                
    st_data_manage.acc.gesture_data_len += len;
    st_data_manage.acc.pedometer_data_len += len;
}

/************************************************************************
** Function name:        DataManage_ProcessGestureData
** Descriptions:         process Gesture data
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void DataManage_ProcessGestureData(void)
{
    int16_t gesture_data[4][5] = {0};
    uint8_t gesture_read_pos = st_data_manage.acc.gesture_read_pos;
    
    // 这里需要做异常处理判断
    if(st_data_manage.acc.gesture_data_len < 5)
    {					
        return;
    }
				
    for(uint8_t index = 0; index < 5; index++)
    {
        gesture_data[0][index] = st_data_manage.acc.x_axis_data[(gesture_read_pos+index)%100]>>4;
        gesture_data[1][index] = st_data_manage.acc.y_axis_data[(gesture_read_pos+index)%100]>>4;
        gesture_data[2][index] = st_data_manage.acc.z_axis_data[(gesture_read_pos+index)%100]>>4;
        gesture_data[3][index] = ((int16_t)sqrt(((float)(gesture_data[0][index] *gesture_data[0][index])) +
        ((float)(gesture_data[1][index] *gesture_data[1][index])) + ((float)(gesture_data[2][index] *gesture_data[2][index]))));//>>4; 
    }
    
    // 姿态处理更新处理
    memcpy(&st_data_manage.acc.x_gesture_data[0],&st_data_manage.acc.x_gesture_data[5],45*sizeof(int16_t));
    memcpy(&st_data_manage.acc.x_gesture_data[45],&gesture_data[0][0],5*sizeof(int16_t));
    
    memcpy(&st_data_manage.acc.y_gesture_data[0],&st_data_manage.acc.y_gesture_data[5],45*sizeof(int16_t));
    memcpy(&st_data_manage.acc.y_gesture_data[45],&gesture_data[1][0],5*sizeof(int16_t));
    
    memcpy(&st_data_manage.acc.z_gesture_data[0],&st_data_manage.acc.z_gesture_data[5],45*sizeof(int16_t));
    memcpy(&st_data_manage.acc.z_gesture_data[45],&gesture_data[2][0],5*sizeof(int16_t));
    
    memcpy(&st_data_manage.acc.v_gesture_data[0],&st_data_manage.acc.v_gesture_data[5],45*sizeof(int16_t));
    memcpy(&st_data_manage.acc.v_gesture_data[45],&gesture_data[3][0],5*sizeof(int16_t));
    
    // 更新数据位置
    st_data_manage.acc.gesture_read_pos = (st_data_manage.acc.gesture_read_pos+5)%100;
    st_data_manage.acc.gesture_data_len -= 5;
}
/************************************************************************
** Function name:        DataManage_ProcessPedometerData
** Descriptions:         process pedometer data
** input parameter:      *p_xdata :the x-coordinate data    * p_ydata:the Y-coordinate data        
**                       *p_zdata:the Z-coordinate data 
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void DataManage_ProcessPedometerData(int16_t* p_xdata,int16_t* p_ydata,int16_t* p_zdata)
{
    uint16_t pos = st_data_manage.acc.pedometer_read_pos%100;
    
    // 异常处理
    if(st_data_manage.acc.pedometer_data_len == 0) 
    {
        return;
    }
				
    *p_xdata = st_data_manage.acc.x_axis_data[pos]>>4;
    *p_ydata = st_data_manage.acc.y_axis_data[pos]>>4;
    *p_zdata = st_data_manage.acc.z_axis_data[pos]>>4;
    
    st_data_manage.acc.pedometer_read_pos = (st_data_manage.acc.pedometer_read_pos+1)%100;
    st_data_manage.acc.pedometer_data_len--;
}
///************************************************************************
//** Function name:        DataManage_GetRTAccData
//** Descriptions:         process pedometer data
//** input parameter:      *p_data :the RTacc data
//**                      
//** Returned Value:       void
//** Created  by:          GT
//** Created  Datas:       2017-08-04
//**-----------------------------------------------------------------------
//** Modify By:
//** Modify data:
//** Modify Description:
//**************************************************************************/
//void DataManage_GetRTAccData(int16_t* p_data)
//{
//    uint8_t pos = (st_data_manage.acc.data_w_pos+99)%100;
//    p_data[0] = st_data_manage.acc.x_axis_data[pos];
//    p_data[1] = st_data_manage.acc.y_axis_data[pos];
//    p_data[2] = st_data_manage.acc.z_axis_data[pos];
//}
/************************************************************************
** Function name:        DataManage_GetGsensorADCData
** Descriptions:         get Gsensor adc data
** input parameter:      *p_data:to  store  the   Gsensor data
**                      
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void DataManage_GetGsensorADCData(int16_t*  p_data)
{
    uint8_t pos = st_data_manage.acc.adc_send_pos;
    
    for(uint8_t index = 0; index < 3; index++)
    {
        p_data[index*3]   = st_data_manage.acc.x_axis_data[(pos+index)%100];
        p_data[index*3+1] = st_data_manage.acc.y_axis_data[(pos+index)%100];
        p_data[index*3+2] = st_data_manage.acc.z_axis_data[(pos+index)%100];
    }
    
    // 更新ADC原始数据的位置和长度信息
    st_data_manage.acc.adc_send_pos = (st_data_manage.acc.adc_send_pos+3)%100;
    st_data_manage.acc.adc_send_len -= 3;
}

/************************************************************************
** Function name:        DataManage_GetGestureData
** Descriptions:         to get gesture data
** input parameter:      *p_xdata :to store  the gesture  data
**                      
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void DataManage_GetGestureData(int16_t* p_data)
{
    uint8_t pos = st_data_manage.acc.gesture_send_pos;
    
    for(uint8_t index = 0; index < 3; index++)
    {
        p_data[index*3]   = st_data_manage.acc.x_gesture_data[(pos+index)%100];
        p_data[index*3+1] = st_data_manage.acc.y_gesture_data[(pos+index)%100];
        p_data[index*3+2] = st_data_manage.acc.z_gesture_data[(pos+index)%100];
    }
    
    // 更新ADC原始数据的位置和长度信息
    st_data_manage.acc.gesture_send_pos = (st_data_manage.acc.gesture_send_pos+3)%100;
    st_data_manage.acc.gesture_wait_send_len -= 3;
}

/************************************************************************
** Function name:        DataManage_AccData_Send_Reset
** Descriptions:         acc data send reset 
** input parameter:      void 
**                       
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void DataManage_AccData_Send_Reset(void)
{
    st_data_manage.acc.adc_send_pos = st_data_manage.acc.data_w_pos;
}
/************************************************************************
** Function name:        DM_Update_DailyHeartData
** Descriptions:         updata daily heart data
** input parameter:      heart:the data  to updata
**                      
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void DM_Update_DailyHeartData(uint8_t heart)
{
//    st_data_manage.daily_attr.heart = heart;
}
/************************************************************************
** Function name:        DataManage_NewestDailyData
** Descriptions:         get the newest daily data 
** input parameter:      void 
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
daily_data_attr_t* DataManage_NewestDailyData(void)
{
    st_data_manage.daily_attr.month  = clock->month;
    st_data_manage.daily_attr.day    = clock->day;
    st_data_manage.daily_attr.hour   = clock->hour;
    st_data_manage.daily_attr.minute = clock->minutes;
    
    // 更新记步值
    st_data_manage.daily_attr.step = Array_ShortSum(p_data_manage->pedometer.step_count, 5);
    st_data_manage.daily_attr.sport = Array_ShortSum(p_data_manage->pedometer.sport_count, 5);
    
    // 更新距离、卡路里值
    st_data_manage.daily_attr.distance = Array_ShortSum(p_data_manage->pedometer.distance_count, 5);
    st_data_manage.daily_attr.calories = Array_ShortSum(p_data_manage->pedometer.calories_count, 5);
	
	
	//重置错误代码
    st_data_manage.daily_attr.resv[0] = 0;
    st_data_manage.daily_attr.resv[1] = 0;	
    
    return &st_data_manage.daily_attr;
}

/************************************************************************
** Function name:        DataManage_InitDailyData
** Descriptions:         重置五分钟数据 
** input parameter:      无
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-10-22
**************************************************************************/
void DataManage_InitDailyData(void)
{
    memset(&st_data_manage.daily_attr, 0x00, sizeof(daily_data_attr_t));   
}

/************************************************************************
** Function name:        DataManage_GetCurrentStepCnt
** Descriptions:         get  local step cnt
** input parameter:      void  
**                      
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
uint32_t DataManage_GetCurrentStepCnt(void)
{
//    return st_data_manage.pedometer.power_on_step + All_StepGet();
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s); 
    return gsii_sport_info_s.posi_steps;//All_StepGet();
}

/************************************************************************
** Function name:        DM_Update_Brightness
** Descriptions:         updata brightness
** input parameter:      brightness :the  value to updata
**                       
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**-----------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
**************************************************************************/
void DM_Update_Brightness(uint8_t brightness)
{
    st_data_manage.oled_bright = brightness;
}

/*************************************************************************
** Function name:        DataManage_SI1182PoolInit
** Descriptions:         AFE4900数据缓存初始化
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void DataManage_SI1182PoolInit(void)
{
    st_data_manage.data_pool.pool_update_second = 0;
    st_data_manage.data_pool.ppg_cache_update_cnt = 0;
	st_data_manage.data_pool.ecg_cache_update_cnt = 0;
	
    memset(&st_data_manage.data_pool.ecg_data[0], 0, sizeof(st_data_manage.data_pool.ecg_data));
	
    // 重新初始化发送到信息
    st_data_manage.data_pool.ppg_send_read_pos = 0;	
    st_data_manage.data_pool.ppg_send_wait_len = 0;
    st_data_manage.data_pool.ppg_send_package_index = 0;
	
	// 重新初始化发送到信息
    st_data_manage.data_pool.ecg_send_read_pos = 0;	
    st_data_manage.data_pool.ecg_send_wait_len = 0;
    st_data_manage.data_pool.ecg_send_package_index = 0;
}


/*************************************************************************
** Function name:        DataManage_GetEcgDataPool
** Descriptions:         获取ECG缓存数据
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-12-25
**************************************************************************/
const int32_t* DataManage_GetEcgDataPool(void)
{
    return &st_data_manage.data_pool.ecg_data[0];
}

// 最新更新1s的数据起始地址
int32_t* DataManage_GetEcgDataPoolLd(void)
{
    return &st_data_manage.data_pool.ecg_data[ECG_POOL_SIZE - ECG_CACHE_SIZE];
}


/*************************************************************************
** Function name:        DataManage_GetGreenDataPool
** Descriptions:         获取绿光缓存数据
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-12-25
**************************************************************************/
const int32_t* DataManage_GetGreenDataPool(void)
{
    return &st_data_manage.data_pool.ppg_data.green[0];
}

/*************************************************************************
** Function name:        DataManage_GetRedDataPool
** Descriptions:         获取红光缓存数据
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-12-25
**************************************************************************/
const int32_t* DataManage_GetRedDataPool(void)
{
    return &st_data_manage.data_pool.ppg_data.red[0];
}

/*************************************************************************
** Function name:        DataManage_GetIredDataPool
** Descriptions:         获取红外缓存数据
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-12-25
**************************************************************************/
const int32_t* DataManage_GetIredDataPool(void)
{
    return &st_data_manage.data_pool.ppg_data.ired[0];
}

/*************************************************************************
** Function name:        DataManage_GetAmbDataPool
** Descriptions:         获取环境光缓存数据
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-12-25
**************************************************************************/
const int32_t* DataManage_GetAmbDataPool(void)
{
    // 暂时使用红外光替代环境光做佩戴
    //return &st_data_manage.data_pool.ppg_data.amb[0];    
    return &st_data_manage.data_pool.ppg_data.ired[IRED_POOL_SIZE - 2 * IRED_CACHE_SIZE];
}

/*************************************************************************
** Function name:        DataManage_GetWearDataGreenPool
** Descriptions:         获取佩戴数据-绿光
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-12-25
**************************************************************************/
const int32_t* DataManage_GetWearDataGreenPool(void)
{
    return &st_data_manage.data_pool.ppg_data.green[GREEN_POOL_SIZE - (2 * GREEN_CACHE_SIZE)];
}

/*************************************************************************
** Function name:        DataManage_GetWearDataIredPool
** Descriptions:         获取佩戴数据-红外
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-12-25
**************************************************************************/
const int32_t* DataManage_GetWearDataIredPool(void)
{
    return &st_data_manage.data_pool.ppg_data.ired[IRED_POOL_SIZE - (2 * IRED_CACHE_SIZE)];
}

/*************************************************************************
** Function name:        DataManage_PoolSecondUpdate
** Descriptions:         AFE4900缓存池秒计数更新
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void DataManage_PoolSecondUpdate(void)
{
    // AFE4900 缓存池时间计数加1
    st_data_manage.data_pool.pool_update_second++;  
}

/*************************************************************************
** Function name:        DataManage_CopyPpgCacheData
** Descriptions:         复制PPG的cache数据到PPG缓存数据
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void DataManage_CopyPpgCacheData(void)
{
    st_data_manage.data_pool.ppg_cache_update_cnt = 0;
    
    if(st_data_manage.data_pool.ppg_send_read_pos >= PPG_CACHE_SIZE)
    {
        st_data_manage.data_pool.ppg_send_read_pos -= PPG_CACHE_SIZE;
    }
    
//    // 移动最新1秒数据
//    memcpy(&st_data_manage.data_pool.ppg_data[0], &st_data_manage.data_pool.ppg_data[PPG_CACHE_SIZE], 
//		   PPG_POOL_SIZE*sizeof(int32_t));   
//    
//    //WJ.20181022.同步更新环境光缓存
//    memcpy(&st_data_manage.data_pool.amb_data[0], &st_data_manage.data_pool.amb_data[AMB_CACHE_SIZE], 
//		   AMB_POOL_SIZE*sizeof(int32_t)); 
}

/*************************************************************************
** Function name:        DataManage_CopyEcgCacheData
** Descriptions:         复制ECG的cache数据到ECG缓存数据
** input parameter:      无   
** Returned Value:       无
** Created  by:          JSH
** Created  Datas:       2018-10-17
**************************************************************************/
void DataManage_CopyEcgCacheData(void)
{
    st_data_manage.data_pool.ecg_cache_update_cnt = 0;
    
    if(st_data_manage.data_pool.ecg_send_read_pos >= ECG_CACHE_SIZE)
    {
        st_data_manage.data_pool.ecg_send_read_pos -= ECG_CACHE_SIZE;
    }
    
    // 移动最新1秒数据
    memcpy(&st_data_manage.data_pool.ecg_data[0], &st_data_manage.data_pool.ecg_data[ECG_CACHE_SIZE], 
	        ECG_POOL_SIZE*sizeof(int32_t));
}

/*************************************************************************
** Function name:        Combine_PackageData
** Descriptions:         转换adc 数据用于发送到app
** input parameter:      无   
** Returned Value:       无
** Created  by:          WJ
** Created  Datas:       2018-10-25
**************************************************************************/
static void Combine_PackageData(uint8_t *p_data, int32_t *p_adc, uint8_t package)
{
    uint32_t sum = 0;
    
    for(uint8_t index = 0; index < 5; index++)
    {
        p_data[index*3]   = (p_adc[index]>>16)&0xFF;
        p_data[index*3+1] = (p_adc[index]>>8)&0xFF;
        p_data[index*3+2] =  p_adc[index]&0xFF;
        //WJ.20181029.修改校验方式
        sum += p_data[index*3+2] + (p_data[index*3+1]<<8) + (p_data[index*3]<<16);
        //sum += p_adc[index];
    }
    
    p_data[15] = (sum>>24)&0xFF;
    p_data[16] = (sum>>16)&0xFF;
    p_data[17] = (sum>>8)&0xFF;
    
//    p_data[15] = (sum>>16)&0xFF;
//    p_data[16] = (sum>>8)&0xFF;
//    p_data[17] = sum&0xFF;
    p_data[18] = package;
}	

/*************************************************************************
** Function name:        DataManage_InitPoolData
** Descriptions:         初始化adc数据缓存
** input parameter:      无   
** Returned Value:       
** Created  by:          JSH
** Created  Datas:       2018-12-21
**************************************************************************/
void DataManage_InitPoolData(void)
{
	memset(&st_data_manage.data_pool, 0, sizeof(&st_data_manage.data_pool));
	
	si1182_adc_last_data_num = 0;
	si1182_adc_data_num = 0;
	si1182_ld_last_data_num = 0;
	si1182_ld_data_num = 0;
}

/*************************************************************************
** Function name:        DataManage_GetEcgAdcData
** Descriptions:         获取ecg数据
** input parameter:      无   
** Returned Value:       
** Created  by:          JSH
** Created  Datas:       2018-12-21
**************************************************************************/
extern int32_t  ecg_filter_data[250];
extern uint8_t  adc_ecg_signal_state;
extern uint8_t  adc_ecg_signal_state1;
extern uint8_t  adc_ecg_signal_state2;
void DataManage_GetEcgAdcData(uint8_t *ecg_data, uint8_t adc_type)
{	
    uint32_t pos = st_data_manage.data_pool.ecg_send_read_pos % ECG_CACHE_SIZE;
   
    if(adc_type == 0)
    {   //原始信号   
        Combine_PackageData(ecg_data, &st_data_manage.data_pool.ecg_data[ECG_POOL_SIZE - ECG_CACHE_SIZE + pos],
                            st_data_manage.data_pool.ecg_send_package_index);
    }
    else
    {
        //滤波信号
        Combine_PackageData(ecg_data, &ecg_filter_data[pos],
                            st_data_manage.data_pool.ecg_send_package_index);
    }
    
    ecg_data[15] = adc_ecg_signal_state | (adc_ecg_signal_state2<<7);
    ecg_data[16] = 8;
    ecg_data[17] = adc_ecg_signal_state1;
    
    // 更新位置信息和长度信息
    if(st_data_manage.data_pool.ecg_send_package_index++ >= 255)
    {
        st_data_manage.data_pool.ecg_send_package_index = 0;
    }
    
    st_data_manage.data_pool.ecg_send_wait_len -= 5;
    st_data_manage.data_pool.ecg_send_read_pos += 5;  	
}

/*************************************************************************
** Function name:        DataManage_GetEcgLdData
** Descriptions:         获取ecg的导联数据
** input parameter:      无   
** Returned Value:       
** Created  by:          JSH
** Created  Datas:       2018-12-28
**************************************************************************/
void DataManage_GetEcgLdData1(uint8_t *ld_data)
{	
	memcpy(ld_data, &si1182_ld_cache[0], 15);
	ld_data[18] = 0x01;
}

/*************************************************************************
** Function name:        DataManage_GetEcgLdData
** Descriptions:         获取ecg的导联数据
** input parameter:      无   
** Returned Value:       
** Created  by:          JSH
** Created  Datas:       2018-12-28
**************************************************************************/
void DataManage_GetEcgLdData2(uint8_t *ld_data)
{	
	uint16_t *p_data = (uint16_t *)&ld_data[5];
	
    memcpy(ld_data, &si1182_ld_cache[15], 5);
	
	for(uint32_t i = 0; i < 5; i++)
	{
		*p_data = st_data_manage.data_pool.ld_data[i];
		p_data++;
	}
	ld_data[18] = 0x02;
}

/*************************************************************************
** Function name:        DataManage_GetPpgAdcDataGreenAmb
** Descriptions:         获取绿光、环境光的数据
** input parameter:      无   
** Returned Value:       
** Created  by:          JSH
** Created  Datas:       2018-12-21
**************************************************************************/
void DataManage_GetPpgAdcDataGreenAmb(uint8_t *ppg_green, uint8_t *ppg_amb)
{
	uint32_t pos = st_data_manage.data_pool.ppg_send_read_pos % PPG_CACHE_SIZE;
    
	// 绿光
    Combine_PackageData(ppg_green, &st_data_manage.data_pool.ppg_data.green[GREEN_POOL_SIZE - GREEN_CACHE_SIZE + pos],
                        st_data_manage.data_pool.ppg_send_package_index);
	// 环境光
	//Combine_PackageData(ppg_amb, &st_data_manage.data_pool.ppg_data.amb[AMB_POOL_SIZE - AMB_CACHE_SIZE + pos],
    //                    st_data_manage.data_pool.ppg_send_package_index);
    
    // 更新位置信息和长度信息
    if(st_data_manage.data_pool.ppg_send_package_index++ >= 255)
    {
        st_data_manage.data_pool.ppg_send_package_index = 0;
    }
    
    st_data_manage.data_pool.ppg_send_wait_len -= 5;
    st_data_manage.data_pool.ppg_send_read_pos += 5;
}

/*************************************************************************
** Function name:        DataManage_GetPpgAdcDataGreenIredAmb
** Descriptions:         获取绿光、红外、环境光的数据
** input parameter:      无   
** Returned Value:       
** Created  by:          JSH
** Created  Datas:       2018-12-21
**************************************************************************/
void DataManage_GetPpgAdcDataGreenIredAmb(uint8_t *ppg_green, uint8_t *ppg_ired, uint8_t *ppg_amb)
{
	uint32_t pos = st_data_manage.data_pool.ppg_send_read_pos % PPG_CACHE_SIZE;
    
	// 绿光
    Combine_PackageData(ppg_green, &st_data_manage.data_pool.ppg_data.green[GREEN_POOL_SIZE - GREEN_CACHE_SIZE + pos],
                        st_data_manage.data_pool.ppg_send_package_index);
	// 红外
    Combine_PackageData(ppg_ired, &st_data_manage.data_pool.ppg_data.ired[IRED_POOL_SIZE - IRED_CACHE_SIZE + pos],
                        st_data_manage.data_pool.ppg_send_package_index);
	// 环境光
	//Combine_PackageData(ppg_amb, &st_data_manage.data_pool.ppg_data.amb[AMB_POOL_SIZE - AMB_CACHE_SIZE + pos],
    //                    st_data_manage.data_pool.ppg_send_package_index);
    
    // 更新位置信息和长度信息
    if(st_data_manage.data_pool.ppg_send_package_index++ >= 255)
    {
        st_data_manage.data_pool.ppg_send_package_index = 0;
    }
    
    st_data_manage.data_pool.ppg_send_wait_len -= 5;
    st_data_manage.data_pool.ppg_send_read_pos += 5;
}

/*************************************************************************
** Function name:        DataManage_GetPpgAdcDataGreenRedIredAmb
** Descriptions:         获取绿光、红光、红外、环境光的数据
** input parameter:      无   
** Returned Value:       
** Created  by:          JSH
** Created  Datas:       2018-12-21
**************************************************************************/
void DataManage_GetPpgAdcDataGreenRedIredAmb(uint8_t *ppg_green, uint8_t *ppg_red, uint8_t *ppg_ired, uint8_t *ppg_amb)
{
	uint32_t pos = st_data_manage.data_pool.ppg_send_read_pos % PPG_CACHE_SIZE;
    
	// 绿光
    Combine_PackageData(ppg_green, &st_data_manage.data_pool.ppg_data.green[GREEN_POOL_SIZE - GREEN_CACHE_SIZE + pos],
                        st_data_manage.data_pool.ppg_send_package_index);
	// 红光
    Combine_PackageData(ppg_red, &st_data_manage.data_pool.ppg_data.red[RED_POOL_SIZE - RED_CACHE_SIZE + pos],
                        st_data_manage.data_pool.ppg_send_package_index);
	// 红外
    Combine_PackageData(ppg_ired, &st_data_manage.data_pool.ppg_data.ired[IRED_POOL_SIZE - IRED_CACHE_SIZE + pos],
                        st_data_manage.data_pool.ppg_send_package_index);
	// 环境光
	//Combine_PackageData(ppg_amb, &st_data_manage.data_pool.ppg_data.amb[AMB_POOL_SIZE - AMB_CACHE_SIZE + pos],
    //                    st_data_manage.data_pool.ppg_send_package_index);
    
    // 更新位置信息和长度信息
    if(st_data_manage.data_pool.ppg_send_package_index++ >= 255)
    {
        st_data_manage.data_pool.ppg_send_package_index = 0;
    }
    
    st_data_manage.data_pool.ppg_send_wait_len -= 5;
    st_data_manage.data_pool.ppg_send_read_pos += 5;
}

/*************************************************************************
** Function name:        DataManage_UpdateGreenAmbData
** Descriptions:         更新绿光环境光的数据缓存
** input parameter:      无   
** Returned Value:       0：正常，其余：出错
** Created  by:          JSH
** Created  Datas:       2018-12-19
**************************************************************************/
static int32_t DataManage_UpdateGreenAmbData(uint32_t start_flag, uint32_t over_flag)
{
	uint32_t	value = 0;
    uint32_t	num = 0;
	uint32_t	remain_num = 0;
	uint32_t	last_index = 0;
	uint32_t	over_sample = 0;
	int32_t 	err_or_num = 0;
	uint8_t		*p_data = NULL;
	
	
	if(start_flag == 1)
	{
		// 以下一行代码，打开，就不支持超读的情况
		//si1182_adc_last_data_num = 0;
		
		si1182_adc_data_num = 0;
	}
	
	num = si1182_adc_last_data_num;
	
	err_or_num = Si1182_GetDataPpg(&si1182_adc_cache[si1182_adc_last_data_num]);
	if(err_or_num < 0)
    {
        si1182_adc_last_data_num = 0;
        return err_or_num;
    }
	else
	{
		num += err_or_num;
		si1182_adc_data_num += err_or_num;
	}
    
	// 复制adc数据到缓存
	p_data = &si1182_adc_cache[0];
	for(uint32_t i = 0; i < num / GREEN_AMB_CACHE_REMAINDER; i++)
	{
		if(st_data_manage.data_pool.ppg_cache_update_cnt < PPG_CACHE_SIZE)
		{
			// 绿光
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			
			st_data_manage.data_pool.ppg_data.red[RED_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
			
			// 环境光
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			
			st_data_manage.data_pool.ppg_data.ired[IRED_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
			
			st_data_manage.data_pool.ppg_cache_update_cnt++;
		}
		else
		{
			// 由于更新fifo计数延时，导致本次的样本数中断的样本个数超过预设的样本数。
			// 只能保存 SI1182_FIFO_BUFFER_SIZE 个byte,超出报错
			last_index = i * 3;
			over_sample = 1;
			
			break;
		}
	}	
	
	if(over_flag == 0)
	{
		// 当前样本数中断的数据处理，未完成
		if(over_sample == 0)
		{
			remain_num = num % GREEN_AMB_CACHE_REMAINDER;
			for(uint32_t i = 0; i < remain_num; i++)
			{
				si1182_adc_cache[i] = si1182_adc_cache[(num / GREEN_AMB_CACHE_REMAINDER) * GREEN_AMB_CACHE_REMAINDER + i];
			}

			si1182_adc_last_data_num = remain_num;
		}
		else
		{
			remain_num = num - last_index;
			memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], remain_num);
			si1182_adc_last_data_num = remain_num;
		}
		
		return 0;
	}
	else
	{
		// 当前样本数中断的数据处理,已完成
		// 移动最新的1s数据
		memcpy(&st_data_manage.data_pool.ppg_data.red[0], &st_data_manage.data_pool.ppg_data.red[RED_CACHE_SIZE], 
				RED_POOL_SIZE * sizeof(int32_t));
		memcpy(&st_data_manage.data_pool.ppg_data.ired[0], &st_data_manage.data_pool.ppg_data.ired[IRED_CACHE_SIZE], 
				IRED_POOL_SIZE * sizeof(int32_t));
		
		st_data_manage.data_pool.ppg_send_wait_len = PPG_CACHE_SIZE;
		st_data_manage.data_pool.ppg_send_read_pos = 0;
		st_data_manage.data_pool.ppg_cache_update_cnt = 0;
		
		DataManage_PoolSecondUpdate();
		
		si1182_adc_last_data_num = si1182_adc_data_num - GREEN_AMB_CACHE_SIZE * 3;
    
		if(si1182_adc_last_data_num > (sizeof(si1182_adc_cache) / 2))
		{
			si1182_adc_last_data_num = 0;
			return -0xff - 1;
		}
		
		// 保存多余的adc数据
		if(si1182_adc_last_data_num)
		{
			if(last_index != 0)
			{
				memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], si1182_adc_last_data_num);
			}
		}
		
		return 0;
	}
}

/*************************************************************************
** Function name:        DataManage_UpdateGreenIredAmbData
** Descriptions:         更新绿光红外环境光的数据缓存
** input parameter:      无   
** Returned Value:       0：正常，其余：出错
** Created  by:          JSH
** Created  Datas:       2018-12-19
**************************************************************************/
static int32_t DataManage_UpdateGreenIredAmbData(uint32_t start_flag, uint32_t over_flag)
{
    uint32_t	value = 0;
    uint32_t	num = 0;
	uint32_t	remain_num = 0;
	uint32_t	last_index = 0;
	uint32_t	over_sample = 0;
	int32_t 	err_or_num = 0;
	uint8_t		*p_data = NULL;
	
	
	if(start_flag == 1)
	{
		// 以下一行代码，打开，就不支持超读的情况
		//si1182_adc_last_data_num = 0;
		
		si1182_adc_data_num = 0;
	}
	
	num = si1182_adc_last_data_num;
	
	err_or_num = Si1182_GetDataPpg(&si1182_adc_cache[si1182_adc_last_data_num]);
	if(err_or_num < 0)
    {
        si1182_adc_last_data_num = 0;
        return err_or_num;
    }
	else
	{
		num += err_or_num;
		si1182_adc_data_num += err_or_num;
	}   

	// 复制adc数据到缓存
	p_data = &si1182_adc_cache[0];
	for(uint32_t i = 0; i < num / GREEN_IRED_AMB_CACHE_REMAINDER; i++)
	{
		if(st_data_manage.data_pool.ppg_cache_update_cnt < PPG_CACHE_SIZE)
		{
			// 绿光
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.green[GREEN_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
			
			// 红外
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.ired[IRED_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
			
			// 环境光
            //p_data += 3;    // WJ.20190611.解决信号解析问题
            
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			/*
			//负数处理
            if(value & 0x800000)
            {
                /////value = -(0x7FFFFF - (value & 0x7FFFFF) + 1);
                value = value | 0xFF000000;
            }
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.amb[AMB_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
            */
			// 更新计数
			st_data_manage.data_pool.ppg_cache_update_cnt++;
		}
		else
		{
			// 由于更新fifo计数延时，导致本次的样本数中断的样本个数超过预设的样本数。
			// 只能保存 SI1182_FIFO_BUFFER_SIZE 个byte,超出报错
			last_index = i * 3;
			over_sample = 1;
			
			break;
		}
	}	
	
	if(over_flag == 0)
	{
		// 当前样本数中断的数据处理，未完成
		if(over_sample == 0)
		{
			remain_num = num % GREEN_IRED_AMB_CACHE_REMAINDER;
			for(uint32_t i = 0; i < remain_num; i++)
			{
				si1182_adc_cache[i] = si1182_adc_cache[(num / GREEN_IRED_AMB_CACHE_REMAINDER) * GREEN_IRED_AMB_CACHE_REMAINDER + i];
			}

			si1182_adc_last_data_num = remain_num;
		}
		else
		{
			remain_num = num - last_index;
			memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], remain_num);
			si1182_adc_last_data_num = remain_num;
		}
		
		return 0;
	}
	else
	{
		// 当前样本数中断的数据处理,已完成
		// 移动最新的1s数据
		memcpy(&st_data_manage.data_pool.ppg_data.green[0], &st_data_manage.data_pool.ppg_data.green[GREEN_CACHE_SIZE], 
				GREEN_POOL_SIZE * sizeof(int32_t));
		memcpy(&st_data_manage.data_pool.ppg_data.ired[0], &st_data_manage.data_pool.ppg_data.ired[IRED_CACHE_SIZE], 
				IRED_POOL_SIZE * sizeof(int32_t));
		//memcpy(&st_data_manage.data_pool.ppg_data.amb[0], &st_data_manage.data_pool.ppg_data.amb[AMB_CACHE_SIZE], 
		//		AMB_POOL_SIZE * sizeof(int32_t));
		
		st_data_manage.data_pool.ppg_send_wait_len = PPG_CACHE_SIZE;
		st_data_manage.data_pool.ppg_send_read_pos = 0;
		st_data_manage.data_pool.ppg_cache_update_cnt = 0;
		
		DataManage_PoolSecondUpdate();		
		
		si1182_adc_last_data_num = si1182_adc_data_num - GREEN_IRED_AMB_CACHE_SIZE * 3;
    
		if(si1182_adc_last_data_num > (sizeof(si1182_adc_cache) / 2))
		{
			si1182_adc_last_data_num = 0;
			return -0xff - 1;
		}
		
		// 保存多余的adc数据
		if(si1182_adc_last_data_num)
		{
			if(last_index != 0)
			{
				memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], si1182_adc_last_data_num);
			}
		}
		
		return 0;
	}
}

/*************************************************************************
** Function name:        DataManage_UpdateGreenRedIredAmbData
** Descriptions:         更新绿光红光红外环境光的数据缓存
** input parameter:      无   
** Returned Value:       0：正常，其余：出错
** Created  by:          JSH
** Created  Datas:       2018-12-19
**************************************************************************/
static int32_t DataManage_UpdateGreenRedIredAmbData(uint32_t start_flag, uint32_t over_flag)
{
    uint32_t	value = 0;
    uint32_t	num = 0;
	uint32_t	remain_num = 0;
	uint32_t	last_index = 0;
	uint32_t	over_sample = 0;
	int32_t 	err_or_num = 0;
	uint8_t		*p_data = NULL;
	
	
	if(start_flag == 1)
	{
		// 以下一行代码，打开，就不支持超读的情况
		//si1182_adc_last_data_num = 0;
		
		si1182_adc_data_num = 0;
	}
	
	num = si1182_adc_last_data_num;
	
	err_or_num = Si1182_GetDataPpg(&si1182_adc_cache[si1182_adc_last_data_num]);
	if(err_or_num < 0)
    {
        si1182_adc_last_data_num = 0;
        return err_or_num;
    }
	else
	{
		num += err_or_num;
		si1182_adc_data_num += err_or_num;
	}
	
	// 复制adc数据到缓存
	p_data = &si1182_adc_cache[0];
	for(uint32_t i = 0; i < num / GREEN_RED_IRED_AMB_CACHE_REMAINDER; i++)
	{
		if(st_data_manage.data_pool.ppg_cache_update_cnt < PPG_CACHE_SIZE)
		{
			// 绿光
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.green[GREEN_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;

			// 红光
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.red[RED_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
			
			// 红外
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.ired[IRED_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
			
			// 环境光
            //p_data += 3;    // WJ.20190611.解决信号解析问题
            
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			/*
			//负数处理
            if(value & 0x800000)
            {
                /////value = -(0x7FFFFF - (value & 0x7FFFFF) + 1);
                value = value | 0xFF000000;
            }
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.amb[AMB_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
            */
			// 更新计数
			st_data_manage.data_pool.ppg_cache_update_cnt++;
		}
		else
		{
			// 由于更新fifo计数延时，导致本次的样本数中断的样本个数超过预设的样本数。
			// 只能保存 SI1182_FIFO_BUFFER_SIZE 个byte,超出报错
			last_index = i * 3;
			over_sample = 1;
			
			break;
		}
	}	
	
	if(over_flag == 0)
	{
		// 当前样本数中断的数据处理，未完成
		if(over_sample == 0)
		{
			remain_num = num % GREEN_RED_IRED_AMB_CACHE_REMAINDER;
			for(uint32_t i = 0; i < remain_num; i++)
			{
				si1182_adc_cache[i] = si1182_adc_cache[(num / GREEN_RED_IRED_AMB_CACHE_REMAINDER) * GREEN_RED_IRED_AMB_CACHE_REMAINDER + i];
			}

			si1182_adc_last_data_num = remain_num;
		}
		else
		{
			remain_num = num - last_index;
			memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], remain_num);
			si1182_adc_last_data_num = remain_num;
		}
		
		return 0;
	}
	else
	{
		// 当前样本数中断的数据处理,已完成
		// 移动最新的1s数据
		memcpy(&st_data_manage.data_pool.ppg_data.green[0], &st_data_manage.data_pool.ppg_data.green[GREEN_CACHE_SIZE], 
				GREEN_POOL_SIZE * sizeof(int32_t));
		memcpy(&st_data_manage.data_pool.ppg_data.red[0], &st_data_manage.data_pool.ppg_data.red[RED_CACHE_SIZE], 
				RED_POOL_SIZE * sizeof(int32_t));
		memcpy(&st_data_manage.data_pool.ppg_data.ired[0], &st_data_manage.data_pool.ppg_data.ired[IRED_CACHE_SIZE], 
				IRED_POOL_SIZE * sizeof(int32_t));
		//memcpy(&st_data_manage.data_pool.ppg_data.amb[0], &st_data_manage.data_pool.ppg_data.amb[AMB_CACHE_SIZE], 
		//		AMB_POOL_SIZE * sizeof(int32_t));
		
		st_data_manage.data_pool.ppg_send_wait_len = PPG_CACHE_SIZE;
		st_data_manage.data_pool.ppg_send_read_pos = 0;
		st_data_manage.data_pool.ppg_cache_update_cnt = 0;
		
		DataManage_PoolSecondUpdate();		
		
		si1182_adc_last_data_num = si1182_adc_data_num - GREEN_RED_IRED_AMB_CACHE_SIZE * 3;
    
		if(si1182_adc_last_data_num > (sizeof(si1182_adc_cache) / 2))
		{
			si1182_adc_last_data_num = 0;
			return -0xff - 1;
		}
		
		// 保存多余的adc数据
		if(si1182_adc_last_data_num)
		{
			if(last_index != 0)
			{
				memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], si1182_adc_last_data_num);
			}
		}
		
		return 0;
	}
}

/*************************************************************************
** Function name:        DataManage_UpdateRedIredAmbData
** Descriptions:         更新红光红外环境光的数据缓存
** input parameter:      无   
** Returned Value:       0：正常，其余：出错
** Created  by:          JSH
** Created  Datas:       2019-2-19
**************************************************************************/
static int32_t DataManage_UpdateRedIredAmbData(uint32_t start_flag, uint32_t over_flag)
{
    uint32_t	value = 0;
    uint32_t	num = 0;
	uint32_t	remain_num = 0;
	uint32_t	last_index = 0;
	uint32_t	over_sample = 0;
	int32_t 	err_or_num = 0;
	uint8_t		*p_data = NULL;
	
	
	if(start_flag == 1)
	{
		// 以下一行代码，打开，就不支持超读的情况
		//si1182_adc_last_data_num = 0;
		
		si1182_adc_data_num = 0;
	}
	
	num = si1182_adc_last_data_num;
	
	err_or_num = Si1182_GetDataPpg(&si1182_adc_cache[si1182_adc_last_data_num]);
	if(err_or_num < 0)
    {
        si1182_adc_last_data_num = 0;
        return err_or_num;
    }
	else
	{
		num += err_or_num;
		si1182_adc_data_num += err_or_num;
	}
	
	// 复制adc数据到缓存
	p_data = &si1182_adc_cache[0];
	for(uint32_t i = 0; i < num / RED_IRED_AMB_CACHE_REMAINDER; i++)
	{
		if(st_data_manage.data_pool.ppg_cache_update_cnt < PPG_CACHE_SIZE)
		{
			// 红光
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.red[RED_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
			
			// 红外
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.ired[IRED_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
			
			// 环境光
            //p_data += 3;    // WJ.20190611.解决信号解析问题
            
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			/*
			//负数处理
            if(value & 0x800000)
            {
                /////value = -(0x7FFFFF - (value & 0x7FFFFF) + 1);
                value = value | 0xFF000000;
            }
			
			//以后看数据需求，是否需要进行符号位扩展
			st_data_manage.data_pool.ppg_data.amb[AMB_POOL_SIZE + st_data_manage.data_pool.ppg_cache_update_cnt] = value;
            */
			// 更新计数
			st_data_manage.data_pool.ppg_cache_update_cnt++;
		}
		else
		{
			// 由于更新fifo计数延时，导致本次的样本数中断的样本个数超过预设的样本数。
			// 只能保存 SI1182_FIFO_BUFFER_SIZE 个byte,超出报错
			last_index = i * 3;
			over_sample = 1;
			
			break;
		}
	}	
	
	if(over_flag == 0)
	{
		// 当前样本数中断的数据处理，未完成
		if(over_sample == 0)
		{
			remain_num = num % RED_IRED_AMB_CACHE_REMAINDER;
			for(uint32_t i = 0; i < remain_num; i++)
			{
				si1182_adc_cache[i] = si1182_adc_cache[(num / RED_IRED_AMB_CACHE_REMAINDER) * RED_IRED_AMB_CACHE_REMAINDER + i];
			}

			si1182_adc_last_data_num = remain_num;
		}
		else
		{
			remain_num = num - last_index;
			memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], remain_num);
			si1182_adc_last_data_num = remain_num;
		}
		
		return 0;
	}
	else
	{
		// 当前样本数中断的数据处理,已完成
		// 移动最新的1s数据
		memcpy(&st_data_manage.data_pool.ppg_data.red[0], &st_data_manage.data_pool.ppg_data.red[RED_CACHE_SIZE], 
				RED_POOL_SIZE * sizeof(int32_t));
		memcpy(&st_data_manage.data_pool.ppg_data.ired[0], &st_data_manage.data_pool.ppg_data.ired[IRED_CACHE_SIZE], 
				IRED_POOL_SIZE * sizeof(int32_t));
		//memcpy(&st_data_manage.data_pool.ppg_data.amb[0], &st_data_manage.data_pool.ppg_data.amb[AMB_CACHE_SIZE], 
		//		AMB_POOL_SIZE * sizeof(int32_t));
		
		st_data_manage.data_pool.ppg_send_wait_len = PPG_CACHE_SIZE;
		st_data_manage.data_pool.ppg_send_read_pos = 0;
		st_data_manage.data_pool.ppg_cache_update_cnt = 0;
		
		DataManage_PoolSecondUpdate();		
		
		si1182_adc_last_data_num = si1182_adc_data_num - RED_IRED_AMB_CACHE_SIZE * 3;
    
		if(si1182_adc_last_data_num > (sizeof(si1182_adc_cache) / 2))
		{
			si1182_adc_last_data_num = 0;
			return -0xff - 1;
		}
		
		// 保存多余的adc数据
		if(si1182_adc_last_data_num)
		{
			if(last_index != 0)
			{
				memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], si1182_adc_last_data_num);
			}
		}
		
		return 0;
	}
}

/****************************************************************
** Function name:             DataManage_UpdateEcgData
** Descriptions:              获取si1182的原始数据，转换
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-19
****************************************************************/
int32_t DataManage_UpdateEcgData(uint32_t start_flag, uint32_t over_flag)
{
    uint32_t	value = 0;
    uint32_t	num = 0;
	uint32_t	remain_num = 0;
	uint32_t	last_index = 0;
	uint32_t	over_sample = 0;
	int32_t 	err_or_num = 0;
	uint8_t		*p_data = NULL;
	
	
	if(start_flag == 1)
	{
		// 以下一行代码，打开，就不支持超读的情况
		//si1182_adc_last_data_num = 0;
		
		si1182_adc_data_num = 0;
	}
	
	num = si1182_adc_last_data_num;
	
	err_or_num = Si1182_GetDataEcg(&si1182_adc_cache[si1182_adc_last_data_num]);
	if(err_or_num < 0)
    {
        si1182_adc_last_data_num = 0;
        return err_or_num;
    }
	else
	{
		num += err_or_num;
		si1182_adc_data_num += err_or_num;
	}

	// 复制adc数据到缓存
	p_data = &si1182_adc_cache[0];
	for(uint32_t i = 0; i < num / ECG_CACHE_REMAINDER; i++)
	{
		if(st_data_manage.data_pool.ecg_cache_update_cnt < ECG_CACHE_SIZE)
		{
			value  = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
			
			//以后看数据需求，是否需要进行符号位扩展
			
			st_data_manage.data_pool.ecg_data[ECG_POOL_SIZE + st_data_manage.data_pool.ecg_cache_update_cnt] = value;
			st_data_manage.data_pool.ecg_cache_update_cnt++;
		}
		else
		{
			// 由于更新fifo计数延时，导致本次的样本数中断的样本个数超过预设的样本数。
			// 只能保存 SI1182_FIFO_BUFFER_SIZE 个byte,超出报错
			last_index = i * 3;
			over_sample = 1;
			
			break;
		}
	}	
	
	if(over_flag == 0)
	{
		// 当前样本数中断的数据处理，未完成
		if(over_sample == 0)
		{
			remain_num = num % ECG_CACHE_REMAINDER;
			for(uint32_t i = 0; i < remain_num; i++)
			{
				si1182_adc_cache[i] = si1182_adc_cache[(num / ECG_CACHE_REMAINDER) * ECG_CACHE_REMAINDER + i];
			}

			si1182_adc_last_data_num = remain_num;
		}
		else
		{
			remain_num = num - last_index;
			memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], remain_num);
			si1182_adc_last_data_num = remain_num;
		}
		
		return 0;
	}
	else
	{
		// 当前样本数中断的数据处理,已完成
		// 移动最新的1s数据
		memcpy(&st_data_manage.data_pool.ecg_data[0], &st_data_manage.data_pool.ecg_data[ECG_CACHE_SIZE], 
				ECG_POOL_SIZE * sizeof(int32_t));
		
		st_data_manage.data_pool.ecg_send_wait_len = ECG_CACHE_SIZE;
		st_data_manage.data_pool.ecg_send_read_pos = 0;
		st_data_manage.data_pool.ecg_cache_update_cnt = 0;
		
		DataManage_PoolSecondUpdate();
		
		si1182_adc_last_data_num = si1182_adc_data_num - ECG_CACHE_SIZE * 3;
		  
		if(si1182_adc_last_data_num > (sizeof(si1182_adc_cache) / 2))
		{
			si1182_adc_last_data_num = 0;
			return -0xff - 1;
		}
		
		// 保存多余的adc数据
		if(si1182_adc_last_data_num)
		{
			if(last_index != 0)
			{
				memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], si1182_adc_last_data_num);
			}
		}
		
		return 0;
	}
}

/****************************************************************
** Function name:             DataManage_UpdateEcgDataAndLd
** Descriptions:              获取si1182的原始数据，转换ecg和导联
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-19
****************************************************************/
int32_t DataManage_UpdateEcgDataAndLd(uint32_t start_flag, uint32_t over_flag)
{
    uint32_t    value = 0;
    uint32_t	num = 0;
	uint32_t	remain_num = 0;
	uint32_t	last_index = 0;
	uint32_t	over_sample = 0;
	int32_t 	err_or_num = 0;
	uint8_t		*p_data = NULL;
	uint32_t 	ld_data_num = 0;
	
	
    adc_ecg_signal_state1++;
    
	if(start_flag == 1)
	{
		// 以下一行代码，打开，就不支持超读的情况
		//si1182_adc_last_data_num = 0;
		
		si1182_adc_data_num = 0;
		si1182_ld_data_allnum = 0;
	}
	
	num = si1182_adc_last_data_num;
	
	err_or_num = Si1182_GetDataEcgAndLd(&si1182_adc_cache[si1182_adc_last_data_num], (uint8_t *)&si1182_ld_cache[si1182_ld_last_data_num], &ld_data_num);
	if(err_or_num < 0)
    {
        // adc error
        adc_ecg_signal_state = 2;
        
        si1182_adc_last_data_num = 0;
		si1182_ld_last_data_num = 0;
        return err_or_num;
    }
	else
	{
		num += err_or_num;
		si1182_adc_data_num += err_or_num;
		si1182_ld_data_num += ld_data_num;
	}

	// 复制adc数据到缓存
	p_data = &si1182_adc_cache[0];
	for(uint32_t i = 0; i < num / ECG_CACHE_REMAINDER; i++)
	{
		if(st_data_manage.data_pool.ecg_cache_update_cnt < ECG_CACHE_SIZE)
		{
			value = *p_data<< 16;
			p_data++;
			value |= (*p_data<< 8);
			p_data++;
			value |= (*p_data);
			p_data++;
            
            //负数处理
            if(value & 0x800000)
            {
                /////value = -(0x7FFFFF - (value & 0x7FFFFF) + 1);
                value = value | 0xFF000000;
            }
			
			//以后看数据需求，是否需要进行符号位扩展
			
			st_data_manage.data_pool.ecg_data[ECG_POOL_SIZE + st_data_manage.data_pool.ecg_cache_update_cnt] = value;
			st_data_manage.data_pool.ecg_cache_update_cnt++;
		}
		else
		{
			// 由于更新fifo计数延时，导致本次的样本数中断的样本个数超过预设的样本数。
			// 只能保存 SI1182_FIFO_BUFFER_SIZE 个byte,超出报错
			last_index = i * 3;
			over_sample = 1;
			
			break;
		}
	}	
	
	// 复制导联数据到缓存
	for(uint32_t i = 0; i < si1182_ld_data_num / LD_CACHE_REMAINDER; i++)
	{
		uint32_t    j = 0;
		int16_t     value_1 = 0;
		int16_t     value_2 = 0;
		
		j = i * 4;
		if(si1182_ld_cache[j] >= si1182_ld_cache[j + 2])
		{
			value_1 = si1182_ld_cache[j] - si1182_ld_cache[j + 2];
		}
		else
		{
			value_1 = si1182_ld_cache[j + 2] - si1182_ld_cache[j];
		}
		
		if(si1182_ld_cache[j + 1] >= si1182_ld_cache[j + 3])
		{
			value_2 = si1182_ld_cache[j + 1] - si1182_ld_cache[j + 3];
		}
		else
		{
			value_2 = si1182_ld_cache[j + 3] - si1182_ld_cache[j + 1];
		}
		
        if(value_1 < 0)
        {
            value_1 = -value_1;
        }
        if(value_2 < 0)
        {
            value_2 = -value_2;
        }
        
        
		st_data_manage.data_pool.ld_data[LD_POOL_SIZE] = value_1 + value_2;
		memcpy(&st_data_manage.data_pool.ld_data[0], &st_data_manage.data_pool.ld_data[LD_CACHE_SIZE], LD_POOL_SIZE * sizeof(int32_t));
		st_data_manage.data_pool.ld_update_counter++;
	}
	si1182_ld_last_data_num = si1182_ld_data_num % LD_CACHE_REMAINDER;
	if(si1182_ld_last_data_num)
	{
		memcpy(&si1182_ld_cache[0], &si1182_ld_cache[(si1182_ld_data_num / LD_CACHE_REMAINDER) * LD_CACHE_REMAINDER], si1182_ld_last_data_num);
	}
	// JSH.20190119解决临界条件
	si1182_ld_data_allnum += si1182_ld_data_num;
	si1182_ld_data_num = si1182_ld_last_data_num;
	
	if(over_flag == 0)
	{
		// 当前样本数中断的数据处理，未完成
		if(over_sample == 0)
		{
			remain_num = num % ECG_CACHE_REMAINDER;
			for(uint32_t i = 0; i < remain_num; i++)
			{
				si1182_adc_cache[i] = si1182_adc_cache[(num / ECG_CACHE_REMAINDER) * ECG_CACHE_REMAINDER + i];
			}

			si1182_adc_last_data_num = remain_num;
		}
		else
		{
			remain_num = num - last_index;
			memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], remain_num);
			si1182_adc_last_data_num = remain_num;
		}
		
		return 0;
	}
	else
	{
		// 当前样本数中断的数据处理,已完成
		// 移动最新的1s数据
		memcpy(&st_data_manage.data_pool.ecg_data[0], &st_data_manage.data_pool.ecg_data[ECG_CACHE_SIZE], 
				ECG_POOL_SIZE * sizeof(int32_t));
        ///WJ.20190304.多余导致ecg计算不出值
		////memset(&st_data_manage.data_pool.ecg_data[ECG_CACHE_SIZE], 0, ECG_CACHE_SIZE * sizeof(int32_t));
        
        // 清空缓存
        memset(&st_data_manage.data_pool.ecg_data[ECG_POOL_SIZE], 0, ECG_CACHE_SIZE * sizeof(int32_t));
        
		
		st_data_manage.data_pool.ecg_send_wait_len = ECG_CACHE_SIZE;
		st_data_manage.data_pool.ecg_send_read_pos = 0;
		st_data_manage.data_pool.ecg_cache_update_cnt = 0;
		
		DataManage_PoolSecondUpdate();
		
		si1182_adc_last_data_num = si1182_adc_data_num /* + si1182_ld_data_allnum */ - ECG_CACHE_SIZE * 3 /* - LD_POOL_SIZE * 4 */;
		  
		if(si1182_adc_last_data_num > (sizeof(si1182_adc_cache) / 2))
		{
			si1182_adc_last_data_num = 0;
			return -0xff - 1;
		}
		
		// 保存多余的adc数据
		if(si1182_adc_last_data_num)
		{
			if(last_index != 0)
			{
				memcpy(&si1182_adc_cache[0], &si1182_adc_cache[last_index], si1182_adc_last_data_num);
			}
		}
		
		return 0;
	}
}
	
/****************************************************************
** Function name:             DataManage_UpdatePpgData
** Descriptions:              获取si1182的原始数据，转换
** input parameters:                                      
** output parameters:         
** Returned value:            
** Created by:                JSH
** Created Date:              2018-12-19
****************************************************************/
int32_t DataManage_UpdateAdcData(uint32_t adc_mode, uint32_t start_flag, uint32_t over_flag)
{
	int32_t ret_value = 0;
	
	switch(adc_mode)
	{
		case MODE_ECG:
			
			ret_value = DataManage_UpdateEcgDataAndLd(start_flag, over_flag);
			break;
		case MODE_GREEN_AMB:
			
			ret_value = DataManage_UpdateGreenAmbData(start_flag, over_flag);
			break;
		case MODE_GREEN_IRED_AMB:
			
			ret_value = DataManage_UpdateGreenIredAmbData(start_flag, over_flag);
			break;
		case MODE_GREEN_RED_IRED_AMB:
			
			ret_value = DataManage_UpdateGreenRedIredAmbData(start_flag, over_flag);
			break;
		case MODE_RED_IRED_AMB:
			
			ret_value = DataManage_UpdateRedIredAmbData(start_flag, over_flag);
		
		default:
			
			ret_value = -0xffff;
			break;
	}
	
	return ret_value;
}

