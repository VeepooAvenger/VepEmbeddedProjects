/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
// Board/nrf6310/ble/ble_app_hrs_rtx/main.c
/**
 *
 * @brief Heart Rate Service Sample Application with RTX main file.
 *
 * This file contains the source code for a sample application using RTX and the
 * Heart Rate service (and also Battery and Device Information services).
 * This application uses the @ref srvlib_conn_params module.
 */

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_gap.h"
#include "ble_conn_params.h"
#include "board.h"
#include "sensorsim.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "peer_manager.h"
#include "bsp_button.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "fds.h"
#include "fstorage.h"
#include "ble_conn_state.h"
#include "nrf_drv_clock.h"
#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "app_hid.h"
#include "ble_wechat_service.h"
#include "app_type.h"
#include "uicr_config.h"

#include "gui_thread.h"
#include "app_type.h"

#define IS_SRVC_CHANGED_CHARACT_PRESENT  1                                /**< Include the Service Changed characteristic. If not enabled, the server's database cannot be changed for the lifetime of the device. */

#if (NRF_SD_BLE_API_VERSION == 3)
#define NRF_BLE_MAX_MTU_SIZE            GATT_MTU_SIZE_DEFAULT             /**< MTU size used in the softdevice enabling and to reply to a BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST event. */
#endif

#define CENTRAL_LINK_COUNT               0                                /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT            1                                /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define DEVICE_NAME                      "B36"                           /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME                "NordicSemiconductor"            /**< Manufacturer. Will be passed to Device Information Service. */
#define APP_ADV_INTERVAL                 900//300                         /**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */
#define APP_ADV_SLOW_INTERVAL            3200                             /**< Slow advertising interval (in units of 0.625 ms). The default value corresponds to 2 seconds. */
#define APP_ADV_SLOW_TIMEOUT             180                              /**< The advertising time-out in units of seconds. */

#define APP_ADV_TIMEOUT_IN_SECONDS       10//180                              /**< The advertising time-out in units of seconds. */

#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(40, UNIT_1_25_MS) /**< Minimum acceptable connection interval (0.4 seconds). */
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(65, UNIT_1_25_MS) /**< Maximum acceptable connection interval (0.65 second). */
#define SLAVE_LATENCY                    0                                /**< Slave latency. */
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)  /**< Connection supervisory time-out (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY   5000                             /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY    30000                            /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT     3                                /**< Number of attempts before giving up the connection parameter negotiation. */

#define SEC_PARAM_BOND                   1                                /**< Perform bonding. */
#define SEC_PARAM_MITM                   0                                /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                   0                                /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS               0                                /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES        BLE_GAP_IO_CAPS_NONE             /**< No I/O capabilities. */
#define SEC_PARAM_OOB                    0                                /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE           7                                /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE           16                               /**< Maximum encryption key size. */

#define DEAD_BEEF                        0xDEADBEEF                       /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

//#define OSTIMER_WAIT_FOR_QUEUE           2                                /**< Number of ticks to wait for the timer queue to be ready */

#define APP_FEATURE_NOT_SUPPORTED        BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2        /**< Reply when unsupported features are requested. */

static uint16_t  m_conn_handle = BLE_CONN_HANDLE_INVALID; /**< Handle of the current connection. */
ble_nus_t m_bat;   
ble_clk_t m_clk;

uint32_t reset_value = 0; 

static ble_uuid_t m_adv_uuids[] =                         /**< Universally unique service identifiers. */
{
    {BLE_UUID_WECHAT_SERVICE, BLE_UUID_TYPE_BLE}
};


static SemaphoreHandle_t m_ble_event_ready;  /**< Semaphore raised if there is a new event to be processed in the BLE thread. */

static TaskHandle_t m_ble_stack_thread;      /**< Definition of BLE stack thread. */
static TaskHandle_t m_logger_thread;         /**< Definition of Logger thread. */
static TaskHandle_t m_app_thread;         /**< Definition of app thread. */

