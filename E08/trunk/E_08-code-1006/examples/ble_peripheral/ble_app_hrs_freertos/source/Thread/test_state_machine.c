/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  test_state_machine.c
** Last modified Date:         2018-11-12
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2018-11-12
** SYS_VERSION:                0.0.1
** Descriptions:               测量状态机
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "test_state_machine.h"

#include "data_manage.h"
#include "calc_data_type.h"
#include "ble_thread.h"

/****************************************************************
** Function name:           Statemachine_AddState
** Descriptions:            测量状态加入
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-12
*****************************************************************/ 
uint8_t Statemachine_AddState(uint16_t the_state)
{
    ecg_ppg_measure_t *p_measure = NULL;
    
    p_measure = &p_data_manage->ecg_ppg_s;
    
    // 如果加入进来的测量事件为按键类型的，关闭当前测量，在开启
    if(the_state & MEAT_PPG_BTN_HEART || the_state & MEAT_PPG_BTN_BP 
      || the_state & MEAT_ECG_BTN_HEART || the_state & MEAT_OXYGEN_BTN)
    {
        switch(the_state)
        {
            case MEAT_PPG_BTN_HEART:
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG
                   && (p_measure->ppg_measure_s.app == 1 || p_measure->ppg_measure_s.time == 1))
                {
                    p_measure->ppg_measure_s.btn = 1;
                }
                else
                {
                    p_measure->read_data = 0;
                    p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_PPG;
                    BleThread_PpgOpen();
                    p_measure->ppg_measure_s.init = 1;
                    p_measure->ppg_measure_s.btn = 1;
                    p_data_manage->wear_measure_s.wear_status = 0;
                }
                
                p_measure->oxygen_measure_s.ox_time = 0;
                
                break;
            case MEAT_PPG_BTN_BP:
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG
                   && (p_measure->ppg_measure_s.app == 0 
                      && p_measure->ppg_measure_s.app_bp == 0
                      && p_measure->ppg_measure_s.time == 1))
                {
                    p_measure->ppg_measure_s.btn_bp = 1;   
                    p_measure->ppg_measure_s.calc_bp_init = 1;
                    p_measure->ppg_measure_s.bp_progress = 0;
                }
                else
                {
                    p_measure->read_data = 0;
                    p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_PPG;
                    BleThread_PpgOpen();
                    p_measure->ppg_measure_s.init = 1;
                    p_measure->ppg_measure_s.btn_bp = 1;            
                    p_measure->ppg_measure_s.calc_bp_init = 1;
                    p_measure->ppg_measure_s.bp_progress = 0;
                    p_data_manage->wear_measure_s.wear_status = 0; 
                }
                p_measure->ppg_measure_s.hreart_state = 1;
                
                p_measure->oxygen_measure_s.ox_time = 0;
                
                break;
            case MEAT_ECG_BTN_HEART:
                p_measure->read_data = 0;
                p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_ECG;
                BleThread_EcgOpen();
                p_measure->ecg_measure_s.init = 1;
                p_measure->ecg_measure_s.btn = 1;
            
                p_measure->ppg_measure_s.time = 0;
                
                p_measure->oxygen_measure_s.ox_time = 0;
            
                break; 
            
            case MEAT_OXYGEN_BTN:    //血氧
                p_measure->read_data = 0;
                p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_OXYGEN;
                BleThread_OxygenOpen();
                p_measure->oxygen_measure_s.ox_init = 1;
                p_measure->oxygen_measure_s.ox_btn = 1;
            
                p_measure->ppg_measure_s.init = 1;
            
                p_measure->ppg_measure_s.time = 0;
                
                p_measure->oxygen_measure_s.ox_time = 0;
            
                break;
            
        }
        return 1;
    }
    
    // 如果加入进来的测量事件为app类型，需要先判断是否可以加入
    if(the_state & MEAT_PPG_APP_HEART || the_state & MEAT_PPG_APP_BP 
      || the_state & MEAT_ECG_APP_HEART || the_state & MEAT_OXYGEN_APP)
    {
        switch(the_state)
        {
            case MEAT_PPG_APP_HEART:
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG 
                   || (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG
                       && p_measure->ppg_measure_s.btn_bp == 1)
                   || (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN
                       && p_measure->oxygen_measure_s.ox_btn == 1))
                {   //设备忙碌
                    return 0;
                }
                else if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG 
                        && p_measure->ppg_measure_s.btn_bp == 0 
                        && (p_measure->ppg_measure_s.btn == 1 
                            || p_measure->ppg_measure_s.time == 1))
                {
                    p_measure->ppg_measure_s.app = 1;
                }
                else
                {
                    p_measure->read_data = 0;
                    p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_PPG;
                    BleThread_PpgOpen();
                    p_measure->ppg_measure_s.init = 1;
                    p_measure->ppg_measure_s.app = 1;
                    p_data_manage->wear_measure_s.wear_status = 0;  
                }
                p_measure->ppg_measure_s.hreart_state = 0;
                
                p_measure->oxygen_measure_s.ox_time = 0;
                
                break;   
            case MEAT_PPG_APP_BP: 
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG 
                   || (p_measure->ppg_measure_s.btn == 1 || p_measure->ppg_measure_s.btn_bp == 1)
                   || (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN
                       && p_measure->oxygen_measure_s.ox_btn == 1))
                {   //设备忙碌
                    return 0;
                }
                else if((p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG 
                       && p_measure->ppg_measure_s.btn == 0
                       && p_measure->ppg_measure_s.btn_bp == 0 
                       && p_measure->ppg_measure_s.time == 1))
                {
                    p_measure->ppg_measure_s.app_bp = 1;        
                    p_measure->ppg_measure_s.calc_bp_init = 1;
                    p_measure->ppg_measure_s.bp_progress = 0;
                }
                else
                {
                    p_measure->read_data = 0;
                    p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_PPG;
                    BleThread_PpgOpen();
                    p_measure->ppg_measure_s.init = 1;
                    p_measure->ppg_measure_s.app_bp = 1;        
                    p_measure->ppg_measure_s.calc_bp_init = 1;
                    p_measure->ppg_measure_s.bp_progress = 0;
                    p_data_manage->wear_measure_s.wear_status = 0;  
                }
                
                p_measure->oxygen_measure_s.ox_time = 0;
                
                break; 
            case MEAT_ECG_APP_HEART: 
                if((p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG && p_measure->ecg_measure_s.btn == 1)
                  || (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG 
                      && (p_measure->ppg_measure_s.btn == 1 || p_measure->ppg_measure_s.btn_bp == 1)) 
                  || (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN
                      && p_measure->oxygen_measure_s.ox_btn == 1))
                {   //设备忙碌
                    return 0;
                }
                else
                {
                    p_measure->read_data = 0;
                    p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_ECG;
                    BleThread_EcgOpen();
                    p_measure->ecg_measure_s.init = 1;
                    p_measure->ecg_measure_s.app = 1;
                    p_measure->ecg_measure_s.e_app_calc_cnt = 0;
                    p_data_manage->wear_measure_s.wear_status = 0;  
                }
                p_measure->ppg_measure_s.time = 0;
                
                p_measure->oxygen_measure_s.ox_time = 0;
                
                break; 
            case MEAT_OXYGEN_APP: 
                if((p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG && p_measure->ecg_measure_s.btn == 1)
                  || (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG 
                      && (p_measure->ppg_measure_s.btn == 1 || p_measure->ppg_measure_s.btn_bp == 1))) 
                {   //设备忙碌
                    return 0;
                }
                else if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN
                        && p_measure->oxygen_measure_s.ox_btn == 1)
                {
                    p_measure->oxygen_measure_s.ox_app = 1;
                }
                else
                {
                    p_measure->read_data = 0;
                    p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_OXYGEN;
                    BleThread_OxygenOpen();
                    p_measure->oxygen_measure_s.ox_init = 1;
                    p_measure->oxygen_measure_s.ox_app = 1;
                    
                    p_measure->ppg_measure_s.init = 1;
            
                }                
                p_measure->ppg_measure_s.time = 0;
                
                p_measure->oxygen_measure_s.ox_time = 0;
                
                break;

        }
        return 1;
    }        
    
    // 如果加入进来的测量时间为time类型，需要先判断是否可以加入
    if(the_state & MEAT_PPG_TIME || the_state & MEAT_OXYGEN_TIME)
    {
        switch(the_state)
        {
            case MEAT_PPG_TIME:
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode != MEASURE_MODE_ECG
                   && (p_measure->ppg_measure_s.btn == 0
                       && p_measure->ppg_measure_s.btn_bp == 0
                       && p_measure->ppg_measure_s.app == 0
                       && p_measure->ppg_measure_s.app_bp == 0))
                {   
                    if(p_measure->ppg_measure_s.time != 1 
                      || p_data_manage->ecg_ppg_s.ecg_ppg_mode != MEASURE_MODE_PPG
                      || p_data_manage->ecg_ppg_s.afe4900_is_open != 1)
                    {
                        p_measure->read_data = 0;
                        p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_PPG;
                        BleThread_PpgOpen();
                        p_measure->ppg_measure_s.init = 1;
                        p_measure->ppg_measure_s.time = 1;
                        p_data_manage->wear_measure_s.wear_status = 0;  
                    }
                }
                else
                {   //设备忙碌
                    return 0;
                }
                break;   
            case MEAT_ECG_TIME:
                break; 
            case MEAT_OXYGEN_TIME:
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode != MEASURE_MODE_ECG
                   && (p_measure->ppg_measure_s.btn == 0
                       && p_measure->ppg_measure_s.btn_bp == 0
                       && p_measure->ppg_measure_s.app == 0
                       && p_measure->ppg_measure_s.app_bp == 0)
                   && (p_data_manage->ecg_ppg_s.ecg_ppg_mode != MEASURE_MODE_OXYGEN
                       && p_measure->oxygen_measure_s.ox_app == 0 
                       && p_measure->oxygen_measure_s.ox_btn == 0))
                {   
                    //if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN 
                    //   && p_measure->oxygen_measure_s.ox_time == 1)
                    {
                        p_measure->read_data = 0;
                        p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_OXYGEN;
                        BleThread_OxygenOpen();
                        p_measure->oxygen_measure_s.ox_init = 1;
                        p_measure->oxygen_measure_s.ox_time = 1;
                        
                        p_measure->ppg_measure_s.init = 1;
                        
                        p_data_manage->wear_measure_s.wear_status = 0;  
                    }
                }
                else
                {   //设备忙碌
                    return 0;
                }
                break; 
        }
        return 1;
    }     
    return 0;
}
   
