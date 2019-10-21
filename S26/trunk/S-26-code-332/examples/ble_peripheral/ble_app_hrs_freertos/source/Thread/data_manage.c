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
#include "afe4900_init.h"


static data_manage_param  st_data_manage;

data_manage_param*  p_data_manage = &st_data_manage;
/*******************************************************************************************************
** Function name:        DataManage_Init
** Descriptions:         data manage  init
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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
//    st_data_manage.blood_oxygen_measure_s.result = 0;
    
    // 脱手检测算法初始化
    st_data_manage.no_wear_measure_s.init = 1;
	
    // 开机标志置位
    st_data_manage.device_trace_param_s.sign_on = 1;
    
    st_data_manage.upload = UPLOAD_ADC_NULL_EVENT;
    st_data_manage.data_pool.send_read_pos = 100;
    st_data_manage.data_pool.send_wait_len = 0;
    
    Ped_SetSportsInfo(p_flash_param->flash_index_s.daily.step_cnt[local_day_id], p_flash_param->flash_index_s.daily.calories_cnt[local_day_id]*100,
                      p_flash_param->flash_index_s.daily.distance_cnt[local_day_id]);
    Ped_SetInputPersonalInformation(p_flash_param->person_s.height, p_flash_param->person_s.weight,
                                    p_flash_param->person_s.gender, p_flash_param->person_s.age);
}

/*******************************************************************************************************
** Function name:        DataManage_UpdateFIFOData
** Descriptions:         update FIFO Data
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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

/*******************************************************************************************************
** Function name:        DataManage_ProcessGestureData
** Descriptions:         process Gesture data
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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
        ((float)(gesture_data[1][index] *gesture_data[1][index])) + ((float)(gesture_data[2][index] *gesture_data[2][index]))))>>4; 
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
/*******************************************************************************************************
** Function name:        DataManage_ProcessPedometerData
** Descriptions:         process pedometer data
** input parameter:      *p_xdata :the x-coordinate data    * p_ydata:the Y-coordinate data        
**                       *p_zdata:the Z-coordinate data 
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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
/*******************************************************************************************************
** Function name:        DataManage_GetRTAccData
** Descriptions:         process pedometer data
** input parameter:      *p_data :the RTacc data
**                      
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void DataManage_GetRTAccData(int16_t* p_data)
{
    uint8_t pos = (st_data_manage.acc.data_w_pos+99)%100;
    p_data[0] = st_data_manage.acc.x_axis_data[pos];
    p_data[1] = st_data_manage.acc.y_axis_data[pos];
    p_data[2] = st_data_manage.acc.z_axis_data[pos];
}
/*******************************************************************************************************
** Function name:        DataManage_GetGsensorADCData
** Descriptions:         get Gsensor adc data
** input parameter:      *p_data:to  store  the   Gsensor data
**                      
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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

/*******************************************************************************************************
** Function name:        DataManage_GetGestureData
** Descriptions:         to get gesture data
** input parameter:      *p_xdata :to store  the gesture  data
**                      
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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

/*******************************************************************************************************
** Function name:        DataManage_AccData_Send_Reset
** Descriptions:         acc data send reset 
** input parameter:      void 
**                       
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void DataManage_AccData_Send_Reset(void)
{
    st_data_manage.acc.adc_send_pos = st_data_manage.acc.data_w_pos;
}
/*******************************************************************************************************
** Function name:        DM_Update_DailyHeartData
** Descriptions:         updata daily heart data
** input parameter:      heart:the data  to updata
**                      
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void DM_Update_DailyHeartData(uint8_t heart)
{
    st_data_manage.daily_attr.heart = heart;
}
/*******************************************************************************************************
** Function name:        DM_Get_NewestDailyData
** Descriptions:         get the newest daily data 
** input parameter:      void 
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
daily_data_attr_t* DM_Get_NewestDailyData(void)
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
	
    // 计算心率平均值	
    st_data_manage.daily_attr.heart = st_data_manage.heart_measure_s.time_heart;
    st_data_manage.heart_measure_s.time_heart = 0;
	// 血氧值初始化
    st_data_manage.daily_attr.ox_value = 0;
	//重置错误代码
    st_data_manage.daily_attr.resv = 0;	
    
    return &st_data_manage.daily_attr;
}

/*******************************************************************************************************
** Function name:        DataManage_GetCurrentStepCnt
** Descriptions:         get  local step cnt
** input parameter:      void  
**                      
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
uint32_t DataManage_GetCurrentStepCnt(void)
{
//    return st_data_manage.pedometer.power_on_step + All_StepGet();
    sport_information_info_t gsii_sport_info_s = {0x00, };
    
    Get_SportInformationInfo(&gsii_sport_info_s); 
    return gsii_sport_info_s.posi_steps;//All_StepGet();
}

/*******************************************************************************************************
** Function name:        DM_Update_Brightness
** Descriptions:         updata brightness
** input parameter:      brightness :the  value to updata
**                       
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void DM_Update_Brightness(uint8_t brightness)
{
    st_data_manage.oled_bright = brightness;
}

 /*******************************************************************************************************
** Function name:        DataManage_AFE4404PoolInit
** Descriptions:         AFE4404Pool init
** input parameter:      void                            
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void DataManage_AFE4404PoolInit(void)
{
    st_data_manage.data_pool.pool_update_second = 0;
    st_data_manage.data_pool.cache_update_cnt = 0;
    memset(&st_data_manage.data_pool.red[0], 0, sizeof(st_data_manage.data_pool.red));
    memset(&st_data_manage.data_pool.ired[0], 0, sizeof(st_data_manage.data_pool.ired));
    memset(&st_data_manage.data_pool.green[0], 0, sizeof(st_data_manage.data_pool.green));
    memset(&st_data_manage.data_pool.ambient[0], 0, sizeof(st_data_manage.data_pool.ambient));
    //YX.20180628.把佩戴的绿光数据单独出来
    memset(&st_data_manage.data_pool.wear_green[0], 0, sizeof(st_data_manage.data_pool.wear_green));

//    st_data_manage.blood_oxygen_measure_s.result = 0;
	
    // 重新初始化发送到信息
    st_data_manage.data_pool.send_read_pos = 100;
    st_data_manage.data_pool.send_wait_len = 0;
    st_data_manage.data_pool.send_package_index = 0;
}
/*******************************************************************************************************
** Function name:        DataManage_GetRedPoolData
** Descriptions:         get AFE4404Red pool data
** input parameter:      void  
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
const int32_t* DataManage_GetRedPoolData(void)
{
    return st_data_manage.data_pool.red;
}
/*******************************************************************************************************
** Function name:        DataManage_GetiRedDataPool
** Descriptions:         get ired data pool
** input parameter:      void  
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
const int32_t* DataManage_GetiRedDataPool(void)
{
    return st_data_manage.data_pool.ired;
}

///*******************************************************************************************************
//** Function name:        DM_Get_BR_iRedDataPool
//** Descriptions:         get BR iRed Data pool
//** input parameter:      void  
//**                        
//** Returned Value:       void
//** Created  by:          GT
//** Created  Datas:       2017-08-04
//**------------------------------------------------------------------------------------------------------
//** Modify By:
//** Modify data:
//** Modify Description:
//*********************************************************************************************************/
//const uint32_t* DataManage_GetRespiRedDataPool(void)
//{
//    return st_data_manage.data_pool.ired;
//}

