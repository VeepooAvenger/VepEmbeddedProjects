/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  app_ancs.c
** Last modified Date:         2016-11-18
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2016-11-18
** SYS_VERSION:                0.0.1
** Descriptions:               ANCS 通知事件
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

#include "ble_ancs_c.h"
#include "nrf_delay.h"
#include "app_error.h"
#include "nrf.h"
#include "nrf_log.h"
#include <string.h>
#include "FreeRTOS.h"
#include "timers.h"
#include "app_timer.h"
#include "app_ancs.h"
#include "app_type.h"
#include "time_manage.h"
#include "ancs_notify_event.h"
#include "data_manage.h"

#define APP_ANCS_TESTE                   //testting
#define ATTR_DATA_SIZE                   BLE_ANCS_ATTR_DATA_MAX    // < Allocated size for attribute data.

// < Delay after connection until security request is sent, if necessary (ticks).
#define SECURITY_REQUEST_DELAY           1500 //APP_TIMER_TICKS(1500, APP_TIMER_PRESCALER)  

extern uint8_t call_infor_type;

static TimerHandle_t m_sec_req_timer;

static ble_ancs_c_t              m_ancs_c;
 
static uint8_t                         m_ancs_uuid_type;                         // < Store ANCS UUID.  
static ble_ancs_c_evt_notif_t          m_notification_latest;                    // < Local copy to keep track of the newest arriving notifications. 

static ble_db_discovery_t        m_ble_db_discovery; 

static pm_peer_id_t   m_peer_id; 
static pm_peer_id_t   m_whitelist_peers[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];  // < List of peers currently in the whitelist. 
static uint32_t       m_whitelist_peer_cnt;                                 // < Number of peers currently in the whitelist. 

// < Indicates if the whitelist has been changed since last time it has been updated in the Peer Manager. 
static bool           m_is_wl_changed;                                      

static uint8_t ble_phone_flag;                                              //接听标志

#if APP_ANCS_TEST
static void App_Ancs_Test(uint8_t *testData, uint16_t len);
#endif  //APP_ANCS_TEST


