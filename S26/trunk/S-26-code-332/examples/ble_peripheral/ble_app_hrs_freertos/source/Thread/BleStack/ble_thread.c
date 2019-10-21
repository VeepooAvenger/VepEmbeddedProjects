/*************************************Copyright(c)***************************************************
**                                     Veepoo Tech
**                  
**                               COPYRIGHT(C)  2016-2019
**--------------------------------------FileInfo----------------------------------------------------
** FileName:                BleStack_Thread.c
** Last Modify Data:        2017-08-04
** Last SYS_VERSION:        XXXXXXXXX
** Descriptions:                     
**---------------------------------------------------------------------------------------------------
** CreatedBy:               GT
** CreateData:              2017-06-xx
** SystemVersion:             
** Description:             This Thread  is  handle  ble server rigister and  ble evt   
**                           
**                          
**---------------------------------------------------------------------------------------------------
** Modify By:                       
** Modify Data:
** System Version:
** Modify Description:    

******************************************************************************************************/
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
#include "bsp_btn_ble.h"
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

#include "ble_wechat_service.h"
#include "app_type.h"
#include "uicr_config.h"
#include "algorithm_thread.h"
#include "communication_thread.h"
#include "gui_thread.h"
#include "cpu_task_data_type.h"
#include "GSensor.h"
#include "ble_thread.h"
#include "data_manage.h"
#include "log.h"
#include "Api_Flash.h"
#include "AFE4900_entry.h"
#include "battery_adc.h"
#include "no_weardetect.h"
#include "app_hid.h"
#include "signal_wear_detection_afe4410.h"
#include "event_manage.h"
#include "movement_patterns.h"
#include "app_config_data.h"
#include "util.h"
#include "posture_get.h"
#include "ble_disconnect_remind.h"

#include "Dev_SN_Check.h"
extern uint32_t reset_value;               //复位代码
extern uint32_t reset_time_minutes;        //复位时间分钟
static uint16_t  m_conn_handle = BLE_CONN_HANDLE_INVALID; /**< Handle of the current connection. */

static ble_uuid_t m_adv_uuids[] =                         /**< Universally unique service identifiers. */
{
    {BLE_UUID_WECHAT_SERVICE, BLE_UUID_TYPE_BLE}
};

static ble_gap_addr_t   ble_gap_address;

static void advertising_start(void);

static bthread_res_t  st_ble_res;

static uint8_t ble_adv_flg = 0;
static uint8_t ble_adv_cnt = 0;
extern uint8_t heart_init_cause;
extern uint8_t heart_calib_init_cause;

uint8_t m_ble_name[10] = {0};       // 蓝牙名称
uint8_t m_ble_adv_init = 0;         // 是否需要重置广播

/************************************************************************
** Function name:        vApplicationMallocFailedHook
** Descriptions:         malloc failed 处理
** input parameter:      无  
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
******************************************************************************************************/

void vApplicationMallocFailedHook( void)
{
}

/*******************************************************************************************************
** Function name:        M_DisconnectBle
** Descriptions:         ble失能 
** input parameter:      无  
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
******************************************************************************************************/
void get_ble_addr(uint8_t *addr)
{
    memcpy(addr, ble_gap_address.addr, BLE_GAP_ADDR_LEN);
}

/*******************************************************************************************************
** Function name:        M_Get_Conn_Handle
** Descriptions:         得到连接句柄  
** input parameter:      无  
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
******************************************************************************************************/

uint16_t *M_Get_Conn_Handle(void)
{
    return &m_conn_handle;
}
/*******************************************************************************************************
** Function name:        assert_nrf_callback
** Descriptions:         鉴别回调函数   
** input parameter:      p_file_name : 数据指针  
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/*******************************************************************************************************
** Function name:        pm_evt_handler
** Descriptions:         pm  事件处理   
** input parameter:      const *p_evt : 数据指针  
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/

static void pm_evt_handler(pm_evt_t const * p_evt)
{
    ret_code_t err_code;

    switch (p_evt->evt_id)
    {
        case PM_EVT_BONDED_PEER_CONNECTED:
        {
            NRF_LOG_DEBUG("Connected to previously bonded device\r\n");
            //SQH.20161014
            App_Ancs_Set_Peer_Id(p_evt->peer_id);
            err_code = pm_peer_rank_highest(p_evt->peer_id);
            if (err_code != NRF_ERROR_BUSY)
            {
                APP_ERROR_CHECK(err_code);
            }
        } break; // PM_EVT_BONDED_PEER_CONNECTED

        case PM_EVT_CONN_SEC_SUCCEEDED:
        {
            NRF_LOG_DEBUG("Link secured. Role: %d. conn_handle: %d, Procedure: %d\r\n",
                           ble_conn_state_role(p_evt->conn_handle),
                           p_evt->conn_handle,
                           p_evt->params.conn_sec_succeeded.procedure);
            //SQH.20161014
            App_Ancs_Ble_Db_Discovery_Start(p_evt);
            //SQH.20161014 end
 
            err_code = pm_peer_rank_highest(p_evt->peer_id);        
            if (err_code != NRF_ERROR_BUSY)
            {
                APP_ERROR_CHECK(err_code);
            }
            App_Ancs_Bonded(p_evt);
			
			p_data_manage->device_trace_param_s.bond_flag = 2;     //配对成功标志
        } break; // PM_EVT_CONN_SEC_SUCCEEDED

        case PM_EVT_CONN_SEC_FAILED:
        {
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

        case PM_EVT_PEER_DATA_UPDATE_FAILED:
            // Assert.
            APP_ERROR_CHECK_BOOL(false);
            break; // PM_EVT_PEER_DATA_UPDATE_FAILED

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

        case PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED:
            // The local database has likely changed, send service changed indications.
            pm_local_database_has_changed();
            break; // PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED

        default:
            // No implementation needed.
            break;
    }
}

/*******************************************************************************************************
** Function name:        gap_params_init
** Descriptions:         gap  参数初始化   
** input parameter:      无      
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    //设置蓝牙广播名称
    //err_code = sd_ble_gap_device_name_set(&sec_mode, 
    //                                      p_flash_param->bluetooth_name_s.name, 
    //                                      p_flash_param->bluetooth_name_s.length);
 
	//WJ.20181022.设置蓝牙广播名称
    if(m_ble_name[0] != 0x00)
    {
        err_code = sd_ble_gap_device_name_set(&sec_mode, m_ble_name, strlen((char *)m_ble_name));
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

/*******************************************************************************************************
** Function name:        services_init
** Descriptions:         服务初始化  
** input parameter:      无      
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void services_init(void)
{
    uint32_t       err_code;
    ble_nus_init_t bat_init;
    ble_clk_init_t clk_init;

      //Initialize bat  
    memset(&bat_init, 0, sizeof(bat_init));
    bat_init.data_handler = Protocol_Bat_Data_Handler;
    err_code = ble_nus_init(&st_ble_res.m_ble_nus, &bat_init);
    APP_ERROR_CHECK(err_code);
        
      // Initialize clk
    memset(&clk_init, 0, sizeof(clk_init));
    clk_init.data_handler    = Protocol_Clk_Data_Handler;
    err_code = ble_clk_init(&st_ble_res.m_ble_clk, &clk_init);
    APP_ERROR_CHECK(err_code);
    
    //微信服务初始化
    ble_wechat_service_init();
}

/*******************************************************************************************************
** Function name:        on_conn_params_evt
** Descriptions:         连接事件
** input parameter:      p_evt:连接事件参数   
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/

static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

/*******************************************************************************************************
** Function name:        on_conn_params_evt
** Descriptions:         连接参数处理   
** input parameter:      nrf_error:  错误检查     
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/

static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/*******************************************************************************************************
** Function name:        conn_params_init
** Descriptions:         连接参数初始化   
** input parameter:      无      
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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

/*******************************************************************************************************
** Function name:        on_adv_evt
** Descriptions:         广播事件处理    
** input parameter:      ble_adv_evt:广播事件指针   
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_IDLE:
            restart_adv_slow();
            break;
        default:
            break;
    }
}

/*******************************************************************************************************
** Function name:        on_ble_evt
** Descriptions:         蓝牙协议栈事件处理    
** input parameter:      p_ble_evt:蓝牙协议栈事件数据指针      
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
********************************************************************************************************/
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code = 0;
   
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
			p_data_manage->device_trace_param_s.bond_flag = 0;
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            BleThread_ConnectDevice();
            break; // BLE_GAP_EVT_CONNECTED

        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            App_Ancs_Disconnected(p_ble_evt);
            BThread_DisconnectDevice(false);
            break; // BLE_GAP_EVT_DISCONNECTED

        case BLE_GATTC_EVT_TIMEOUT:
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTC_EVT_TIMEOUT

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
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

