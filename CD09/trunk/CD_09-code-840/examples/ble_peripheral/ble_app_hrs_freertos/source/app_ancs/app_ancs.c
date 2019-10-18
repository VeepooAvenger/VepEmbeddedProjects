/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     app_ancs.c
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
#include "timer_manage.h"
#include "ancs_notify_event.h"

#if APP_ANCS_ENABLED

/*********************************************************************
* Macro Definition
*/
#define APP_ANCS_TESTE      //testting

#define SECURITY_REQUEST_DELAY           1500//APP_TIMER_TICKS(1500, APP_TIMER_PRESCALER)  /**< Delay after connection until security request is sent, if necessary (ticks). */

/*********************************************************************
* Global Variables
*/

static TimerHandle_t m_sec_req_timer;

static ble_ancs_c_t              m_ancs_c;
 
static uint8_t                   m_ancs_uuid_type;                         /**< Store ANCS UUID. */
static ble_ancs_c_evt_notif_t m_notification_latest;                       /**< Local copy to keep track of the newest arriving notifications. */

static ble_ancs_c_evt_notif_t m_notification_latest_cpy;            //用于记录来电的属性

static ble_db_discovery_t        m_ble_db_discovery; 

static pm_peer_id_t   m_peer_id; 
static pm_peer_id_t   m_whitelist_peers[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];  /**< List of peers currently in the whitelist. */
static uint32_t       m_whitelist_peer_cnt;                                 /**< Number of peers currently in the whitelist. */
static bool           m_is_wl_changed;                                      /**< Indicates if the whitelist has been changed since last time it has been updated in the Peer Manager. */


static uint8_t ble_phone_flag;  //接听标志

#if APP_ANCS_TEST
static void Ancs_Test(uint8_t *testData, uint16_t len);
#endif  //APP_ANCS_TEST


extern system_flag_t	  system_flag_s;