static ble_gap_addr_t   ble_gap_address;


uint8_t ble_name[8] = {0x00,};           //蓝牙名称

static void advertising_start(void);

extern system_flag_t system_flag_s;

/****************************************************************
** Function name:           Ble_GetBatSrvAddr
** Descriptions:            获取Bat服务通道
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
ble_nus_t *Ble_GetBatSrvAddr(void)
{
	return &m_bat;
}

/****************************************************************
** Function name:           Ble_GetClkSrvAddr
** Descriptions:            获取Clk服务通道
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
ble_clk_t *Ble_GetClkSrvAddr(void)
{
	return &m_clk;
}

/****************************************************************
** Function name:           Ble_GetClkSrvAddr
** Descriptions:            获取MAC地址
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Ble_GetBleAddr(uint8_t *addr)
{
    memcpy(addr, ble_gap_address.addr, BLE_GAP_ADDR_LEN);
}

/****************************************************************
** Function name:           Ble_DisconnectBle
** Descriptions:            断开蓝牙
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Ble_DisconnectBle(void)
{
    ret_code_t err_code;
    if(m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle,
                                         BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        if (err_code != NRF_ERROR_INVALID_STATE)
        {
            APP_ERROR_CHECK(err_code);
        }
    }
}

/****************************************************************
** Function name:           Ble_DisconnectBle
** Descriptions:            获取链接句柄
** input parameters:        无
** output parameters:       无
** Returned value:          蓝牙连接句柄
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
uint16_t *Ble_GetConnHandle(void)
{
    return &m_conn_handle;
}

/****************************************************************
** Function name:           Ble_IsConnState
** Descriptions:            蓝牙是否处于蓝牙状态
** input parameters:        无
** output parameters:       无
** Returned value:          返回0：断开；1：连接
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
uint8_t Ble_IsConnState(void)
{
    return (m_conn_handle == BLE_CONN_HANDLE_INVALID)?0:1;
}

/****************************************************************
** Function name:           Ble_OpenBle
** Descriptions:            打开蓝牙
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Ble_OpenBle(void)
{
    advertising_start();
}

/****************************************************************
** Function name:           Ble_CloseBle
** Descriptions:            关闭蓝牙
** input parameters:        无
** output parameters:       无
** Returned value:          无
** Created by:              WJ              
** Created Date:            2017-08-12
*****************************************************************/ 
void Ble_CloseBle(void)
{
    ret_code_t err_code;
    if(m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle,
                                         BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        if (err_code != NRF_ERROR_INVALID_STATE)
        {
//            APP_ERROR_CHECK(err_code);
        }
    }
    else
    {
        err_code = sd_ble_gap_adv_stop();
//        APP_ERROR_CHECK(err_code); 
    }
}


/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief Function for handling Peer Manager events.
 *
 * @param[in] p_evt  Peer Manager event.
 */