/****************************************************************
** Function name:               App_Ancs_BlePhoneGet
** Descriptions:                ancs 蓝牙手机获取
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
uint8_t App_Ancs_BlePhoneGet(void)
{
    return ble_phone_flag;
}

/****************************************************************
** Function name:               App_Ancs_BlePhoneSet
** Descriptions:                ancs 蓝牙手机设置
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_BlePhoneSet(uint8_t phone_flag)
{
    ble_phone_flag = phone_flag;
}

/****************************************************************
** Function name:               App_Ancs_Apple_Notification_Setup
** Descriptions:                
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void App_Ancs_Apple_Notification_Setup(void)
{
    uint32_t err_code;

    nrf_delay_ms(100); 

    err_code = ble_ancs_c_notif_source_notif_enable(&m_ancs_c);
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_data_source_notif_enable(&m_ancs_c);
    APP_ERROR_CHECK(err_code);
}



/****************************************************************
** Function name:               App_Ancs_Notif_Print
** Descriptions:                
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void App_Ancs_Notif_Print(ble_ancs_c_evt_notif_t * p_notif)
{
    uint32_t err_code;

    // 如果挂断电话
	if((p_notif->category_id == BLE_ANCS_CATEGORY_ID_INCOMING_CALL)&&(p_notif->evt_id == BLE_ANCS_EVENT_ID_NOTIFICATION_REMOVED))
	{
        p_data_manage->app_notify_s.call_content_s.mute_call = 0;
        if(p_data_manage->app_notify_s.call_content_s.reject_call == 0)
        {
            p_data_manage->app_notify_s.call_content_s.hang_up_call = 1;
        }
        else
        {
            p_data_manage->app_notify_s.call_content_s.reject_suc = 1;
        }
       
	}
    
    err_code = ble_ancs_c_request_attrs(&m_ancs_c, p_notif);
    APP_ERROR_CHECK(err_code);
}

/****************************************************************
** Function name:               App_Ancs_Notif_Attr_Print
** Descriptions:                
** input parameters:            p_attr：        
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void App_Ancs_Notif_Attr_Print(ble_ancs_c_evt_notif_attr_t * p_attr)
{        
    notif_attr_print(p_attr, NULL);
}


/****************************************************************
** Function name:               App_Ancs_On_Ancs_C_Evt
** Descriptions:                
** input parameters:            p_evt：         
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void App_Ancs_On_Ancs_C_Evt(ble_ancs_c_evt_t * p_evt)
{
    uint32_t err_code = NRF_SUCCESS;

    switch (p_evt->evt_type)
    {
        case BLE_ANCS_C_EVT_DISCOVERY_COMPLETE:
            err_code = ble_ancs_c_handles_assign(&m_ancs_c,p_evt->conn_handle, &p_evt->service);
            APP_ERROR_CHECK(err_code);
            App_Ancs_Apple_Notification_Setup();
            break;

        case BLE_ANCS_C_EVT_NOTIF:
            if(p_evt->notif.category_id == BLE_ANCS_CATEGORY_ID_INCOMING_CALL)
            {
                m_notification_latest = p_evt->notif;
            }
            //YX.2017.12.18.屏蔽推送消息为未接电话时，出现电话事件提醒的问题
            if(p_evt->notif.evt_flags.pre_existing)
            {
                call_infor_type = 0;
                //WJ.20170425.解决重连后提示未读信息.YX.2018.03.05
                return; 
            }
            else
            {
                call_infor_type = 1;
            }
            App_Ancs_Notif_Print(&p_evt->notif);
            break;

        case BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE:
            //YX.2017.12.18.屏蔽推送消息为未接电话时，出现电话事件提醒的问题
            if(p_evt->notif.evt_flags.pre_existing)
            {
                call_infor_type = 0;
                //WJ.20170425.解决重连后提示未读信息.YX.2018.03.05
                return; 
            }
            else
            {
                call_infor_type = 1;
            }
            App_Ancs_Notif_Attr_Print(&p_evt->attr);
            break;

        case BLE_ANCS_C_EVT_DISCOVERY_FAILED:
            break;

        default:
            // No implementation needed.
            break;
    }
}


/****************************************************************
** Function name:               App_Ancs_On_Ble_Evt
** Descriptions:                
** input parameters:            p_ble_evt：
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_On_Ble_Evt(ble_evt_t * p_ble_evt)
{
    ble_ancs_c_on_ble_evt(&m_ancs_c, p_ble_evt);
}

/****************************************************************
** Function name:               App_Ancs_Apple_Notification_Error_Handler
** Descriptions:                ancs 通知错误处理
** input parameters:            nrf_error：       错误
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void App_Ancs_Apple_Notification_Error_Handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/****************************************************************
** Function name:               db_disc_handler
** Descriptions:                
** input parameters:            p_evt： 
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void db_disc_handler(ble_db_discovery_evt_t * p_evt)
{
    ble_ancs_c_on_db_disc_evt(&m_ancs_c, p_evt);
}

/****************************************************************
** Function name:               db_discovery_init
** Descriptions:                db_discovery 初始化
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void db_discovery_init(void)
{
    uint32_t err_code = ble_db_discovery_init(db_disc_handler);

    APP_ERROR_CHECK(err_code);
}

/****************************************************************
** Function name:               App_Ancs_Sec_Req_Timeout_Handler
** Descriptions:                 
** input parameters:            xTimer：
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void App_Ancs_Sec_Req_Timeout_Handler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);
    uint32_t err_code;
    pm_conn_sec_status_t status;
    uint16_t *m_conn_handle = 0;
    
    m_conn_handle = M_Get_Conn_Handle();
    if (*m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        err_code = pm_conn_sec_status_get(*m_conn_handle, &status);
        APP_ERROR_CHECK(err_code);

        // If the link is still not secured by the peer, initiate security procedure.
        if (!status.encrypted)
        {
			p_data_manage->device_trace_param_s.bond_flag = 1;         //开启绑定标志
            err_code = pm_conn_secure(*m_conn_handle, false);
            APP_ERROR_CHECK(err_code);
        }
    }
}

/****************************************************************
** Function name:               App_Ancs_Init
** Descriptions:                ancs初始化
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_Init(void)
{
    uint32_t       err_code;
    
    ble_ancs_c_init_t ancs_init_obj;
    
    db_discovery_init();
    
    err_code = sd_ble_uuid_vs_add(&ble_ancs_base_uuid128, &m_ancs_uuid_type);
    APP_ERROR_CHECK(err_code);
    
    memset(&ancs_init_obj, 0, sizeof(ancs_init_obj));

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER,
                                   m_attr_appID,
                                   sizeof(m_attr_appID));
    APP_ERROR_CHECK(err_code);
    
    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_TITLE,
                                   m_attr_title,
                                   sizeof(m_attr_title));
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_SUBTITLE,
                                   m_attr_subtitle,
                                   sizeof(m_attr_subtitle));
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_MESSAGE,
                                   m_attr_message,
                                   ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_MESSAGE_SIZE,
                                   m_attr_message_size,
                                   sizeof(m_attr_message_size));
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_DATE,
                                   m_attr_date,
                                   sizeof(m_attr_date));
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_POSITIVE_ACTION_LABEL,
                                   m_attr_posaction,
                                   sizeof(m_attr_posaction));
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_NEGATIVE_ACTION_LABEL,
                                   m_attr_negaction,
                                   sizeof(m_attr_negaction));
    APP_ERROR_CHECK(err_code);

    ancs_init_obj.evt_handler   = App_Ancs_On_Ancs_C_Evt;
    ancs_init_obj.error_handler = App_Ancs_Apple_Notification_Error_Handler;

    err_code = ble_ancs_c_init(&m_ancs_c, &ancs_init_obj);
    APP_ERROR_CHECK(err_code);

}

/****************************************************************
** Function name:               App_Ancs_Ble_Db_Discovery_Start
** Descriptions:                
** input parameters:            p_evt： 
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_Ble_Db_Discovery_Start(pm_evt_t const * p_evt)
{
    ret_code_t err_code;
    
    m_peer_id = p_evt->peer_id;  
    memset(&m_ble_db_discovery,0,sizeof(m_ble_db_discovery));    
    err_code  = ble_db_discovery_start(&m_ble_db_discovery, p_evt->conn_handle);    
    APP_ERROR_CHECK(err_code); 
}

/****************************************************************
** Function name:               App_Ancs_Ble_Db_Discovery_On_Ble_Evt
** Descriptions:                
** input parameters:            p_ble_evt：         
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_Ble_Db_Discovery_On_Ble_Evt(ble_evt_t * p_ble_evt)
{
    ble_db_discovery_on_ble_evt(&m_ble_db_discovery, p_ble_evt);
}

/****************************************************************
** Function name:               App_Ancs_Bonded
** Descriptions:                ancs 建立联系
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_Bonded(pm_evt_t const * p_evt)
{
    if (p_evt->params.conn_sec_succeeded.procedure == PM_LINK_SECURED_PROCEDURE_BONDING)
    {
        NRF_LOG_DEBUG("New Bond, add the peer to the whitelist if possible\r\n");
        NRF_LOG_DEBUG("\tm_whitelist_peer_cnt %d, MAX_PEERS_WLIST %d\r\n",
                      m_whitelist_peer_cnt + 1,
                      BLE_GAP_WHITELIST_ADDR_MAX_COUNT);
        if (m_whitelist_peer_cnt < BLE_GAP_WHITELIST_ADDR_MAX_COUNT)
        {
            //bonded to a new peer, add it to the whitelist.
            m_whitelist_peers[m_whitelist_peer_cnt++] = m_peer_id;
            m_is_wl_changed = true;
        }
        //Note: This code will use the older bonded device in the white list and not add any newer bonded to it
        //      You should check on what kind of white list policy your application should use.
    }
}

/****************************************************************
** Function name:               App_Ancs_DisBonded
** Descriptions:                ancs 剥离
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_DisBonded(void)
{
    uint32_t err_code = NRF_SUCCESS; 
    err_code = pm_peers_delete();
    APP_ERROR_CHECK(err_code);
}

/****************************************************************
** Function name:               App_Ancs_Set_Peer_Id
** Descriptions:                设置peer id
** input parameters:            peer_id;        id
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_Set_Peer_Id(pm_peer_id_t peer_id)
{
    m_peer_id = peer_id;    
}


/****************************************************************
** Function name:               App_Ancs_Disconnected
** Descriptions:                断开 ancs
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_Disconnected(ble_evt_t * p_ble_evt)
{
    uint32_t err_code = NRF_SUCCESS;   
    
    if (p_ble_evt->evt.gap_evt.conn_handle == m_ancs_c.conn_handle)
    {
        m_ancs_c.conn_handle = BLE_CONN_HANDLE_INVALID;
    }
    if (m_is_wl_changed)
    {
        // The whitelist has been modified, update it in the Peer Manager.
        err_code = pm_whitelist_set(m_whitelist_peers, m_whitelist_peer_cnt);
        APP_ERROR_CHECK(err_code);

        err_code = pm_device_identities_list_set(m_whitelist_peers, m_whitelist_peer_cnt);
        if (err_code != NRF_ERROR_NOT_SUPPORTED)
        {
            APP_ERROR_CHECK(err_code);
        }

        m_is_wl_changed = false;
    } 
    
    // ANCS断开,初始化
    m_notification_latest.category_id = BLE_ANCS_CATEGORY_ID_OTHER;
//    pm_peers_delete();      //SQH.20161014.删除绑定配对信息        
}

uint8_t App_AncsOperationCalls(void)
{
    uint32_t	err_code = 0;  

    if(m_notification_latest.category_id == BLE_ANCS_CATEGORY_ID_INCOMING_CALL)
    {   
//        m_notification_latest.category_id = BLE_ANCS_CATEGORY_ID_OTHER;	    
        err_code = nrf_ancs_perform_notif_action(&m_ancs_c, m_notification_latest.notif_uid, 1);    
        APP_ERROR_CHECK(err_code); 
    }
  
    return 1;    
}


/****************************************************************
** Function name:               App_Ancs_Peer_List_Get
** Descriptions:                获取 peer 数据
** input parameters:            p_peers：
                                p_size：          数据长度
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
static void App_Ancs_Peer_List_Get(pm_peer_id_t * p_peers, uint32_t * p_size)
{
    pm_peer_id_t peer_id;
    uint32_t     peers_to_copy;

    peers_to_copy = (*p_size < BLE_GAP_WHITELIST_ADDR_MAX_COUNT) ?
                     *p_size : BLE_GAP_WHITELIST_ADDR_MAX_COUNT;

    peer_id = pm_next_peer_id_get(PM_PEER_ID_INVALID);
    *p_size = 0;

    while ((peer_id != PM_PEER_ID_INVALID) && (peers_to_copy--))
    {
        p_peers[(*p_size)++] = peer_id;
        peer_id = pm_next_peer_id_get(peer_id);
    }
}


/****************************************************************
** Function name:               App_Ancs_Advertising_Set
** Descriptions:                ancs 广播设置
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_Advertising_Set(void)
{
    ret_code_t ret;

    memset(m_whitelist_peers, PM_PEER_ID_INVALID, sizeof(m_whitelist_peers));
    m_whitelist_peer_cnt = (sizeof(m_whitelist_peers) / sizeof(pm_peer_id_t));

    App_Ancs_Peer_List_Get(m_whitelist_peers, &m_whitelist_peer_cnt);

    ret = pm_whitelist_set(m_whitelist_peers, m_whitelist_peer_cnt);
    APP_ERROR_CHECK(ret);

    // Setup the device identies list.
    // Some SoftDevices do not support this feature.
    ret = pm_device_identities_list_set(m_whitelist_peers, m_whitelist_peer_cnt);
    if (ret != NRF_ERROR_NOT_SUPPORTED)
    {
        APP_ERROR_CHECK(ret);
    }

    m_is_wl_changed = false;
}

/****************************************************************
** Function name:               App_Ancs_Create_Time
** Descriptions:                ancs 创建定时器
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_Create_Time(void)
{
   m_sec_req_timer = xTimerCreate("SEC",
                                  SECURITY_REQUEST_DELAY,
                                  pdFALSE,
                                  NULL,
                                  App_Ancs_Sec_Req_Timeout_Handler);          //pair 1.5s
}

/****************************************************************
** Function name:               App_Ancs_Start
** Descriptions:                ancs开始
** input parameters:            无
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
void App_Ancs_Start(void)
{
    if (pdPASS != xTimerStart(m_sec_req_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }    
}

/****************************************************************
** Function name:               App_Ancs_Test
** Descriptions:                测设发送数据
** input parameters:            testData：          测试数据指针
                                len：               数据长度
** output parameters:           无
** Returned value:              无
** Created by:                  WJ 
** Created Date:                2016-11-18
**---------------------------------------------------------------
** Modified by:                  
** Modified date:              
** Modified Descriptions:
**---------------------------------------------------------------
****************************************************************/
#if APP_ANCS_TEST 
static void App_Ancs_Test(uint8_t *testData, uint16_t len)
{
   
    uint8_t test_sendData[20] = {0x00, }

    memcpy(test_sendData, testData, (len > 20)?20:len);
    Ble_SendDataViaBat(test_sendData, 20);   

}

#endif //APP_ANCS_ENABLED