/*******************************************************************************************************
** Function name:        ble_evt_dispatch
** Descriptions:         蓝牙事件分发函数    
** input parameter:      p_ble_evt:蓝牙事件数据指针
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    /** The Connection state module has to be fed BLE events in order to function correctly
     * Remember to call ble_conn_state_on_ble_evt before calling any ble_conns_state_* functions. */
    ble_conn_state_on_ble_evt(p_ble_evt);
    pm_on_ble_evt(p_ble_evt);
    
    App_Ancs_Ble_Db_Discovery_On_Ble_Evt(p_ble_evt);
    App_Ancs_On_Ble_Evt(p_ble_evt);
    
    ble_conn_params_on_ble_evt(p_ble_evt);

    ble_nus_on_ble_evt(&st_ble_res.m_ble_nus, p_ble_evt);
    ble_clk_on_ble_evt(&st_ble_res.m_ble_clk, p_ble_evt);
   
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);
	
    Hid_OnBleEvt(p_ble_evt);    
}
/*******************************************************************************************************
** Function name:        sys_evt_dispatch
** Descriptions:         系统事件回调函数       
** input parameter:      sys_evt:事件数据指针  
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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


/*******************************************************************************************************
** Function name:        ble_stack_init
** Descriptions:         蓝牙协议栈初始化       
** input parameter:      无
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void ble_stack_init(void)
{
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;
    
    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

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

/*******************************************************************************************************
** Function name:        peer_manager_init
** Descriptions:         管理初始化
** input parameter:      无 
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
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
/*******************************************************************************************************
** Function name:        advertising_init
** Descriptions:         广播初始化
** input parameter:      无 
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void advertising_init(void)
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
//  options.ble_adv_whitelist_enabled = true;
    options.ble_adv_fast_enabled  = true;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;
//  options.ble_adv_slow_enabled  = true;
//  options.ble_adv_slow_interval = APP_ADV_SLOW_INTERVAL;
//  options.ble_adv_slow_timeout  = APP_ADV_SLOW_TIMEOUT;

    err_code = ble_advertising_init(&advdata, NULL, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
}

/*******************************************************************************************************
** Function name:        advertising_init
** Descriptions:         广播开始
** input parameter:      无 
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void advertising_start(void)
{
    ret_code_t ret;
    
//  App_Ancs_Advertising_Set();    
    
    ret = ble_advertising_start(BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(ret);
}

/*******************************************************************************************************
** Function name:        Algorithm_AFE4404Open
** Descriptions:         AFE4404开灯函数
** input parameter:      无     
** Returned Value:       无 
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Thread_AFE4404HeartOpenTask(void)
{
    if(Battery_Get_Grade() == 0)
    {
        //低电不开启测量
        return;
    }
    
    Afe4900_OpenMode(MEASURE_MODE_PPG1);
    
    // GT 2017-08-09 !注意:这里需要对结果参数全部初始化
    DataManage_AFE4404PoolInit();		
				
    //非黑人模式，才开启佩戴算法
    if(p_flash_param->dev_config_s.led_gear == 0)
    {
        p_data_manage->wear_measure_s.init = 1;
    }
    p_data_manage->wear_measure_s.wear_status = 0;
    
}

/*******************************************************************************************************
** Function name:        Algorithm_AFE4404Open
** Descriptions:         AFE4404开灯函数
** input parameter:      无     
** Returned Value:       无 
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Thread_AFE4404BoOpenTask(void)
{
    if(Battery_Get_Grade() == 0)
    {
        //低电不开启测量
        return;
    }
    
    Afe4900_OpenMode(MEASURE_MODE_PPG2);
    
    // GT 2017-08-09 !注意:这里需要对结果参数全部初始化
    DataManage_AFE4404PoolInit();
	
    //非黑人模式，才开启佩戴算法
    if(p_flash_param->dev_config_s.led_gear == 0)
    {
        p_data_manage->wear_measure_s.init = 1;
    }
    p_data_manage->wear_measure_s.wear_status = 0;
     
    // 需要对当前血氧数据佩戴结果清零
    p_data_manage->oxygen_attr.wear = 0;
    
    // 清除呼吸暂停的5 分钟血氧修正标志位
    p_data_manage->oxygen_attr.spo2_modif_flag = 0;
    
    //新血氧校准，进度条参数初始化
    p_data_manage->blood_oxygen_measure_s.cal_state = 1;
    p_data_manage->blood_oxygen_measure_s.cal_progress = 0;
    
    //WJ.20180629.自动血氧测量数据采集开始
    if(p_data_manage->blood_oxygen_measure_s.time == 1)
    {
        Test_OxygenStart();
    } 
}

/*******************************************************************************************************
** Function name:        Algorithm_AFE4404Open
** Descriptions:         AFE4404开灯函数
** input parameter:      无     
** Returned Value:       无 
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Thread_AFE4404FatigueOpenTask(void)
{
    if(Battery_Get_Grade() == 0)
    {
        //低电不开启测量
        return;
    }
    
    //AFE4404_FatigueOpen();    
    Afe4900_OpenMode(MEASURE_MODE_PPG1);
			
    // GT 2017-08-09 !注意:这里需要对结果参数全部初始化
    DataManage_AFE4404PoolInit();
	
    //非黑人模式，才开启佩戴算法
    if(p_flash_param->dev_config_s.led_gear == 0)
    {
        p_data_manage->wear_measure_s.init = 1;
    }
    p_data_manage->wear_measure_s.wear_status = 0;
				  
}

/*******************************************************************************************************
** Function name:        Task_AFE4404Close
** Descriptions:         AFE4404关灯函数
** input parameter:      无     
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
*********************************************************************************************************/
static void Thread_AFE4404CloseTask(void)
{
	p_data_manage->upload = UPLOAD_ADC_NULL_EVENT;
	
    if(p_flash_param->dev_config_s.led_gear == 0 && p_data_manage->wear_measure_s.wear_status == 1)
    {
        if(p_data_manage->heart_measure_s.app == 1)
        {
            p_data_manage->heart_measure_s.app = 0;
            Ble_SendBatMulData(2,0xD0,0x01);
        }
        else if(p_data_manage->blood_oxygen_measure_s.app == 1)
        {
            p_data_manage->blood_oxygen_measure_s.app = 0;
            Ble_SendBatMulData(4,0x80,0x01,0x00,0x01);
        }
        else if(p_data_manage->fatigue_measure_s.app == 1)
        {
            p_data_manage->fatigue_measure_s.app = 0;
            Ble_SendBatMulData(4,0x81,0x01,0x00,0x64);
        }
        else if(p_data_manage->respiratory_rate_s.app == 1)
        {
            p_data_manage->respiratory_rate_s.app = 0;
            Ble_SendBatMulData(5,0x82,0x01,0x00,0x00,0x01);
        }
        else if(p_data_manage->bp_measure_s.bp_app == 1)
        {
            p_data_manage->bp_measure_s.bp_app = 0;
            Ble_SendBatMulData(6, 0x90, 30, 20, 0x00, 0, 1);
        }
    }
    else
    {
        if(p_data_manage->heart_measure_s.app == 1)
        {
            p_data_manage->heart_measure_s.app = 0;
            Ble_SendBatMulData(6,0xD0,0x00,0x00,0x00,0x00,0x04);
        }
        else if(p_data_manage->blood_oxygen_measure_s.app == 1)
        {
            p_data_manage->blood_oxygen_measure_s.app = 0;
            Ble_SendBatMulData(3,0x80,0x02,0x02);
        }
        else if(p_data_manage->fatigue_measure_s.app == 1)
        {
            p_data_manage->fatigue_measure_s.app = 0;
            Ble_SendBatMulData(3,0x81,0x01,0x04);
        }
        else if(p_data_manage->respiratory_rate_s.app == 1)
        {
            p_data_manage->respiratory_rate_s.app = 0;
            Ble_SendBatMulData(3,0x82,0x01,0x04);
        }       
        else if(p_data_manage->bp_measure_s.bp_app == 1)
        {
            p_data_manage->bp_measure_s.bp_app = 0;
            Ble_SendBatMulData(4, 0x90, 30, 20, 0, 0, 1, 0, 0, 0, 0, 0);
        }
    }
    
    Afe4900_Close();
}