static void pm_evt_handler(pm_evt_t const * p_evt)
{
    ret_code_t err_code;

    switch (p_evt->evt_id)
    {
        case PM_EVT_BONDED_PEER_CONNECTED:
        {
            NRF_LOG_DEBUG("Connected to previously bonded device\r\n");
#if APP_ANCS_ENABLED
            //SQH.20161014
            Ancs_SetPeerId(p_evt->peer_id);
#endif //APP_ANCS_ENABLED
            err_code = pm_peer_rank_highest(p_evt->peer_id);
            if (err_code != NRF_ERROR_BUSY)
            {
                APP_ERROR_CHECK(err_code);
            }
        } break; // PM_EVT_BONDED_PEER_CONNECTED

        case PM_EVT_CONN_SEC_START:
            break; // PM_EVT_CONN_SEC_START

        case PM_EVT_CONN_SEC_SUCCEEDED:
        {
            NRF_LOG_DEBUG("Link secured. Role: %d. conn_handle: %d, Procedure: %d\r\n",
                                 ble_conn_state_role(p_evt->conn_handle),
                                 p_evt->conn_handle,
                                 p_evt->params.conn_sec_succeeded.procedure);
#if APP_ANCS_ENABLED            
            //SQH.20161014
            Ancs_BleDbDiscoveryStart(p_evt);
            //SQH.20161014 end
#endif  // APP_ANCS_ENABLED 
 
            err_code = pm_peer_rank_highest(p_evt->peer_id);        
            if (err_code != NRF_ERROR_BUSY)
            {
                APP_ERROR_CHECK(err_code);
            }
#if APP_ANCS_ENABLED 
            Ancs_Bonded(p_evt);
#endif  // APP_ANCS_ENABLED     
            
            system_flag_s.bond_flag = 2;     //配对成功标志
            
        } break; // PM_EVT_CONN_SEC_SUCCEEDED

        case PM_EVT_CONN_SEC_FAILED:
        {
            /** In some cases, when securing fails, it can be restarted directly. Sometimes it can
             *  be restarted, but only after changing some Security Parameters. Sometimes, it cannot
             *  be restarted until the link is disconnected and reconnected. Sometimes it is
             *  impossible, to secure the link, or the peer device does not support it. How to
             *  handle this error is highly application dependent. */
            switch (p_evt->params.conn_sec_failed.error)
            {
                case PM_CONN_SEC_ERROR_PIN_OR_KEY_MISSING:
                    // Rebond if one party has lost its keys.
                    err_code = pm_conn_secure(p_evt->conn_handle, true);
                    if (err_code != NRF_ERROR_INVALID_STATE)
                    {
                        APP_ERROR_CHECK(err_code);
                    }
                    break; // PM_CONN_SEC_ERROR_PIN_OR_KEY_MISSING

                default:
                    break;
            }
        } break; // PM_EVT_CONN_SEC_FAILED

        case PM_EVT_CONN_SEC_CONFIG_REQ:
        {
            // Reject pairing request from an already bonded peer.
            //WJ.20170406 解决手机端忽略绑定后再次连接不能绑定的问题
            pm_conn_sec_config_t conn_sec_config = {.allow_repairing = true};//false};
            pm_conn_sec_config_reply(p_evt->conn_handle, &conn_sec_config);
        } break; // PM_EVT_CONN_SEC_CONFIG_REQ

        case PM_EVT_STORAGE_FULL:
        {
            // Run garbage collection on the flash.
            err_code = fds_gc();
            if (err_code == FDS_ERR_BUSY || err_code == FDS_ERR_NO_SPACE_IN_QUEUES)
            {
                // Retry.
            }
            else
            {
                APP_ERROR_CHECK(err_code);
            }
        } break; // PM_EVT_STORAGE_FULL

        case PM_EVT_ERROR_UNEXPECTED:
            // Assert.
            APP_ERROR_CHECK(p_evt->params.error_unexpected.error);
            break; // PM_EVT_ERROR_UNEXPECTED

        case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED:
            break; // PM_EVT_PEER_DATA_UPDATE_SUCCEEDED

        case PM_EVT_PEER_DATA_UPDATE_FAILED:
            // Assert.
            APP_ERROR_CHECK_BOOL(false);
            break; // PM_EVT_PEER_DATA_UPDATE_FAILED

        case PM_EVT_PEER_DELETE_SUCCEEDED:
            break; // PM_EVT_PEER_DELETE_SUCCEEDED

        case PM_EVT_PEER_DELETE_FAILED:
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peer_delete_failed.error);
            break; // PM_EVT_PEER_DELETE_FAILED

        case PM_EVT_PEERS_DELETE_SUCCEEDED:
            advertising_start();
            break; // PM_EVT_PEERS_DELETE_SUCCEEDED

        case PM_EVT_PEERS_DELETE_FAILED:
            // Assert.
            APP_ERROR_CHECK(p_evt->params.peers_delete_failed_evt.error);
            break; // PM_EVT_PEERS_DELETE_FAILED

        case PM_EVT_LOCAL_DB_CACHE_APPLIED:
            break; // PM_EVT_LOCAL_DB_CACHE_APPLIED

        case PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED:
            // The local database has likely changed, send service changed indications.
            pm_local_database_has_changed();
            break; // PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED

        case PM_EVT_SERVICE_CHANGED_IND_SENT:
            break; // PM_EVT_SERVICE_CHANGED_IND_SENT

        case PM_EVT_SERVICE_CHANGED_IND_CONFIRMED:
            break; // PM_EVT_SERVICE_CHANGED_IND_SENT

        default:
            // No implementation needed.
            break;
    }
}