/****************************************************************
** Function name:           Statemachine_DelState
** Descriptions:            测量状态删除
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-13
*****************************************************************/ 
uint8_t Statemachine_DelState(uint16_t the_state)
{
    ecg_ppg_measure_t *p_measure = NULL;
    
    p_measure = &p_data_manage->ecg_ppg_s;
    
    // 如果加入进来的测量事件为按键类型的
    if(the_state & MEAT_PPG_BTN_HEART || the_state & MEAT_PPG_BTN_BP 
      || the_state & MEAT_ECG_BTN_HEART || the_state & MEAT_OXYGEN_BTN)
    {
        switch(the_state)
        {
            case MEAT_PPG_BTN_HEART:
                p_measure->ppg_measure_s.btn = 0;
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG
                   && (p_measure->ppg_measure_s.btn_bp == 0
                       && p_measure->ppg_measure_s.time == 0) )
                {
                    p_measure->read_data = 0;
                    p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                    p_measure->ppg_measure_s.init = 0;
                    BleThread_PeripheralsDevClose();
                }
                break;
            case MEAT_PPG_BTN_BP:
                p_measure->ppg_measure_s.btn_bp = 0;
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG
                   && (p_measure->ppg_measure_s.btn == 0
                       && p_measure->ppg_measure_s.time == 0) )
                {
                    p_measure->read_data = 0;
                    p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                    p_measure->ppg_measure_s.init = 0;
                    BleThread_PeripheralsDevClose();
                }
                break;
            case MEAT_ECG_BTN_HEART:
                p_measure->read_data = 0;
                p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                p_measure->ecg_measure_s.init = 0;
                p_measure->ecg_measure_s.btn = 0;
                BleThread_PeripheralsDevClose();
                break;
            case MEAT_OXYGEN_BTN:
                p_measure->read_data = 0;
                p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                p_measure->oxygen_measure_s.ox_init = 0;
                p_measure->oxygen_measure_s.ox_btn = 0;
                BleThread_PeripheralsDevClose();
        }
        return 1;
    }
    
    // 如果加入进来的测量事件为app类型
    if(the_state & MEAT_PPG_APP_HEART || the_state & MEAT_PPG_APP_BP 
      || the_state & MEAT_ECG_APP_HEART || the_state & MEAT_OXYGEN_APP)
    {
        switch(the_state)
        {
            case MEAT_PPG_APP_HEART:
                p_measure->ppg_measure_s.app = 0;
                
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG)
                {
                    if(p_measure->ppg_measure_s.btn == 0
                       && p_measure->ppg_measure_s.btn_bp == 0
                       && p_measure->ppg_measure_s.time == 0)
                    {
                        p_measure->read_data = 0;
                        p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                        BleThread_PeripheralsDevClose();
                    }
                }
            
                break;   
            case MEAT_PPG_APP_BP:
                p_measure->ppg_measure_s.app_bp = 0;
            
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG)
                {
                    if(p_measure->ppg_measure_s.btn == 0 
                       && p_measure->ppg_measure_s.btn_bp == 0
                       && p_measure->ppg_measure_s.time == 0)
                    {
                        p_measure->read_data = 0;
                        p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                        BleThread_PeripheralsDevClose();
                    }
                }
                break; 
            case MEAT_ECG_APP_HEART:
                p_measure->ecg_measure_s.app = 0;
            
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG )
                {   
                    if(p_measure->ecg_measure_s.btn == 0)
                    {
                        p_measure->read_data = 0;
                        p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                        BleThread_PeripheralsDevClose();
                    }
                }
                break; 
            case MEAT_OXYGEN_APP:
                p_measure->oxygen_measure_s.ox_app = 0;
            
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN )
                {   
                    if(p_measure->oxygen_measure_s.ox_btn == 0)
                    {
                        p_measure->read_data = 0;
                        p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                        BleThread_PeripheralsDevClose();
                    }
                }
                break; 
        }
        return 1;
    }        
    
    // 如果加入进来的测量时间为time类型
    if(the_state & MEAT_PPG_TIME || the_state & MEAT_OXYGEN_TIME)
    {
        switch(the_state)
        {
            case MEAT_PPG_TIME:
                p_measure->ppg_measure_s.time = 0;
                
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG)
                {
                    if(p_measure->ppg_measure_s.btn == 0 && p_measure->ppg_measure_s.btn_bp == 0
                      && p_measure->ppg_measure_s.app == 0 && p_measure->ppg_measure_s.app_bp == 0)
                    {
                        p_measure->read_data = 0;
                        p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                        BleThread_PeripheralsDevClose();
                    }
                }
                break;   
            case MEAT_ECG_TIME:
                break; 
            case MEAT_OXYGEN_TIME:
                p_measure->oxygen_measure_s.ox_time = 0;
                
                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_OXYGEN)
                {
                    if(p_measure->oxygen_measure_s.ox_btn == 0 && p_measure->oxygen_measure_s.ox_app == 0)
                    {
                        p_measure->read_data = 0;
                        p_data_manage->ecg_ppg_s.ecg_ppg_mode = MEASURE_MODE_NULL;
                        BleThread_PeripheralsDevClose();
                    }
                }
                break;
        }
        return 1;
    }     
    return 0;
}