/*******************************************************************************************************
** Function name:        BThread_ConnectTask
** Descriptions:         BT连接标志置位
** input parameter:      无
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void Thread_DevConnectTask(void)
{
    p_data_manage->device_trace_param_s.ble_connect = 1;
    //Remind_BleDisconnectRemove();
}
/*******************************************************************************************************
** Function name:        BThread_DisconnectTask
** Descriptions:         BT连接标志清零
** input parameter:      无
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void Thread_AdvertisingStartTask(void);
static void Thread_DevDisconnectTask(void)
{    
    CPU_EVENT_TYPE  m_cpu_event_type = CPU_EVENT_TASK_IDEL;
    p_data_manage->device_trace_param_s.ble_connect = 0;
    p_data_manage->device_trace_param_s.code_passed = 0;
    
    // 蓝牙断开处理事务
    p_data_manage->heart_measure_s.app = 0;
    p_data_manage->blood_oxygen_measure_s.app = 0;
    p_data_manage->fatigue_measure_s.app = 0;
    p_data_manage->respiratory_rate_s.app = 0;
    
    p_data_manage->bp_measure_s.bp_app = 0;
    
    p_data_manage->upload = UPLOAD_ADC_NULL_EVENT;
    p_flash_param->update_uimode = 0;

    p_data_manage->device_trace_param_s.font_test_on = 0;
    
    p_data_manage->respiratory_rate_s.mode = 0;
    
    // 工厂模式处理	
    if(p_data_manage->device_trace_param_s.factory_mode == 1)
    {
        MotorDelEvent();
        p_data_manage->device_trace_param_s.factory_mode = 0;
        GuiThread_AddEvent(CPU_EVENT_TASK_ALL_EXIT);	    
    }
    
    // 如果处于拍照模式,断开蓝牙需要推出拍照模式
    m_cpu_event_type = GuiThread_GetEventType();
    if(m_cpu_event_type == CPU_EVENT_TASK_CAMERA)
    {
        GuiThread_ClearEvent(CPU_EVENT_TASK_CAMERA);    
    }  
    else if(m_cpu_event_type == CPU_EVENT_TASK_CALL)
    {
        MotorDelEvent();
        GuiThread_ClearEvent(CPU_EVENT_TASK_CALL); 
    }
    EventManage_ClearEvent(CPU_EVENT_TASK_CAMERA);
    EventManage_ClearEvent(CPU_EVENT_TASK_CALL);
    
    //YX.20180408.蓝牙断连提醒,开机状态下，提醒开关开启   
    //if(p_data_manage->device_trace_param_s.sign_on == 1)
    //{
    //    Remind_BleDisconnectDisPlay(p_flash_param->dev_config_s.ble_disconnect_enable, 1, 
    //                                p_data_manage->device_trace_param_s.ble_connect);
    //}

    if(m_ble_adv_init == 1)
    {
        // !注意:这里不要对返回值做判断处理
        sd_ble_gap_adv_stop();
        
        advertising_init();
        m_ble_adv_init = 0;
        
        if(p_data_manage->device_trace_param_s.sign_on == 1)
        {
            Thread_AdvertisingStartTask();
        }
    }
}

static void Thread_AdvertisingStartTask(void)
{
    uint32_t ret = 0;
    
    // 等待FLASAH协议栈事件处理完成,否则广播开启失败 Add by GT 2017-07-22
    while(!fs_queue_is_empty())
    {
        intern_softdevice_events_execute();
    }
    //YX.20180619.优化蓝牙不广播的问题
    ret = ble_advertising_start(BLE_ADV_MODE_FAST);
    if(ret != NRF_SUCCESS)
    {
        //蓝牙开启失败，延时后继续开始广播
        ble_adv_flg = 1;
    }
//    if(p_data_manage->device_trace_param_s.sign_on == 1)
//    {
        //ret = ble_advertising_start(BLE_ADV_MODE_FAST);
//      APP_ERROR_CHECK(ret);
//    }

}

static void Thread_AdvertisingStopTask(void)
{
//    uint32_t err_code = 0;
    
    if(m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {                                                   
//        err_code = 
        sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
//        APP_ERROR_CHECK(err_code);
    }
    else
    {
        // !注意:这里不要对返回值做判断处理
        (void)sd_ble_gap_adv_stop();
        //APP_ERROR_CHECK(err_code);
    }
}

static void Data_ClearParamInit(void)
{
     // 零点换块并且清除记步 Add by GT 2017-07-04
    All_StepCln();
    Work_MeasrueCln();
    
    // 清除记步的一些内存信息 Modify by GT 2017-07-22
    memset(&p_data_manage->pedometer, 0, sizeof(history_pedometer_param));
}

static void Thread_InitZeroTimeTask(void)
{
    Data_ClearParamInit();
    Api_FlashDailyJumpBlock();
    Ped_SetSportsInfo(0, 0, 0);
	p_flash_param->Remind_flag = 0;
}

static void Thread_SaveSystemTimeTask(void)
{
    Api_SaveSystemTime(clock->year,clock->month,clock->day,clock->hour,clock->minutes);
}

static void Thread_SaveDailyDataTask(void)
{
    static uint8_t reste_flag = 0;
    daily_data_attr_t *p_daily_data = DM_Get_NewestDailyData();
	
    // 需要对心率值做判断,如果心率值小于30,存储佩戴结果
    if(p_daily_data->heart < 30)
    {
        p_daily_data->heart = p_data_manage->no_wear_measure_s.no_wear_result;
    }

	// JSH.20181123.充电，心率佩戴的值等于6
	if(Battery_Get_Status() != BATTERY_POWER_NORMAL)
	{
		p_daily_data->heart = 6;
	}
        
    //获取姿态以及次数，获取后初始化姿态统计
    Posture_StatisticalGet(&p_daily_data->gesture, &p_daily_data->gesture_cnt);
    p_data_manage->no_wear_measure_s.gesture = p_daily_data->gesture;
    Posture_StatisticalInit();

    // WJ.20181217.血压
    if(p_flash_param->dev_config_s.blood_detect_enable == 0x01 
       && p_flash_param->bp_func_state == 0x01)
    {    
        p_daily_data->blood_high = p_data_manage->bp_cache_s.cache_bp_sp;
        p_daily_data->blood_low = p_data_manage->bp_cache_s.cache_bp_dp;     
        p_data_manage->bp_cache_s.cache_bp_sp = 0;
        p_data_manage->bp_cache_s.cache_bp_dp = 0;
    }    
    if(p_flash_param->blood_oxygen_config_s.enable == 1 && clock->hour >= 7)
    {
        if(p_daily_data->ox_value == 0)
        {   //WJ.20190619.增加尝长时间未佩戴
            if(Battery_Get_Status() != BATTERY_POWER_NORMAL 
              || ((Get_NoWearStation() >= 2 && Get_NoWearStation() != 5)))
            {   
                p_daily_data->ox_value = OX_SPECIAL_VALUES_WEAR; // 充电未佩戴\长时间未佩戴
            }
            else
            {
                p_daily_data->ox_value = OX_SPECIAL_VALUES_INT; // 测量被打断
            }
        }
        p_daily_data->ox_value = p_data_manage->ox_cache_s;
    }
    //初始化
    p_data_manage->ox_cache_s = 0;
    
    if(!reste_flag)
    {
        reste_flag = 1;
        p_daily_data->resv = reset_value;
        //p_daily_data->resv[1] = reset_time_minutes;
        reset_value = 0;
        reset_time_minutes = 0;
    }
    
    Api_SaveDailyAttrData(p_daily_data);
}

static void Thread_SaveOxygenDataTask(void)
{
    uint8_t wear_tmp = 0;
    //const wear_detection_info_t *p_wear_info = NULL;
    scatter_heart_t  m_scatter_heart = {0};
    //p_wear_info = Get_Wear_Detection_Info();
    
    // 存储血氧数据
    DataManage_ProcessOxygenAttrData();
    
    // 获取记步、运动量的值
    p_data_manage->oxygen_attr.sport_amount = p_data_manage->pedometer.last_minute_sport;
    p_data_manage->oxygen_attr.step         = p_data_manage->pedometer.last_minute_step;
    //YX.20180111.佩戴状态是在佩戴算法里面获取的，5次未通过才算是佩戴未通过，不是实时的佩戴状态
    //p_data_manage->oxygen_attr.wear         = p_wear_info->wear_station;
    wear_tmp = p_data_manage->oxygen_attr.wear;
    
    // 存储散点图心率值
    memcpy(&m_scatter_heart, &p_data_manage->scatter_heart_param_s, 12);
    memset(&p_data_manage->scatter_heart_param_s, 0, sizeof(p_data_manage->scatter_heart_param_s));

    //YX.20180511.增加5 分钟血氧修正标志位
	p_data_manage->oxygen_attr.spo2_modif_flag = 0;
    if(/*p_data_manage->oxygen_attr.spo2_modif_flag != 0
       && */p_data_manage->oxygen_attr.minute%5 == 4)
    {
        Api_ModifyOahsFlag(p_data_manage->oxygen_attr.spo2_modif_flag, p_data_manage->oxygen_resut);
        
        p_data_manage->oxygen_attr.oxygen = p_data_manage->oxygen_resut[4];
    }
    else
    {
        p_data_manage->oxygen_attr.oxygen = 0xFF;
    }
    
    if(p_data_manage->oxygen_attr.spo2_modif_flag == 0)
    {
        p_data_manage->oxygen_attr.spo2_modif_flag = 3;
    }
    Api_SaveOxygenAttrData(&p_data_manage->oxygen_attr, &m_scatter_heart);
    
    // 存储完成需要对内存数据清空 Add by GT 2017-11-02
    memset(&p_data_manage->oxygen_attr, 0, sizeof(blood_oxygen_attr_t));
	memset(&p_data_manage->oxygen_resut[0], 0, sizeof(p_data_manage->oxygen_resut));	
    
	p_data_manage->oxygen_attr.wear = wear_tmp;

}