/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
	//设置蓝牙广播名称
    if(ble_name[0] != 0x00)
    {
        err_code = sd_ble_gap_device_name_set(&sec_mode, ble_name, strlen((char *)ble_name));
    }
    else
    {
        err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    }
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_HEART_RATE_SENSOR_HEART_RATE_BELT);
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
                                           
}


/**@brief Function for initializing services that will be used by the application.
 *
 * @details Initialize the Heart Rate, Battery and Device Information services.
 */
static void services_init(void)
{
    uint32_t       err_code;
    ble_nus_init_t bat_init;
    ble_clk_init_t clk_init;

	  //Initialize bat  
    memset(&bat_init, 0, sizeof(bat_init));
    bat_init.data_handler = Protocol_BatDataHandler;
    err_code = ble_nus_init(&m_bat, &bat_init);
    APP_ERROR_CHECK(err_code);
		
      // Initialize clk
    memset(&clk_init, 0, sizeof(clk_init));
    clk_init.data_handler	= Protocol_ClkDataHandler;
    err_code = ble_clk_init(&m_clk, &clk_init);
    APP_ERROR_CHECK(err_code);
    
    //微信服务初始化
    ble_wechat_service_init();
}

/**@brief Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module which
 *          are passed to the application.
 *          @note All this function does is to disconnect. This could have been done by simply
 *                setting the disconnect_on_fail config parameter, but instead we use the event
 *                handler mechanism to demonstrate its use.
 *
 * @param[in]   p_evt   Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;//m_bat.conn_handle; //m_hrs.hrm_handles.cccd_handle;
    cp_init.disconnect_on_fail             = false;  //true：连接参数更新失败导致连接断开
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
//static void sleep_mode_enter(void)
//{
//    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);

//    APP_ERROR_CHECK(err_code);

//    // Prepare wakeup buttons.
//    err_code = bsp_btn_ble_sleep_mode_prepare();
//    APP_ERROR_CHECK(err_code);

//    // Go to system-off mode (this function will not return; wakeup will cause a reset).
//    err_code = sd_power_system_off();
//    APP_ERROR_CHECK(err_code);
//}


/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
//    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
//            NRF_LOG_INFO("Fast Adverstising\r\n");
//            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
//            APP_ERROR_CHECK(err_code);
            break;

        case BLE_ADV_EVT_IDLE:
//            sleep_mode_enter();
            restart_adv_slow();
            break;
//        case BLE_ADV_EVT_WHITELIST_REQUEST:
//        {
//            ble_gap_addr_t whitelist_addrs[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
//            ble_gap_irk_t  whitelist_irks[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
//            uint32_t       addr_cnt = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;
//            uint32_t       irk_cnt  = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;

//            err_code = pm_whitelist_get(whitelist_addrs, &addr_cnt,
//                                        whitelist_irks,  &irk_cnt);
//            APP_ERROR_CHECK(err_code);
//            NRF_LOG_DEBUG("pm_whitelist_get returns %d addr in whitelist and %d irk whitelist\r\n",
//                           addr_cnt,
//                           irk_cnt);

//            // Apply the whitelist.
//            err_code = ble_advertising_whitelist_reply(whitelist_addrs, addr_cnt,
//                                                       whitelist_irks,  irk_cnt);
//            APP_ERROR_CHECK(err_code);
//        }
//        break;
        default:
            break;
    }
}

/**@brief Function for receiving the Application's BLE Stack events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code = 0;
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            
            system_flag_s.bond_flag = 0;
        
            NRF_LOG_INFO("Connected\r\n");
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
#if APP_ANCS_ENABLED
            // Device to connect automatically after open match the binding
//            sec_req_timer_start();    //WJ.2016.11.18
//            Ancs_Start();
#endif        
            App_AncsClearLastCategoryId();  //WJ.20170802.初始化ancs来电标志
            App_AddEvent(APP_BLE_CONNECTED_EVENT); //WJ.2016.11.30
            break; // BLE_GAP_EVT_CONNECTED

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected\r\n");
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
#if APP_ANCS_ENABLED
            Ancs_Disconnected(p_ble_evt);
#endif //APP_ANCS_ENABLED
            App_AddEvent(APP_BLE_DISCONNECTED_EVENT); //WJ.2016.11.30
            break; // BLE_GAP_EVT_DISCONNECTED

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            NRF_LOG_DEBUG("GATT Client Timeout.\r\n");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTC_EVT_TIMEOUT

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            NRF_LOG_DEBUG("GATT Server Timeout.\r\n");
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);    
            break; // BLE_GATTS_EVT_TIMEOUT

        case BLE_EVT_USER_MEM_REQUEST:
            err_code = sd_ble_user_mem_reply(m_conn_handle, NULL);
            APP_ERROR_CHECK(err_code);
            break; // BLE_EVT_USER_MEM_REQUEST

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
        {
            ble_gatts_evt_rw_authorize_request_t  req;
            ble_gatts_rw_authorize_reply_params_t auth_reply;

            req = p_ble_evt->evt.gatts_evt.params.authorize_request;

            if (req.type != BLE_GATTS_AUTHORIZE_TYPE_INVALID)
            {
                if ((req.request.write.op == BLE_GATTS_OP_PREP_WRITE_REQ)     ||
                    (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW) ||
                    (req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL))
                {
                    if (req.type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
                    {
                        auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
                    }
                    else
                    {
                        auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
                    }
                    auth_reply.params.write.gatt_status = APP_FEATURE_NOT_SUPPORTED;
                    err_code = sd_ble_gatts_rw_authorize_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                               &auth_reply);
                    APP_ERROR_CHECK(err_code);
                }
            }
        } break; // BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST

#if (NRF_SD_BLE_API_VERSION == 3)
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
            err_code = sd_ble_gatts_exchange_mtu_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                       NRF_BLE_MAX_MTU_SIZE);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST
#endif        
        default:
            // No implementation needed.
            break;
		
    }
}


/**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the BLE Stack event interrupt handler after a BLE stack
 *          event has been received.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    /** The Connection state module has to be fed BLE events in order to function correctly
     * Remember to call ble_conn_state_on_ble_evt before calling any ble_conns_state_* functions. */
    ble_conn_state_on_ble_evt(p_ble_evt);
    pm_on_ble_evt(p_ble_evt);
    