const int32_t* DataManage_GetTiredGreenPool(void)
{
    return &st_data_manage.data_pool.green[550];
}

/*******************************************************************************************************
** Function name:        DataManage_GetGreenCompletePool
** Descriptions:         get  green complete pool
** input parameter:      void  
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
const int32_t* DataManage_GetGreenCompletePool(void)
{
    return &st_data_manage.data_pool.green[450];
}

/*******************************************************************************************************
** Function name:        DataManage_GetGreenBrPool
** Descriptions:         get  green br pool
** input parameter:      void  
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
const int32_t* DataManage_GetRespGreenDataPool(void)
{
    return st_data_manage.data_pool.green;
}

/*******************************************************************************************************
** Function name:        DM_Get_Wear_GreenDataPool
** Descriptions:         get wear green data pool
** input parameter:      void  
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
const int32_t* DataManage_GetWearGreenDataPool(void)
{
    //YX.20180628.把佩戴的绿光数据单独出来
    return st_data_manage.data_pool.wear_green;
}
/*******************************************************************************************************
** Function name:        DataManage_GetAmbientDataPool
** Descriptions:         get ambient data pool
** input parameter:      void  
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
const int32_t* DataManage_GetAmbientDataPool(void)
{
    return st_data_manage.data_pool.ambient;
}

void DataManage_UpdateBloodOxygenADCData(int32_t red, int32_t ired, int32_t green, int32_t ambient)
{
    uint16_t red_cache_w_pos = 0;
    uint16_t ired_cache_w_pos = 0;
    uint16_t green_cache_w_pos = 0;
    uint16_t ambient_cache_w_pos = 0;
    
    // 更新AFE4404 Cache缓存池数据
    if(st_data_manage.data_pool.cache_update_cnt < DATA_CACHE_POOL_SIZE)
    {
        green_cache_w_pos = GREEN_POOL_SIZE + st_data_manage.data_pool.cache_update_cnt;
        ambient_cache_w_pos = AMBIENT_POOL_SIZE + st_data_manage.data_pool.cache_update_cnt;
        red_cache_w_pos  = RED_POOL_SIZE + st_data_manage.data_pool.cache_update_cnt;
        ired_cache_w_pos = IRED_POOL_SIZE + st_data_manage.data_pool.cache_update_cnt;
        
        st_data_manage.data_pool.green[green_cache_w_pos] = green;
        st_data_manage.data_pool.ambient[ambient_cache_w_pos] = ambient;   
        st_data_manage.data_pool.red[red_cache_w_pos] = red;
        st_data_manage.data_pool.ired[ired_cache_w_pos] = ired;
        
        //YX.20180319.解决使用生产测试采集原始出现数据错乱问题
//        st_data_manage.data_pool.send_wait_len = (st_data_manage.data_pool.send_wait_len+1)%RED_POOL_SIZE;
        st_data_manage.data_pool.send_wait_len = (st_data_manage.data_pool.send_wait_len+1)%(AMBIENT_POOL_SIZE+DATA_CACHE_POOL_SIZE+1);
    }
    
    st_data_manage.data_pool.cache_update_cnt++;	
}

void DataManage_UpdateHeartADCData( int32_t green, int32_t ambient)
{
    uint16_t green_cache_w_pos = 0;
    uint16_t ambient_cache_w_pos = 0;
    
    // 更新AFE4404 Cache缓存池数据
    if(st_data_manage.data_pool.cache_update_cnt < DATA_CACHE_POOL_SIZE)
    {
        green_cache_w_pos = GREEN_POOL_SIZE + st_data_manage.data_pool.cache_update_cnt;
        ambient_cache_w_pos = AMBIENT_POOL_SIZE + st_data_manage.data_pool.cache_update_cnt;
        
        st_data_manage.data_pool.green[green_cache_w_pos] = green;
        st_data_manage.data_pool.ambient[ambient_cache_w_pos] = ambient;
        
//        st_data_manage.data_pool.send_wait_len = (st_data_manage.data_pool.send_wait_len+1)%RED_POOL_SIZE;
        st_data_manage.data_pool.send_wait_len = (st_data_manage.data_pool.send_wait_len+1)%(AMBIENT_POOL_SIZE+DATA_CACHE_POOL_SIZE+1);
    }
    
    st_data_manage.data_pool.cache_update_cnt++;		
}

/*******************************************************************************************************
** Function name:        DataManage_CopyCacheData
** Descriptions:         copy cache data
** input parameter:      void  
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void DataManage_CopyBloodOxygenCacheData(void)
{
    // AFE4404缓存池时间技术加1
    st_data_manage.data_pool.pool_update_second++;
    st_data_manage.data_pool.cache_update_cnt = 0;
    
    if(st_data_manage.data_pool.send_read_pos >= DATA_CACHE_POOL_SIZE)
    {
        st_data_manage.data_pool.send_read_pos -= DATA_CACHE_POOL_SIZE;
    }
    
    // 移动最新1秒数据
    memcpy(&st_data_manage.data_pool.red[0], &st_data_manage.data_pool.red[DATA_CACHE_POOL_SIZE], RED_POOL_SIZE*sizeof(uint32_t));
    memcpy(&st_data_manage.data_pool.ired[0], &st_data_manage.data_pool.ired[DATA_CACHE_POOL_SIZE], IRED_POOL_SIZE*sizeof(uint32_t));
    memcpy(&st_data_manage.data_pool.green[0], &st_data_manage.data_pool.green[DATA_CACHE_POOL_SIZE], GREEN_POOL_SIZE*sizeof(uint32_t));
    memcpy(&st_data_manage.data_pool.ambient[0], &st_data_manage.data_pool.ambient[DATA_CACHE_POOL_SIZE], AMBIENT_POOL_SIZE*sizeof(uint32_t));
    //YX.20180628.把佩戴的绿光数据单独出来
    memcpy(&st_data_manage.data_pool.wear_green[0], &st_data_manage.data_pool.green[GREEN_POOL_SIZE-WEAR_GREEN_POOL_SIZE], WEAR_GREEN_POOL_SIZE*sizeof(uint32_t));
    
}

void DataManage_CopyHeartCacheData(void)
{
    // AFE4404缓存池时间技术加1
    st_data_manage.data_pool.pool_update_second++;
    st_data_manage.data_pool.cache_update_cnt = 0;
    
    if(st_data_manage.data_pool.send_read_pos >= DATA_CACHE_POOL_SIZE)
    {
        st_data_manage.data_pool.send_read_pos -= DATA_CACHE_POOL_SIZE;
    }
    
    // 移动最新1秒数据
    memcpy(&st_data_manage.data_pool.green[0], &st_data_manage.data_pool.green[DATA_CACHE_POOL_SIZE], GREEN_POOL_SIZE*sizeof(uint32_t));
    memcpy(&st_data_manage.data_pool.ambient[0], &st_data_manage.data_pool.ambient[DATA_CACHE_POOL_SIZE], AMBIENT_POOL_SIZE*sizeof(uint32_t));	
    //YX.20180628.把佩戴的绿光数据单独出来
    memcpy(&st_data_manage.data_pool.wear_green[0], &st_data_manage.data_pool.green[GREEN_POOL_SIZE-WEAR_GREEN_POOL_SIZE], WEAR_GREEN_POOL_SIZE*sizeof(uint32_t));
}

static void Combine_PackageData(uint8_t *p_data, int32_t *p_adc, uint8_t package)
{
    uint32_t sum = 0;
    
    for(uint8_t index = 0; index < 5; index++)
    {
        p_data[index*3]   = (p_adc[index]>>16)&0xFF;
        p_data[index*3+1] = (p_adc[index]>>8)&0xFF;
        p_data[index*3+2] =  p_adc[index]&0xFF;
        sum += p_adc[index];
    }
    
    sum = sum&0xFFFFFF;
    p_data[15] = (sum>>16)&0xFF;
    p_data[16] = (sum>>8)&0xFF;
    p_data[17] = sum&0xFF;
    p_data[18] = package;
}	
/*******************************************************************************************************
** Function name:        DataManage_GetOxygenADCData
** Descriptions:         get oxygen adc data 
** input parameter:      *p_red :red light data  p_ired:the ired data  * package:the package data to store    
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void DataManage_GetOxygenADCData(uint8_t *p_green, uint8_t *p_red, uint8_t *p_ired, uint8_t *p_ambient)
{
    uint16_t pos = st_data_manage.data_pool.send_read_pos%146;
    
    Combine_PackageData(p_green,&st_data_manage.data_pool.green[650+pos],st_data_manage.data_pool.send_package_index);
    Combine_PackageData(p_red,&st_data_manage.data_pool.red[200+pos],st_data_manage.data_pool.send_package_index);
    Combine_PackageData(p_ired,&st_data_manage.data_pool.ired[200+pos],st_data_manage.data_pool.send_package_index);
    Combine_PackageData(p_ambient,&st_data_manage.data_pool.ambient[pos],st_data_manage.data_pool.send_package_index);
    
    // 更新位置信息和长度信息
    if(st_data_manage.data_pool.send_package_index++ >= 255)
    {
        st_data_manage.data_pool.send_package_index = 0;
    }
    
    st_data_manage.data_pool.send_wait_len -= 5;
    st_data_manage.data_pool.send_read_pos += 5;
}    

/*******************************************************************************************************
** Function name:        DataManage_GetHeartADCData
** Descriptions:         heart adc data
** input parameter:      p_green: the green light data  *package:the package data  to get
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void DataManage_GetHeartADCData(uint8_t *p_green, uint8_t *p_ambient)
{
    uint16_t pos = st_data_manage.data_pool.send_read_pos%146;
    
    Combine_PackageData(p_green,&st_data_manage.data_pool.green[650+pos],st_data_manage.data_pool.send_package_index);
    Combine_PackageData(p_ambient,&st_data_manage.data_pool.ambient[pos],st_data_manage.data_pool.send_package_index);
    
    // 更新位置信息和长度信息
    if(st_data_manage.data_pool.send_package_index++ >= 255)
    {
        st_data_manage.data_pool.send_package_index = 0;
    }
    
    st_data_manage.data_pool.send_wait_len -= 5;
    st_data_manage.data_pool.send_read_pos += 5;
}

/*******************************************************************************************************
** Function name:        DM_Process_OxygenAttrData
** Descriptions:         process oxygenattr data
** input parameter:      void  
**                        
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void DataManage_ProcessOxygenAttrData(void)
{
    st_data_manage.oxygen_attr.hour = clock->hour;
    st_data_manage.oxygen_attr.minute = clock->minutes;
               
    // 计算血氧平均值
    if(st_data_manage.blood_oxygen_measure_s.cnt > 0)
    {
        st_data_manage.oxygen_attr.oxygen = Average_CharArray(&st_data_manage.blood_oxygen_measure_s.time_result[st_data_manage.blood_oxygen_measure_s.min_start_index],
                                                              st_data_manage.blood_oxygen_measure_s.cnt - st_data_manage.blood_oxygen_measure_s.min_start_index);
        
//        // !注意:血氧数据获取后需要初始化参数 Add by GT 2017-10-19
//        st_data_manage.blood_oxygen_measure_s.cnt = 0;
//        memset(&st_data_manage.blood_oxygen_measure_s.time_result,0, sizeof(st_data_manage.blood_oxygen_measure_s.time_result));
        st_data_manage.blood_oxygen_measure_s.min_start_index = st_data_manage.blood_oxygen_measure_s.cnt;
        
    }
    else
    {
        st_data_manage.oxygen_attr.oxygen = st_data_manage.blood_oxygen_measure_s.err_code;
        st_data_manage.blood_oxygen_measure_s.err_code = 0;
    }
    
    st_data_manage.oxygen_attr.heart = st_data_manage.heart_measure_s.time_heart;
    
    if(st_data_manage.hrv_measure_s.cnt > 0)
    {
        st_data_manage.oxygen_attr.hrv = Average_CharArray(st_data_manage.hrv_measure_s.result,st_data_manage.hrv_measure_s.cnt);
        
        // !注意:HRV数据获取后需要初始化参数 Add by GT 2017-10-19
        st_data_manage.hrv_measure_s.cnt = 0;
        memset(&st_data_manage.hrv_measure_s.result,0, sizeof(st_data_manage.hrv_measure_s.result));
    }
    else
    {
        st_data_manage.oxygen_attr.hrv = 0;
    }
    
	st_data_manage.oxygen_attr.br = st_data_manage.respiratory_rate_s.result;
    st_data_manage.respiratory_rate_s.result = 0;
//    if(st_data_manage.respiratory_rate_s.cnt > 0)
//    {
//        st_data_manage.oxygen_attr.br = Average_CharArray(st_data_manage.respiratory_rate_s.result,st_data_manage.respiratory_rate_s.cnt);
//        
//        // !注意:呼吸率数据获取后需要初始化参数 Add by GT 2017-10-19
//        st_data_manage.respiratory_rate_s.cnt = 0;
//        memset(&st_data_manage.respiratory_rate_s.result,0, sizeof(st_data_manage.respiratory_rate_s.result));
//    }
//    else
//    {
//        st_data_manage.oxygen_attr.br = st_data_manage.respiratory_rate_s.err_code;
//        st_data_manage.respiratory_rate_s.err_code = 0;
//    }
	
    // 心脏负荷结果计算
    if(st_data_manage.heart_load_param_s.cnt > 0)
    {
        st_data_manage.oxygen_attr.heart_load = Average_CharArray(st_data_manage.heart_load_param_s.result,st_data_manage.heart_load_param_s.cnt);
        
        // !注意:心脏负荷数据获取后需要初始化参数 Add by GT 2017-10-19
        st_data_manage.heart_load_param_s.cnt = 0;
        memset(&st_data_manage.heart_load_param_s.result, 0, sizeof(st_data_manage.heart_load_param_s.result));
    }
    else
    {
        st_data_manage.oxygen_attr.heart_load = st_data_manage.heart_load_param_s.err_code;
        st_data_manage.heart_load_param_s.err_code = 0;
    }
}