static void Thread_AdjustTimeClearDataTask(void)
{	
    bool adjust_time_clear_success = false;

    adjust_time_clear_success = Api_AdjustTimeClearFlash(&p_data_manage->history_time,clock);
    if(adjust_time_clear_success)
    {
        Data_ClearParamInit();
    }  	
}

static void Thread_AsynUpdateAppConfig(void)
{	
    Api_AsynUpdateAppConfig();
}

//修改蓝牙名称
static void Thread_BleNameConfig(void)
{	
    Dev_SetBleAdvName(m_ble_name);
    m_ble_adv_init = 1;
}

static void Thread_FactoryClearDataTask(void)
{	
//    Thread_InitZeroTimeTask();
//    Api_FlashFatoryRestore();
    
    if(p_data_manage->app_factory_clear_s.clear_data == 0x01
       && p_data_manage->app_factory_clear_s.clear_config == 0x01)
    {    
        Api_FlashFatoryRestore();     
        
        Data_ClearParamInit();
        Ped_SetSportsInfo(0, 0, 0);
        Ped_SetInputPersonalInformation(p_flash_param->person_s.height, p_flash_param->person_s.weight,
                                        p_flash_param->person_s.gender, p_flash_param->person_s.age);
        p_flash_param->Remind_flag = 0;
        
        Api_FlashDailyJumpBlock();
        
        // 其他参数或者功能状态清除
        Posture_StatisticalInit();
        Movement_Init();
        Algorithm_OledBrightnessAdjustTask();  

        //久坐
        p_data_manage->sedentary_cnt = 0;
        
        p_flash_param->Remind_flag = 0;
        
        p_data_manage->app_factory_clear_s.clear_data = 0x00;  
        p_data_manage->app_factory_clear_s.clear_config = 0x00;       
    }
    
    if(p_data_manage->app_factory_clear_s.clear_data == 0x01)
    {      
        Api_FlashClearUserData();      
                
        Data_ClearParamInit();
        Ped_SetSportsInfo(0, 0, 0);
        Ped_SetInputPersonalInformation(p_flash_param->person_s.height, p_flash_param->person_s.weight,
                                        p_flash_param->person_s.gender, p_flash_param->person_s.age);
        p_flash_param->Remind_flag = 0;
        
        Api_FlashDailyJumpBlock();

        // 其他参数或者功能状态清除
        Posture_StatisticalInit();
        Movement_Init();
        //久坐
        p_data_manage->sedentary_cnt = 0;
        
        p_flash_param->Remind_flag = 0;
        
        p_data_manage->app_factory_clear_s.clear_data = 0x00;       
    }
    
    if(p_data_manage->app_factory_clear_s.clear_config == 0x01)
    {
        Api_FlashClearUserConfig();        
       
        Algorithm_OledBrightnessAdjustTask();  
        //久坐
        p_data_manage->sedentary_cnt = 0;
        
        p_flash_param->Remind_flag = 0;
        
        p_data_manage->app_factory_clear_s.clear_config = 0x00; 
    }    
}