/****************************************************************
** Function name:           Statemachine_BusyStateCode
** Descriptions:            测量状态加入
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2018-11-12
*****************************************************************/ 
uint8_t Statemachine_BusyStateCode(uint16_t the_state)
{
//    uint8_t     busy_code = 0;
//    ecg_ppg_measure_t *p_measure = NULL;
//    
//    p_measure = &p_data_manage->ecg_ppg_s;
//    
//    // 如果加入进来的测量事件为app类型，需要先判断是否可以加入
//    if(the_state & MEAT_PPG_APP_HEART || the_state & MEAT_PPG_APP_BP 
//      || the_state & MEAT_ECG_APP_HEART || the_state & MEAT_PTT_APP)
//    {
//        switch(the_state)
//        {
//            case MEAT_PPG_APP_HEART:           
//                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PTT
//                  || (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG && p_measure->ppg_measure_s.btn == 1))
//                {
//                        busy_code = 0;
//                }
//                else if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG 
//                       || (p_measure->ppg_measure_s.btn_bp == 1))
//                {   //设备忙碌
//                    return 0;
//                }
//                else
//                {
//                    busy_code = 0;
//                }
//                break;   
//            case MEAT_PPG_APP_BP:
//                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PTT)
//                {

//                }
//                else if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG 
//                       || (p_measure->ppg_measure_s.btn == 1 || p_measure->ppg_measure_s.btn_bp == 1))
//                {   //设备忙碌
//                    return 0xFF;
//                }
//                else
//                {
//                }
//                break; 
//            case MEAT_ECG_APP_HEART:
//                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PTT)
//                {
//                }
//                else if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG 
//                       || (p_measure->ppg_measure_s.btn == 1 || p_measure->ppg_measure_s.btn_bp == 1))
//                {   //设备忙碌
//                    return 0xFF;
//                }
//                else
//                {
//                }
//                break; 
//            case MEAT_PTT_APP:
//                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PTT)
//                {
//                }
//                else if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG 
//                       || (p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG && p_measure->ppg_measure_s.time != 1))
//                {   //设备忙碌
//                    return 0xFF;
//                }
//                else
//                {
//                }
//                
//                break; 
//        }
//        return 0;
//    }        
//    
//    // 如果加入进来的测量时间为time类型，需要先判断是否可以加入
//    if(the_state & MEAT_PPG_TIME)
//    {
//        switch(the_state)
//        {
//            case MEAT_PPG_TIME:
//                if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PTT
//                   || p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_PPG)
//                {
//                }
//                else if(p_measure->ecg_measure_s.btn != 1)
//                {   
//                }
//                else
//                {   //设备忙碌
//                    return 0xFF;
//                }
//                break;   
//            case MEAT_ECG_TIME:
//                break; 
//            case MEAT_PTT_TIME:
//                break;
//        }
//        return 0;
//    }     
    return 0;
}


uint8_t Statemachine_IsEcg(void)
{
    // WJ.20190111.比较写成赋值了
    if(p_data_manage->ecg_ppg_s.ecg_ppg_mode == MEASURE_MODE_ECG)
    {   
        return 1;
    }
    return 0;
    
}