#if APP_ANCS_ENABLED
    Ancs_BleDbDiscoveryOnBleEvt(p_ble_evt);
    Ancs_OnBleEvt(p_ble_evt);
#endif //APP_ANCS_ENABLED   
       
    ble_conn_params_on_ble_evt(p_ble_evt);

    ble_nus_on_ble_evt(&m_bat, p_ble_evt);
    ble_clk_on_ble_evt(&m_clk, p_ble_evt);
   
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);
    
#if APP_HID_ENABLED
    Hid_OnBleEvt(p_ble_evt);    
#endif  

}


/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in]   sys_evt   System stack event.
 */
static void sys_evt_dispatch(uint32_t sys_evt)
{
    // Dispatch the system event to the fstorage module, where it will be
    // dispatched to the Flash Data Storage (FDS) module.
    fs_sys_event_handler(sys_evt);

    // Dispatch to the Advertising module last, since it will check if there are any
    // pending flash operations in fstorage. Let fstorage process system events first,
    // so that it can report correctly to the Advertising module.
    ble_advertising_on_sys_evt(sys_evt);
}

/**
 * @brief Event handler for new BLE events
 *
 * This function is called from the SoftDevice handler.
 * It is called from interrupt level.
 *
 * @return The returned value is checked in the softdevice_handler module,
 *         using the APP_ERROR_CHECK macro.
 */