static void Thread_LedAbnormalTask(void)
{	
    // 实时监测关灯条件
    if((Afe4900_IsOpen())&&(clock->hour >= 7))
    {
        bool stop_measure_condition = true;
        
        // 判断是否有血氧测量
        stop_measure_condition = (p_data_manage->blood_oxygen_measure_s.app == 0)&&(p_data_manage->blood_oxygen_measure_s.btn == 0);
        // 判断是否有心率测量
        stop_measure_condition = stop_measure_condition&&(p_data_manage->heart_measure_s.app == 0)&&(p_data_manage->heart_measure_s.btn == 0);
        // 判读是否有呼吸率测量
        stop_measure_condition = stop_measure_condition&&(p_data_manage->respiratory_rate_s.app == 0);
        // 判读是否有疲劳度测量
        stop_measure_condition = stop_measure_condition&&(p_data_manage->fatigue_measure_s.app == 0);
        // 判断是否白天测试心率(定时测量)
        stop_measure_condition = stop_measure_condition&&(p_data_manage->heart_measure_s.day_time == 0);
        // 判断是否开启运动模式
        stop_measure_condition = stop_measure_condition&&(Movement_GetState() != MOVEMENT_STATE_RUN);
        // 判断是否白天测量血压
        stop_measure_condition = stop_measure_condition&&(p_data_manage->bp_measure_s.bp_app == 0)&&(p_data_manage->bp_measure_s.bp_btn == 0);
        // 判断是否白天测试心率(定时测量)
        stop_measure_condition = stop_measure_condition&&(p_data_manage->bp_measure_s.bp_day_time == 0);
        
        // 判断是否白天测试血氧(定时测量)
        stop_measure_condition = stop_measure_condition && (p_data_manage->blood_oxygen_measure_s.ox_day_time == 0);
        
        if(stop_measure_condition)
        {
            BleThread_AFE4404DevClose();
        }
    }
    
    // 如果检测到充电,则需要立即关掉AFE4404
    if(Battery_Get_Status() != BATTERY_POWER_NORMAL)
    {
        BleThread_AFE4404DevClose();
    }
}

static void Thread_BpFuncSet(void)
{
    Dev_SetBpFunction(p_flash_param->bp_func_state);
}

/*******************************************************************************************************
** Function name:        ble_stack_thread
** Descriptions:         create  ble  stact  thread
** input parameter:      void   
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
static void ble_stack_thread(void * arg)
{
    UNUSED_PARAMETER(arg);
        
    while (1)
    {	 
        // 血氧开启测试功能
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_afe4404_bo_open,st_ble_res.m_task_count,Thread_AFE4404BoOpenTask);
	    
        // 心率开启测试功能
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_afe4404_heart_open,st_ble_res.m_task_count,Thread_AFE4404HeartOpenTask);
	    
        // 疲劳度开启测试功能
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_afe4404_fatigue_open,st_ble_res.m_task_count,Thread_AFE4404FatigueOpenTask);
        
        // AFE4404关闭功能
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_afe4404_close,st_ble_res.m_task_count,Thread_AFE4404CloseTask);
	    
        // 设备连接蓝牙任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_dev_connect,st_ble_res.m_task_count,Thread_DevConnectTask);
        
        // 设备断开蓝牙任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_dev_disconnect,st_ble_res.m_task_count,Thread_DevDisconnectTask);
	    	    
        // GSensor重启任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_gsensor_restart,st_ble_res.m_task_count,Thread_GSensorRestartTask);
	   
        // 出厂数据清除
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_factory_clear,st_ble_res.m_task_count,Thread_FactoryClearDataTask); 
        
        // 零点初始化
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_init_zerotime,st_ble_res.m_task_count,Thread_InitZeroTimeTask);  
	    
        // 保存系统时间任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_save_systemtime,st_ble_res.m_task_count,Thread_SaveSystemTimeTask);   
	     
        // 保存日常数据任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_save_dailydata,st_ble_res.m_task_count,Thread_SaveDailyDataTask); 

        // 保存血氧数据任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_save_oxygendata,st_ble_res.m_task_count,Thread_SaveOxygenDataTask);
	 
        // 对时清零任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_adjust_time_clear,st_ble_res.m_task_count,Thread_AdjustTimeClearDataTask);
        
        // App开关异步更新任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_app_config,st_ble_res.m_task_count,Thread_AsynUpdateAppConfig);

        // App修改蓝牙名称任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_ble_name_changa, st_ble_res.m_task_count, Thread_BleNameConfig);

        // 广播停止任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_adv_stop,st_ble_res.m_task_count,Thread_AdvertisingStopTask);
        
        // 广播启动任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_adv_start,st_ble_res.m_task_count,Thread_AdvertisingStartTask);
        
        // LED灯异常监测
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_led_abnormal_monitor,st_ble_res.m_task_count,Thread_LedAbnormalTask);
        
        // App修改血压功能开关任务
        THREAD_MUTEX_TASK_EXCUTE(st_ble_res.m_bp_func_set, st_ble_res.m_task_count, Thread_BpFuncSet);
        
	         
         /* !注意：这里需要判断算法任务的是否处理完成,只有任务处理完成,才能挂起线程
        *        如果这里返回pdFALSE时,说明线程没有调度任务 
        */
        if(xSemaphoreTake(st_ble_res.m_task_count,(TickType_t)0) == pdFALSE)
        {
            vTaskSuspend(st_ble_res.m_thread_handle);
        }
    }
}