/****************************************************************
** Function name:           Ancs_GetBlePhone
** Descriptions:            获取来电通知状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Ancs_GetBlePhone(void)
{
    return ble_phone_flag;
}

/****************************************************************
** Function name:           Ancs_SetBlePhone
** Descriptions:            设置来电通知状态
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Ancs_SetBlePhone(uint8_t phone_flag)
{
    ble_phone_flag = phone_flag;
}

/****************************************************************
** Function name:           Ancs_Apple_Notification_Setup
** Descriptions:            Notification使能
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Ancs_Apple_Notification_Setup(void)
{
    uint32_t err_code;

    nrf_delay_ms(100); 

    err_code = ble_ancs_c_notif_source_notif_enable(&m_ancs_c);
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_data_source_notif_enable(&m_ancs_c);
    APP_ERROR_CHECK(err_code);
}


/****************************************************************
** Function name:           Ancs_NotifPrint
** Descriptions:            通知状态处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Ancs_NotifPrint(ble_ancs_c_evt_notif_t * p_notif)
{
    uint32_t err_code;
    if(p_notif->evt_flags.silent == 1)
    {

    }
    if(p_notif->evt_flags.important == 1)
    {

    }
    if(p_notif->evt_flags.pre_existing == 1)
    {

    }
    if(p_notif->evt_flags.positive_action == 1)
    {    
    }
    if(p_notif->evt_flags.negative_action == 1)
    {

    }
    err_code = ble_ancs_c_request_attrs(&m_ancs_c, &m_notification_latest);
    APP_ERROR_CHECK(err_code);
	
	if(m_notification_latest.category_id == BLE_ANCS_CATEGORY_ID_INCOMING_CALL)
    {
        memcpy(&m_notification_latest_cpy, &m_notification_latest, sizeof(ble_ancs_c_evt_notif_t));
    }
}

/****************************************************************
** Function name:           Ancs_NotifAttrPrint
** Descriptions:            通知属性
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Ancs_NotifAttrPrint(ble_ancs_c_evt_notif_attr_t * p_attr)
{		
    Notify_AttrPrint(p_attr, NULL);
}

/****************************************************************
** Function name:           Ancs_OnAncsCEvt
** Descriptions:            ancs事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Ancs_OnAncsCEvt(ble_ancs_c_evt_t * p_evt)
{
    uint32_t err_code = NRF_SUCCESS;

    switch (p_evt->evt_type)
    {
        case BLE_ANCS_C_EVT_DISCOVERY_COMPLETE:
//            NRF_LOG("Apple Notification Service discovered on the server.\r\n");
            err_code = ble_ancs_c_handles_assign(&m_ancs_c,p_evt->conn_handle, &p_evt->service);
            APP_ERROR_CHECK(err_code);
            Ancs_Apple_Notification_Setup();
            break;

        case BLE_ANCS_C_EVT_NOTIF:
            m_notification_latest = p_evt->notif;    
            //WJ.20170425.解决重连后提示未读信息  
            if(p_evt->notif.evt_flags.pre_existing) return;  
            Ancs_NotifPrint(&m_notification_latest);
            //Ancs_NotifAttrPrint(&p_evt->attr);
            break;

        case BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE:
            //WJ.20170425.解决重连后提示未读信息
            if(p_evt->notif.evt_flags.pre_existing) return;
            Ancs_NotifAttrPrint(&p_evt->attr);
            break;

        case BLE_ANCS_C_EVT_DISCOVERY_FAILED:
            break;

        default:
            // No implementation needed.
            break;
    }
}

/****************************************************************
** Function name:           Ancs_OnBleEvt
** Descriptions:            蓝牙事件处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Ancs_OnBleEvt(ble_evt_t * p_ble_evt)
{
    ble_ancs_c_on_ble_evt(&m_ancs_c, p_ble_evt);
}

/****************************************************************
** Function name:           Ancs_AppleNotificationErrorHandler
** Descriptions:            通知出错处理
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Ancs_AppleNotificationErrorHandler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/****************************************************************
** Function name:           Ancs_OnBleEvt
** Descriptions:            
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Ancs_DbDiscHandler(ble_db_discovery_evt_t * p_evt)
{
    ble_ancs_c_on_db_disc_evt(&m_ancs_c, p_evt);
}

/****************************************************************
** Function name:           Ancs_DbDiscoveryInit
** Descriptions:            ancs 服务发现
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Ancs_DbDiscoveryInit(void)
{
    uint32_t err_code = ble_db_discovery_init(Ancs_DbDiscHandler);

    APP_ERROR_CHECK(err_code);
}


/****************************************************************
** Function name:           Ancs_SecReqTimerOutHandler
** Descriptions:            定时器超时处理，开启绑定
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Ancs_SecReqTimerOutHandler(TimerHandle_t xTimer)
{
    UNUSED_PARAMETER(xTimer);
    uint32_t err_code;
    pm_conn_sec_status_t status;
    uint16_t *m_conn_handle = 0;
    
    m_conn_handle = Ble_GetConnHandle();
    if (*m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        err_code = pm_conn_sec_status_get(*m_conn_handle, &status);
        APP_ERROR_CHECK(err_code);

        // If the link is still not secured by the peer, initiate security procedure.
        if (!status.encrypted)
        {
            system_flag_s.bond_flag = 1;         //开启绑定标志
            err_code = pm_conn_secure(*m_conn_handle, false);
            APP_ERROR_CHECK(err_code);
        }
    }
}

/****************************************************************
** Function name:           Ancs_Init
** Descriptions:            初始化，添加服务
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Ancs_Init(void)
{
    uint32_t       err_code;
    
    ble_ancs_c_init_t ancs_init_obj;
    
    Ancs_DbDiscoveryInit();
    
    err_code = sd_ble_uuid_vs_add(&ble_ancs_base_uuid128, &m_ancs_uuid_type);
    APP_ERROR_CHECK(err_code);
    
    memset(&ancs_init_obj, 0, sizeof(ancs_init_obj));

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER,
                                   m_attr_appID,
                                   ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err_code);
    
    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_TITLE,
                                   m_attr_title,
                                   ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_SUBTITLE,
                                   m_attr_subtitle,
                                   ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_MESSAGE,
                                   m_attr_message,
                                   ATTR_MESSAGE_DATA_SIZE);//ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_MESSAGE_SIZE,
                                   m_attr_message_size,
                                   ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_DATE,
                                   m_attr_date,
                                   ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_POSITIVE_ACTION_LABEL,
                                   m_attr_posaction,
                                   ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err_code);

    err_code = ble_ancs_c_attr_add(&m_ancs_c,
                                   BLE_ANCS_NOTIF_ATTR_ID_NEGATIVE_ACTION_LABEL,
                                   m_attr_negaction,
                                   ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err_code);

    ancs_init_obj.evt_handler   = Ancs_OnAncsCEvt;
    ancs_init_obj.error_handler = Ancs_AppleNotificationErrorHandler;

    err_code = ble_ancs_c_init(&m_ancs_c, &ancs_init_obj);
    APP_ERROR_CHECK(err_code);

}

/****************************************************************
** Function name:           Ancs_BleDbDiscoveryStart
** Descriptions:            发现服务
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Ancs_BleDbDiscoveryStart(pm_evt_t const * p_evt)
{
    ret_code_t err_code;
    
    m_peer_id = p_evt->peer_id;  
    memset(&m_ble_db_discovery,0,sizeof(m_ble_db_discovery));    
    err_code  = ble_db_discovery_start(&m_ble_db_discovery, p_evt->conn_handle);    
    APP_ERROR_CHECK(err_code); 
}

/****************************************************************
** Function name:           Ancs_BleDbDiscoveryOnBleEvt
** Descriptions:            服务事件回复
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Ancs_BleDbDiscoveryOnBleEvt(ble_evt_t * p_ble_evt)
{
    ble_db_discovery_on_ble_evt(&m_ble_db_discovery, p_ble_evt);
}

/****************************************************************
** Function name:           Ancs_Bonded
** Descriptions:            bond device and add device to whitelist
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Ancs_Bonded(pm_evt_t const * p_evt)
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
** Function name:           Ancs_DisBonded
** Descriptions:            disbond device 
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Ancs_DisBonded(void)
{
    uint32_t err_code = NRF_SUCCESS; 
    err_code = pm_peers_delete();
    APP_ERROR_CHECK(err_code);
}

/****************************************************************
** Function name:           Ancs_SetPeerId
** Descriptions:            set peer id
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Ancs_SetPeerId(pm_peer_id_t peer_id)
{
    m_peer_id = peer_id;    
}

/****************************************************************
** Function name:           Ancs_Disconnected
** Descriptions:            ble disconnected, ancs disconnected deal
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Ancs_Disconnected(ble_evt_t * p_ble_evt)
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
            //APP_ERROR_CHECK(err_code);    //YX.20170711.连接APP绑定后，直接退出APP再去设置解除绑定死机问题
        }

        m_is_wl_changed = false;
    } 
//    pm_peers_delete();      //SQH.20161014.删除绑定配对信息        
}

/****************************************************************
** Function name:           Ancs_GetPeerList
** Descriptions:            获取peer列表
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
static void Ancs_GetPeerList(pm_peer_id_t * p_peers, uint32_t * p_size)
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
** Function name:           Ancs_SetAdvertising
** Descriptions:            set advertising , device whitelist
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/
void Ancs_SetAdvertising(void)
{
    ret_code_t ret;

    memset(m_whitelist_peers, PM_PEER_ID_INVALID, sizeof(m_whitelist_peers));
    m_whitelist_peer_cnt = (sizeof(m_whitelist_peers) / sizeof(pm_peer_id_t));

    Ancs_GetPeerList(m_whitelist_peers, &m_whitelist_peer_cnt);

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
** Function name:           Ancs_TimerCreate
** Descriptions:            创建ancs定时器
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Ancs_TimerCreate(void)
{
   m_sec_req_timer = xTimerCreate("SEC",
                             SECURITY_REQUEST_DELAY,
                             pdFALSE,
                             NULL,
                             Ancs_SecReqTimerOutHandler);          //pair 1.5s
   
}

/****************************************************************
** Function name:           Ancs_Start
** Descriptions:            开启定时器
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void Ancs_Start(void)
{
    if (pdPASS != xTimerStart(m_sec_req_timer, OSTIMER_WAIT_FOR_QUEUE))
    {
//        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }       
}


/****************************************************************
** Function name:           Ancs_Test
** Descriptions:            test ,send data to APP by ble
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ             
** Created Date:            2016-11-18
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
#if APP_ANCS_TEST 
static void Ancs_Test(uint8_t *testData, uint16_t len)
{  
    uint8_t test_sendData[20] = {0x00, }

    memcpy(test_sendData, testData, (len > 20)?20:len);
    Ble_SendDataViaBat(test_sendData, 20);   

}
#endif  //APP_ANCS_TEST

 
#define CALL_ANSWERI        0
#define CALL_HANG_UP        1

//控制来电
/****************************************************************
** Function name:           Ancs_OperationCalls
** Descriptions:            来电拒接操作
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
uint8_t Ancs_OperationCalls(uint8_t type)
{
    uint32_t	err_code = 0;  
    uint8_t    call_sendData[20] = {0xC1, 0x02, 0x00,};

    Ble_SendDataViaBat(call_sendData, 20);     
    
    if(m_notification_latest_cpy.category_id != BLE_ANCS_CATEGORY_ID_INCOMING_CALL)
    {        
        return 0;
    }
  
    err_code = nrf_ancs_perform_notif_action(&m_ancs_c, m_notification_latest_cpy.notif_uid, CALL_HANG_UP);    
    APP_ERROR_CHECK(err_code);   
    
    return 1;    
}

/****************************************************************
** Function name:           App_AncsClearLastCategoryId
** Descriptions:            清除消息ID记录
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              SQH              
** Created Date:            2016-10-20
**----------------------------------------------------------------
** Modified by:
** Modified date:
**----------------------------------------------------------------
*****************************************************************/ 
void App_AncsClearLastCategoryId(void)
{
    memset(&m_notification_latest_cpy, 0x00, sizeof(m_notification_latest_cpy));
}

#endif //APP_ANCS_ENABLED