static uint32_t ble_new_event_handler(void)
{
    BaseType_t yield_req = pdFALSE;

    // The returned value may be safely ignored, if error is returned it only means that
    // the semaphore is already given (raised).
    UNUSED_VARIABLE(xSemaphoreGiveFromISR(m_ble_event_ready, &yield_req));
    portYIELD_FROM_ISR(yield_req);
    return NRF_SUCCESS;
}

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, ble_new_event_handler);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT, PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
#if (NRF_SD_BLE_API_VERSION == 3)
    ble_enable_params.gatt_enable_params.att_mtu = NRF_BLE_MAX_MTU_SIZE;
#endif
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the Peer Manager initialization.
 *
 * @param[in] erase_bonds  Indicates whether bonding information should be cleared from
 *                         persistent storage during initialization of the Peer Manager.
 */
static void peer_manager_init(bool erase_bonds)
{
    ble_gap_sec_params_t sec_param;
    ret_code_t           err_code;

    err_code = pm_init();
    APP_ERROR_CHECK(err_code);

    if (erase_bonds)
    {
        err_code = pm_peers_delete();
        APP_ERROR_CHECK(err_code);
    }

    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters to be used for all security procedures.
    sec_param.bond           = SEC_PARAM_BOND;
    sec_param.mitm           = SEC_PARAM_MITM;
    sec_param.lesc           = SEC_PARAM_LESC;
    sec_param.keypress       = SEC_PARAM_KEYPRESS;
    sec_param.io_caps        = SEC_PARAM_IO_CAPABILITIES;
    sec_param.oob            = SEC_PARAM_OOB;
    sec_param.min_key_size   = SEC_PARAM_MIN_KEY_SIZE;
    sec_param.max_key_size   = SEC_PARAM_MAX_KEY_SIZE;
    sec_param.kdist_own.enc  = 1;
    sec_param.kdist_own.id   = 1;
    sec_param.kdist_peer.enc = 1;
    sec_param.kdist_peer.id  = 1;

    err_code = pm_sec_params_set(&sec_param);
    APP_ERROR_CHECK(err_code);

    err_code = pm_register(pm_evt_handler);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the Advertising functionality.
 */

void advertising_init(void)
{
    uint32_t               err_code;
    ble_advdata_t          advdata;
    ble_adv_modes_config_t options;
	
//    ble_gap_addr_t   ble_gap_address;
    memset(&ble_gap_address, 0, sizeof(ble_gap_address));

    // Get BLE address.
#if (NRF_SD_BLE_API_VERSION == 3)
    err_code = sd_ble_gap_addr_get(&ble_gap_address);
#else
    err_code = sd_ble_gap_address_get(&ble_gap_address);
#endif
    APP_ERROR_CHECK(err_code);

    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));
    
    
    ble_advdata_manuf_data_t user_data;
    memset(&user_data,0,sizeof(user_data));

    user_data.company_identifier = 0xF8F8;
    user_data.data.p_data = ble_gap_address.addr;
    user_data.data.size   = 6;
    
    advdata.p_manuf_specific_data = &user_data;
    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = m_adv_uuids;

    memset(&options, 0, sizeof(options));
//    options.ble_adv_whitelist_enabled = true;
    options.ble_adv_fast_enabled  = true;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;