/**@brief A function which is hooked to idle task.
 * @note Idle hook must be enabled in FreeRTOS configuration (configUSE_IDLE_HOOK).
 */
void vApplicationIdleHook( void )
{
         
}

/*******************************************************************************************************
** Function name:        vApplicationStackOverflowHook
** Descriptions:         协议栈溢出
** input parameter:      无   
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    while(1);
}


static void Create_Resource(void)
{
    // 创建信号量资源
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_afe4404_heart_open);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_afe4404_bo_open);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_afe4404_fatigue_open);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_afe4404_close);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_dev_connect);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_dev_disconnect);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_adv_start);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_adv_stop);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_gsensor_restart);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_factory_clear);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_app_config);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_init_zerotime);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_save_systemtime);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_save_dailydata);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_save_oxygendata);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_adjust_time_clear);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_led_abnormal_monitor);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_ble_name_changa);
    SEMAPHORE_CREATE_BINARY_RET(st_ble_res.m_bp_func_set);
    
    // 创建计数信号量,初始值为0,信号量最大值为19
    st_ble_res.m_task_count = xSemaphoreCreateCounting(19,0);
    if (NULL == st_ble_res.m_task_count)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
    
    // Start execution.
    if (pdPASS != xTaskCreate(ble_stack_thread, "BLE", 384, NULL, BLE_STACK_THREAD_PRIORITY, &st_ble_res.m_thread_handle))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}

/*******************************************************************************************************
** Function name:        BleThread_Init
** Descriptions:         ble 协议栈初始化 
** input parameter:      无
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void BleThread_Init(void)
{
    bool erase_bonds = false;
    
    // Initialize.
    timers_init();
 
    Buttons_Init();
    
    // 蓝牙协议栈初始化
    ble_stack_init();
    peer_manager_init(erase_bonds);

    Api_FlashInit();

    gap_params_init();
    services_init();
    Hid_Init();
    App_Ancs_Init();
    advertising_init();

    conn_params_init();
    
    Api_DevSNCheck();
        
    Thread_AdvertisingStartTask();
    
    Create_Resource();
}

void BleThread_AFE4404BloodOxygenOpen(void)
{
    THREAD_TASK_WAKE_UP(st_ble_res.m_afe4404_bo_open,st_ble_res.m_task_count,st_ble_res.m_thread_handle);	
}

void BleThread_AFE4404HeartOpen(void)
{
    THREAD_TASK_WAKE_UP(st_ble_res.m_afe4404_heart_open,st_ble_res.m_task_count,st_ble_res.m_thread_handle);
}

void BleThread_AFE4404BpOpen(void)
{   //心率和血压开启同一个模式
    THREAD_TASK_WAKE_UP(st_ble_res.m_afe4404_heart_open, st_ble_res.m_task_count, st_ble_res.m_thread_handle);
}

void BleThread_AFE4404FatigueOpen(void)
{
    THREAD_TASK_WAKE_UP(st_ble_res.m_afe4404_fatigue_open,st_ble_res.m_task_count,st_ble_res.m_thread_handle);
}

/*******************************************************************************************************
** Function name:        BleThread_AFE4404DevClose
** Descriptions:         AFE4404设备关闭任务唤醒函数
** input parameter:      无     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void BleThread_AFE4404DevClose(void)
{
    THREAD_TASK_WAKE_UP(st_ble_res.m_afe4404_close,st_ble_res.m_task_count,st_ble_res.m_thread_handle);
}

/*******************************************************************************************************
** Function name:        BleThread_ConnectDevice
** Descriptions:         连接唤醒
** input parameter:      无   
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void BleThread_ConnectDevice(void)
{
    THREAD_TASK_WAKE_UP(st_ble_res.m_dev_connect,st_ble_res.m_task_count,st_ble_res.m_thread_handle);
}

/*******************************************************************************************************
** Function name:        BThread_DisconnectDevice
** Descriptions:         断开唤醒
** input parameter:      无   
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void BThread_DisconnectDevice(bool app_command_disconnect)
{
    if(app_command_disconnect)
    {
        sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
    }
    else
    {
        THREAD_TASK_WAKE_UP(st_ble_res.m_dev_disconnect,st_ble_res.m_task_count,st_ble_res.m_thread_handle);
    }
}

/*******************************************************************************************************
** Function name:        BleThread_AdvertisingStart
** Descriptions:         开始广播 
** input parameter:      无   
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void BleThread_AdvertisingStart(void)
{
    THREAD_TASK_WAKE_UP(st_ble_res.m_adv_start,st_ble_res.m_task_count,st_ble_res.m_thread_handle);
}

/*******************************************************************************************************
** Function name:        BleThread_AdvertisingStop
** Descriptions:         停止广播
** input parameter:      无  
** Returned Value:       无
** Created  by:          郭涛
** Created  Datas:       2017-08-04
**------------------------------------------------------------------------------------------------------
** Modify By:
** Modify data:
** Modify Description:
*********************************************************************************************************/
void BleThread_AdvertisingStop(void)
{
    THREAD_TASK_WAKE_UP(st_ble_res.m_adv_stop,st_ble_res.m_task_count,st_ble_res.m_thread_handle);
}

void BleThread_GsensorRestart(void)
{
    THREAD_TASK_WAKE_UP(st_ble_res.m_gsensor_restart,st_ble_res.m_task_count,st_ble_res.m_thread_handle);	
}

static void BleThread_BoMeasureCondition(uint8_t hour, uint8_t minute, uint8_t second)
{    
    // 检测时间范围[24:00-7:00]
//    if(hour < 8)
	if(hour < 7)
    {
        //新校准：修改为5分钟初始化一次
        if(second == 0 && minute%5 == 0)
        {
            // 脱手检测状态获取
            if((Get_NoWearStation() <= 1)&&(p_flash_param->blood_oxygen_config_s.enable == 1))
            {
                if((p_data_manage->blood_oxygen_measure_s.btn == 0) && (p_data_manage->blood_oxygen_measure_s.app == 0)
                   && (p_data_manage->heart_measure_s.btn == 0) && (p_data_manage->heart_measure_s.app == 0)
                   && (p_data_manage->respiratory_rate_s.app == 0) && (Movement_GetState() != MOVEMENT_STATE_RUN)
                   && (p_data_manage->bp_measure_s.bp_app == 0) && (p_data_manage->bp_measure_s.bp_btn == 0))
                {
                    // 夜间满足条件测量
                    p_data_manage->blood_oxygen_measure_s.init = 1;
                    p_data_manage->blood_oxygen_measure_s.time = 1;
                    p_data_manage->blood_oxygen_measure_s.ox_day_time = 0; //区分是不是白天开启的
                    p_data_manage->blood_oxygen_measure_s.ox_calc_cnt = 0; // 校准完成后，计算次数，用于白天自动测量 
                    p_data_manage->blood_oxygen_measure_s.ox_prompt_sign_flag = 0;
                    p_data_manage->hrv_measure_s.init = 1;
                    p_data_manage->hrv_measure_s.time = 1;
                    p_data_manage->respiratory_rate_s.init = 1;
                    p_data_manage->respiratory_rate_s.time = 1;
                    p_data_manage->heart_measure_s.init = 1;
                    heart_init_cause = 1;
                    p_data_manage->heart_measure_s.day_time = 0;
                    p_data_manage->heart_measure_s.night_time = 1;
                    
                    //WJ.20190619
                    if(minute % 5 == 0 && p_flash_param->bp_func_state == 0x01)
                    {
                        p_data_manage->bp_measure_s.bp_init = 1;
                        p_data_manage->bp_measure_s.bp_day_time = 0;
                        p_data_manage->bp_measure_s.bp_night_time = 1;
                    }
                    
                    xSemaphoreGive(st_ble_res.m_afe4404_bo_open); 
                    xSemaphoreGive(st_ble_res.m_task_count);
                }
            }
        }
        else if(second == 59)
        {
            // 保存血氧数据  
            xSemaphoreGive(st_ble_res.m_save_oxygendata); 
            xSemaphoreGive(st_ble_res.m_task_count);
            
            //WJ.20180630.结束一次五分钟数据采集发送
            if(minute % 5 == 4 )
            {
                Test_OxygenStop();
            }
            
        }
        //新校准：正常情况下，佩戴通过，什么时候关闭血氧测量,计算呼吸暂停综合证
        else if(p_data_manage->blood_oxygen_measure_s.time == 1)
        {
            if(second == 0)
            {
                //每分钟初始化一次缓存
                p_data_manage->heart_measure_s.min_init = 1;
                p_data_manage->hrv_measure_s.min_init = 1;
                p_data_manage->respiratory_rate_s.min_init = 1;
                p_data_manage->heart_measure_s.night_time = 1;
            }
            else if(second == 58 && minute%5 == 4)
            {
                //YX.20180104.解决夜间血氧检测出现大片数据丢失的问题
                p_data_manage->blood_oxygen_measure_s.time = 0;
                p_data_manage->hrv_measure_s.time = 0;
                p_data_manage->respiratory_rate_s.time = 0;        
                AlgThread_Osahs();
            }
        }
    }
}

static void BleThread_HeartMeasureCondition(uint8_t hour, uint8_t minute)
{ 
    uint8_t bp_or_heart = 0;
    
    if((minute%5 == 0) && Get_NoWearStation() <= 1)
    {
        if(p_flash_param->blood_oxygen_config_s.enable == 0)
        {   // 血氧测量关闭
            if(!Afe4900_IsOpen())
			{
                if(p_flash_param->dev_config_s.heart_detect_enable == 1)
                {                    
                    p_data_manage->heart_measure_s.init = 1;
                    p_data_manage->heart_measure_s.day_time = 1;
                    p_data_manage->heart_measure_s.night_time = 0;
                    bp_or_heart = 1;
                }
                if(p_flash_param->dev_config_s.blood_detect_enable == 1 
                   && p_flash_param->bp_func_state == 0x01)
                {
                    p_data_manage->bp_measure_s.bp_init = 1;
                    p_data_manage->bp_measure_s.bp_day_time = 1;
                    p_data_manage->bp_measure_s.bp_night_time = 0;  
                    bp_or_heart = 1;                      
                }
                
                if(bp_or_heart != 1)
                {
                    return;
                }
                
                heart_init_cause = 2;

                //20180620 HJJ open calibration
                p_data_manage->heart_measure_s.calib_well = 0;
                p_data_manage->heart_measure_s.calib_init = 1;
                heart_calib_init_cause = 1;
                
                // 开启AFE4404任务
                xSemaphoreGive(st_ble_res.m_afe4404_heart_open);
                xSemaphoreGive(st_ble_res.m_task_count);	
            }
            else if((Afe4900_IsOpen() == 1)||(Afe4900_IsOpen() == 2))
            {
                if(p_flash_param->dev_config_s.heart_detect_enable == 1
                   && (p_data_manage->heart_measure_s.app == 0)
                   && (p_data_manage->heart_measure_s.btn == 0))
                { 
                    p_data_manage->heart_measure_s.day_time = 1;
                    p_data_manage->heart_measure_s.night_time = 0;
                    p_data_manage->heart_measure_s.valid_heart_cnt = 0;
                    p_data_manage->heart_measure_s.time_measure_cnt = 0;
                }
                if(p_flash_param->dev_config_s.blood_detect_enable == 1
                   && (p_data_manage->bp_measure_s.bp_app == 0)
                   && (p_data_manage->bp_measure_s.bp_btn == 0))
                {
                    p_data_manage->bp_measure_s.bp_init = 1;
                    p_data_manage->bp_measure_s.bp_day_time = 1;
                    p_data_manage->bp_measure_s.bp_night_time = 0;                      
                }                   
                
                memset(&p_data_manage->heart_measure_s.heart_cache,
                       0, sizeof(p_data_manage->heart_measure_s.heart_cache));
            }
        }
        else 
        {  // 血氧测量开启，只处理7~24点的测量
            if((hour >= 7)&&(hour < 24))
            {
                if((p_data_manage->blood_oxygen_measure_s.btn == 0) && (p_data_manage->blood_oxygen_measure_s.app == 0)
                   && (p_data_manage->heart_measure_s.btn == 0) && (p_data_manage->heart_measure_s.app == 0)
                   && (p_data_manage->respiratory_rate_s.app == 0) && (Movement_GetState() != MOVEMENT_STATE_RUN)
                   && (p_data_manage->bp_measure_s.bp_app == 0) && (p_data_manage->bp_measure_s.bp_btn == 0)
                   && (p_data_manage->fatigue_measure_s.app == 0))
                {
                    // 夜间满足条件测量
                    p_data_manage->blood_oxygen_measure_s.init = 1;
                    p_data_manage->blood_oxygen_measure_s.time = 1;
                    p_data_manage->blood_oxygen_measure_s.ox_day_time = 1; //区分是不是白天开启的
                    p_data_manage->blood_oxygen_measure_s.ox_calc_cnt = 0; // 校准完成后，计算次数，用于白天自动测量 
                    p_data_manage->blood_oxygen_measure_s.ox_prompt_sign_flag = 0;
                    p_data_manage->hrv_measure_s.init = 0;
                    p_data_manage->hrv_measure_s.time = 0;
                    p_data_manage->respiratory_rate_s.init = 0;
                    p_data_manage->respiratory_rate_s.time = 0;
                    /*
                    p_data_manage->heart_measure_s.init = 1;
                    heart_init_cause = 1;
                    p_data_manage->heart_measure_s.day_time = 0;
                    p_data_manage->heart_measure_s.night_time = 1;
                    */
                    if(p_flash_param->dev_config_s.blood_detect_enable == 1 
                      && p_flash_param->bp_func_state == 0x01)
                    {
                        p_data_manage->bp_measure_s.bp_init = 1;
                        p_data_manage->bp_measure_s.bp_day_time = 0;
                        p_data_manage->bp_measure_s.bp_night_time = 1;
                    }
                    if(p_flash_param->dev_config_s.heart_detect_enable == 1)
                    {                    
                        p_data_manage->heart_measure_s.init = 1;
                        heart_init_cause = 1;
                        p_data_manage->heart_measure_s.day_time = 0;
                        p_data_manage->heart_measure_s.night_time = 1;
                    }

                    xSemaphoreGive(st_ble_res.m_afe4404_bo_open); 
                    xSemaphoreGive(st_ble_res.m_task_count);
                }
                else if(Movement_GetState() == MOVEMENT_STATE_RUN)                
                {               
                    if(p_flash_param->dev_config_s.blood_detect_enable == 1 
                      && p_flash_param->bp_func_state == 0x01)
                    {
                        p_data_manage->bp_measure_s.bp_init = 1;
                        p_data_manage->bp_measure_s.bp_day_time = 1;
                    }
                    if(p_flash_param->dev_config_s.heart_detect_enable == 1)
                    {                    
                        p_data_manage->heart_measure_s.day_time = 1;
                    } 
                }
            }
        }
    }	
}