//    options.ble_adv_slow_enabled  = true;
//    options.ble_adv_slow_interval = APP_ADV_SLOW_INTERVAL;
//    options.ble_adv_slow_timeout  = APP_ADV_SLOW_TIMEOUT;

    err_code = ble_advertising_init(&advdata, NULL, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    ret_code_t ret;
    
#if APP_ANCS_ENABLED
//    Ancs_SetAdvertising();    
#endif //APP_ANCS_ENABLED
    
    //WJ.2016.12.02
    if(system_flag_s.sign_off != 0x01)   //Broadcast shutdown under no longer open
    {
        ret = ble_advertising_start(BLE_ADV_MODE_FAST);
        APP_ERROR_CHECK(ret);
    }
}

/**@brief Thread for handling the Application's BLE Stack events.
 *
 * @details This thread is responsible for handling BLE Stack events sent from on_ble_evt().
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
static void ble_stack_thread(void * arg)
{
    bool erase_bonds = false;
    
    UNUSED_PARAMETER(arg);

    // Initialize.
    Timers_Init();
 
#if APP_KEY_ENABLED     
    Button_Init();
#endif //APP_KEY_ENABLED     
    
    ble_stack_init();
    peer_manager_init(erase_bonds);

    Product_CheckBleAdvName();//检测蓝牙名称
    gap_params_init();
    services_init();    
#if APP_HID_ENABLED
    Hid_Init();
#endif
    
#if APP_ANCS_ENABLED
    Ancs_Init();
#endif //APP_ANCS_ENABLED


    advertising_init();

    conn_params_init();
		
//    advertising_start();

    // Start execution.
    if (pdPASS != xTaskCreate(App_Thread, "APP_THREAD", 2048, NULL, 1, &m_app_thread))  //8k
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }

    while (1)
    {
        /* Wait for event from SoftDevice */
        while (pdFALSE == xSemaphoreTake(m_ble_event_ready, portMAX_DELAY))
        {
            // Just wait again in the case when INCLUDE_vTaskSuspend is not enabled
        }

        // This function gets events from the SoftDevice and processes them by calling the function
        // registered by softdevice_ble_evt_handler_set during stack initialization.
        // In this code ble_evt_dispatch would be called for every event found.
        intern_softdevice_events_execute();
    }
}

#if NRF_LOG_ENABLED
/**@brief Thread for handling the logger.
 *
 * @details This thread is responsible for processing log entries if logs are deferred.
 *          Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
static void logger_thread(void * arg)
{
    UNUSED_PARAMETER(arg);

    while(1)
    {
        NRF_LOG_FLUSH();

        vTaskSuspend(NULL); // Suspend myself
    }
}
#endif //NRF_LOG_ENABLED

/**@brief A function which is hooked to idle task.
 * @note Idle hook must be enabled in FreeRTOS configuration (configUSE_IDLE_HOOK).
 */
void vApplicationIdleHook( void )
{
     vTaskResume(m_logger_thread); 
}


/**@brief Function for application main entry.
 */
//static uint32_t clock_state = 0;
int main(void)
{
    ret_code_t err_code = 0;   
    
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    
    reset_value = NRF_POWER->RESETREAS;//*((uint32_t *)(0x40000000+ 0x400));
        
    if(reset_value == 0)
    {
        //WJ.20180319.用于表示复位
        reset_value = 0xFE;
    }
    // Do not start any interrupt that uses system functions before system initialisation.
    // The best solution is to start the OS before any other initalisation.

    // Init a semaphore for the BLE thread.
    m_ble_event_ready = xSemaphoreCreateBinary();
    if (NULL == m_ble_event_ready)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }

    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    // Start execution.
    if (pdPASS != xTaskCreate(ble_stack_thread, "BLE", 256, NULL, 5, &m_ble_stack_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }	
    
#if NRF_LOG_ENABLED
    // Start execution.
    if (pdPASS != xTaskCreate(logger_thread, "LOGGER", 256, NULL, 1, &m_logger_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
#endif //NRF_LOG_ENABLED

    /* Activate deep sleep mode */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Start FreeRTOS scheduler.
    vTaskStartScheduler();

    while (true)
    {
        APP_ERROR_HANDLER(NRF_ERROR_FORBIDDEN);
    }
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
//    while(1);
}