void BleThread_PeriodCycleTask(uint8_t hour, uint8_t minute, uint8_t second)
{
    // Flash保存时间信息任务
    if(second == 0)
    {
        xSemaphoreGive(st_ble_res.m_save_systemtime);
        xSemaphoreGive(st_ble_res.m_task_count);
    }
    
    // 不管开机还是关机都需要换块
    if((hour == 0)&&(minute == 0)&&(second == 0))
    {
        xSemaphoreGive(st_ble_res.m_init_zerotime);
        xSemaphoreGive(st_ble_res.m_task_count);		     
    }
    //每天一次 Remind_flag = 0 或者重新设定了目标再清理
	
    if(p_data_manage->device_trace_param_s.sign_on == 1)
    {
        //YX.20180619.再次开启蓝牙广播
        BleThread_RestartAdv();

        if(second == 0)
        {
            xSemaphoreGive(st_ble_res.m_led_abnormal_monitor); 
            xSemaphoreGive(st_ble_res.m_task_count);           

            // !注意:充电时不能启动心率测量
            if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
            {
                BleThread_HeartMeasureCondition(hour,minute);
            }
        }	 
	 
        //ZJH.20180611.修改为不管有无在充电，都需要定时5分钟保存日常数据
        // 定时保存五分钟数据
        if(((minute%5) == 0)&&(second == 5))
        {
            xSemaphoreGive(st_ble_res.m_save_dailydata);	
            xSemaphoreGive(st_ble_res.m_task_count);		     
        }
	 
        if(Battery_Get_Status() == BATTERY_POWER_NORMAL)
        {
            // 定时测量血氧
            BleThread_BoMeasureCondition(hour,minute,second);
        }
        else
        {
            Vnwd_SetWear(6);
        }
    } 
    
    // !注意:这里需要判断计数信号量是否有任务,如果有任务则唤醒任务线程
    if(xSemaphoreTake(st_ble_res.m_task_count,(TickType_t)0) == pdTRUE)
    {
        // 上面做计数信号量(减一)处理,这里需要加一还原
        xSemaphoreGive(st_ble_res.m_task_count);
        vTaskResume(st_ble_res.m_thread_handle); 
    }
}

void BleThread_AdjustTimeClearFlash(const system_clock_time_t* p_history_time)
{
    // 记录历史时间
    memcpy(&p_data_manage->history_time,clock,sizeof(system_clock_time_t));
    THREAD_TASK_WAKE_UP(st_ble_res.m_adjust_time_clear,st_ble_res.m_task_count,st_ble_res.m_thread_handle);	
}

void BleThread_AsynUpdateAppConfig(void)
{
    // !注意:这里APP配置开关是被动刷新,防止用户在APP上频繁点击开关造成异常.
    xSemaphoreGive(st_ble_res.m_app_config);
    xSemaphoreGive(st_ble_res.m_task_count);
}

void BleThread_FactoryClearData(void)
{
    THREAD_TASK_WAKE_UP(st_ble_res.m_factory_clear,st_ble_res.m_task_count,st_ble_res.m_thread_handle);
}

ble_nus_t* BleThread_GetBleNus(void)
{
    return &st_ble_res.m_ble_nus;	
}

ble_clk_t* BleThread_GetBleClk(void)
{
    return &st_ble_res.m_ble_clk;	
}

/*******************************************************************************************************
** Function name:        BleThread_RestartAdv
** Descriptions:         延时后继续开启蓝牙广播
** input parameter:      void     
** Returned Value:       void
** Created  by:          GT
** Created  Datas:       2017-08-04
*********************************************************************************************************/
void BleThread_RestartAdv(void)
{
    if(ble_adv_flg == 1)
    {
        if(p_data_manage->device_trace_param_s.ble_connect == 0)
        {           
            if(ble_adv_cnt++ > 3)
            {
                ble_adv_flg = 0;
                ble_adv_cnt = 0;
                BleThread_AdvertisingStop();
                BleThread_AdvertisingStart();
            }
        }
        else
        {
            ble_adv_flg = 0;
            ble_adv_cnt = 0;
        }
    }
}

void BleThread_UpdateBleNameConfig(void)
{
    xSemaphoreGive(st_ble_res.m_ble_name_changa);
    xSemaphoreGive(st_ble_res.m_task_count);
}

// 血压
void BleThread_UpdateBpFuncConfig(void)
{
    xSemaphoreGive(st_ble_res.m_bp_func_set);
    xSemaphoreGive(st_ble_res.m_task_count);
}

