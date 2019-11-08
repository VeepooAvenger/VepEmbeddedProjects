/*
*	modified  by anqiren  2014/12/02  V1.0bat
*
**/

#include "ble_wechat_service.h"
#include "nrf_gpio.h"
#include "ble_conn_params.h"
#include "ble_types.h"
#include "ble_wechat_util.h"
#include "app_error.h"
#include <stdlib.h>

#define USING_PRODUCT PRODUCT_TYPE_MPBLEDEMO2

static uint8_t m_addl_adv_manuf_data[BLE_GAP_ADDR_LEN];

ble_wechat_t m_ble_wechat;

uint32_t wechat_step = 0;

/*****************************************************************************
* data handle
*****************************************************************************/
data_info g_send_data = {NULL, 0, 0};
data_info g_rcv_data = {NULL, 0, 0};
 
static int ble_wechat_indicate_data_chunk(ble_wechat_t *p_wcs)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t chunk_len = 0;

    chunk_len = g_send_data.len-g_send_data.offset;
    chunk_len = chunk_len > BLE_WECHAT_MAX_DATA_LEN?BLE_WECHAT_MAX_DATA_LEN:chunk_len;

    if (chunk_len == 0) {
//    	p_data_handler->m_data_free_func(g_send_data.data, g_send_data.len);
        g_send_data.data = NULL;
        g_send_data.len = 0;
        g_send_data.offset = 0;
        return 0;
    }
    memset(&hvx_params, 0, sizeof(hvx_params));
    hvx_params.handle = p_wcs->indicate_handles.value_handle;
    hvx_params.p_data = g_send_data.data+g_send_data.offset;
    hvx_params.p_len  = &chunk_len;
    hvx_params.type   = BLE_GATT_HVX_INDICATION;

    g_send_data.offset += chunk_len;
    return sd_ble_gatts_hvx(p_wcs->conn_handle, &hvx_params);
}

void on_indicate_comfirm(ble_wechat_t *p_wcs, ble_evt_t * p_ble_evt)
{
    ble_wechat_indicate_data_chunk(p_wcs);
}

/**@brief Function for error handling, which is called when an error has occurred. 
 * @warning This handler is an example only and does not fit a final product. You need to analyze 
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] p_data_handler where Error come from
 */
void wechat_error_chack(ble_wechat_t *p_wcs, data_handler *p_data_handler, int error_code)
{
//	switch(error_code)
//	{
//		case EEC_system:
//		{
//		#ifdef CATCH_LOG
//			printf("\r\n! error: system error");
//		#endif
//		}
//			break ;
//		case EEC_needAuth:
//		#ifdef CATCH_LOG
//			printf("\r\n! error: needAuth");
//		#endif
//			break ;
//		case EEC_sessionTimeout:
//		#ifdef CATCH_LOG
//			printf("\r\n! error: sessionTimeout");
//		#endif
//			break ;
//		case EEC_decode:
//		#ifdef CATCH_LOG
//			printf("\r\n! error: decode");
//		#endif
//			break ;
//		case EEC_deviceIsBlock:
//		#ifdef CATCH_LOG
//			printf("\r\n! error: deviceIsBlock");
//		#endif
//			break ;
//		case EEC_serviceUnAvalibleInBackground:
//		#ifdef CATCH_LOG
//			printf("\r\n! error: serviceUnAvalibleInBackground");
//		#endif
//			break ;
//		case EEC_deviceProtoVersionNeedUpdate:
//		#ifdef CATCH_LOG
//			printf("\r\n! error: deviceProtoVersionNeedUpdate");
//		#endif
//			break ;
//		case EEC_phoneProtoVersionNeedUpdate:
//		#ifdef CATCH_LOG
//			printf("\r\n! error: phoneProtoVersionNeedUpdate");
//		#endif
//			break ;
//		case EEC_maxReqInQueue:
//		#ifdef CATCH_LOG
//			printf("\r\n! error: maxReqInQueue");
//		#endif
//			break ;
//		case EEC_userExitWxAccount:
//		#ifdef CATCH_LOG
//			printf("\r\n! error: userExitWxAccount");
//		#endif
//			break ;
//		default:
//			break ;
//	}
	p_data_handler->m_data_error_func(error_code);
}

/**@brief     Function for handling the @ref BLE_GATTS_EVT_WRITE event from the S110 SoftDevice.
 *
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_write(ble_wechat_t *p_wcs, ble_evt_t * p_ble_evt)
{
//	int error_code;
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    int chunk_size = 0;
    if (p_evt_write->handle == p_wcs->write_handles.value_handle &&
            p_evt_write->len <= BLE_WECHAT_MAX_DATA_LEN) 
    {
        if (g_rcv_data.len == 0) 
        {
            BpFixHead *fix_head = (BpFixHead *) p_evt_write->data;
            g_rcv_data.len = wechat_ntohs(fix_head->nLength);
            g_rcv_data.offset = 0;
            g_rcv_data.data = (uint8_t *)malloc(g_rcv_data.len);
        }	
        chunk_size = g_rcv_data.len - g_rcv_data.offset;
        chunk_size = chunk_size < p_evt_write->len ? chunk_size : p_evt_write->len;
        memcpy(g_rcv_data.data+g_rcv_data.offset, p_evt_write->data, chunk_size);
        g_rcv_data.offset += chunk_size;
        if (g_rcv_data.len <= g_rcv_data.offset) 
        {
//			error_code = p_data_handler->m_data_consume_func(g_rcv_data.data, g_rcv_data.len);
//			p_data_handler->m_data_free_func(g_rcv_data.data,g_rcv_data.len);
//			wechat_error_chack(p_wcs, p_data_handler, error_code);
            g_rcv_data.len = 0;
            g_rcv_data.offset = 0; 
        }	
	}
}

void ble_wechat_on_ble_evt(ble_wechat_t *p_wcs,ble_evt_t * p_ble_evt) 
{
	switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            p_wcs->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;	
            break;
        case BLE_GATTS_EVT_WRITE:
            on_write(p_wcs,p_ble_evt);
//          ble_wechat_send_data(p_wcs);
            break;
        case BLE_GATTS_EVT_HVC:
//			on_indicate_comfirm(p_wcs,p_ble_evt);
			break;	
        case BLE_GAP_EVT_DISCONNECTED:
            p_wcs->conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
        default:
            break;
    }
}

//Add WeChat Service to GATTS sercvice
uint32_t ble_wechat_add_service(ble_wechat_t *p_wechat)
{
    uint32_t err_code;
    ble_uuid_t ble_wechat_uuid;
    BLE_UUID_BLE_ASSIGN(ble_wechat_uuid, BLE_UUID_WECHAT_SERVICE);
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_wechat_uuid, &p_wechat->service_handle);
    return err_code;
}

static void get_mac_addr(uint8_t *p_mac_addr)
{
    uint32_t error_code;
    ble_gap_addr_t *p_mac_addr_t = (ble_gap_addr_t*)malloc(sizeof(ble_gap_addr_t));
    
#if (NRF_SD_BLE_API_VERSION == 3)
    error_code = sd_ble_gap_addr_get(p_mac_addr_t);
#else
    error_code = sd_ble_gap_address_get(p_mac_addr_t);
#endif
    
    APP_ERROR_CHECK(error_code);
    uint8_t *d = p_mac_addr_t->addr;
    for ( uint8_t i = 6; i >0;)
    {	
        i--;
        p_mac_addr[5-i]= d[i];
    }
    free(p_mac_addr_t);
    p_mac_addr_t = NULL;
}

//Add the indicate characteristic //添加显示特征值
uint32_t ble_wechat_add_indicate_char(ble_wechat_t *p_wechat)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
  
    uint8_t data[4] =  {0x01,0x00,0x00,0x00};

    data[1] = (uint8_t)(wechat_step>>0x00);
    data[2] = (uint8_t)(wechat_step>>0x08);
    data[3] = (uint8_t)(wechat_step>>0x10);
		
    char user_desc[]    = "current_pedometer_measurement";

    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
	
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.indicate = 1;
    char_md.char_props.read     = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;//&cccd_md;
    char_md.p_sccd_md         = NULL;//&cccd_md;
    char_md.p_char_user_desc  = (uint8_t *)user_desc;
  	char_md.char_user_desc_size     = strlen(user_desc);
    char_md.char_user_desc_max_size	= strlen(user_desc); 
		
    BLE_UUID_BLE_ASSIGN(char_uuid, BLE_UUID_WECHAT_INDICATE_CHARACTERISTICS);
    memset(&attr_md, 0, sizeof(attr_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));
   
    attr_char_value.p_uuid       = &char_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(data);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = BLE_WECHAT_MAX_DATA_LEN;
    attr_char_value.p_value      = (uint8_t *)data;
    
    return sd_ble_gatts_characteristic_add(p_wechat->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_wechat->indicate_handles);
                                                                       
}

//Add the write characteristic 
static uint32_t ble_wechat_add_write_char(ble_wechat_t *p_wechat)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    char *data = "write char";
    char user_desc[]    = "target";
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.indicate = 1;
    char_md.char_props.write    = 1;
    char_md.char_props.read     = 1;
    char_md.p_char_user_desc  = (uint8_t *)user_desc;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
    char_md.char_user_desc_size     = strlen(user_desc);
    char_md.char_user_desc_max_size	= strlen(user_desc); 
  
    BLE_UUID_BLE_ASSIGN(char_uuid, BLE_UUID_WECHAT_WRITE_CHARACTERISTICS);
	
    memset(&attr_md, 0, sizeof(attr_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    
    attr_char_value.p_uuid       = &char_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = strlen(data);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = BLE_WECHAT_MAX_DATA_LEN;
    attr_char_value.p_value      = (uint8_t *)data;
    
    return sd_ble_gatts_characteristic_add(p_wechat->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_wechat->write_handles);
}

//Add the read characteristic 
static uint32_t ble_wechat_add_read_char(ble_wechat_t *p_wechat)
{
	//m_addl_adv_manuf_data.
    get_mac_addr(m_addl_adv_manuf_data);
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
	
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
	
    BLE_UUID_BLE_ASSIGN(char_uuid, BLE_UUID_WECHAT_READ_CHARACTERISTICS);
	
    memset(&attr_md, 0, sizeof(attr_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    
    attr_char_value.p_uuid       = &char_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(m_addl_adv_manuf_data);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = BLE_WECHAT_MAX_DATA_LEN;
    attr_char_value.p_value      = m_addl_adv_manuf_data;
    
    return sd_ble_gatts_characteristic_add(p_wechat->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_wechat->read_handles);
}

//Add the WeChat characteristic include indicate write and read characteristic
uint32_t ble_wechat_add_characteristics(ble_wechat_t *p_wechat)
{
    uint32_t err_code;
    err_code = ble_wechat_add_indicate_char(p_wechat);
    APP_ERROR_CHECK(err_code);
    err_code = ble_wechat_add_write_char(p_wechat);
    APP_ERROR_CHECK(err_code);
    err_code = ble_wechat_add_read_char(p_wechat);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

//device sent data on the indicate characteristic
int ble_wechat_indicate_data(ble_wechat_t *p_wcs,data_handler *p_data_handler, uint8_t *data, int len)
{

    if (data == NULL || len == 0) {
            return 0;	
    }
    if (g_send_data.len != 0 && g_send_data.offset != g_send_data.len) {
//		printf("\r\n offset:%d %d",g_send_data.offset,g_send_data.len);
//		g_send_data.len = 0;
//		g_send_data.offset = 0;
        
        return 0;
    }
    g_send_data.data = data;
    g_send_data.len = len;
    g_send_data.offset = 0;
    return (ble_wechat_indicate_data_chunk(p_wcs));
}

//wechat service initialization
void ble_wechat_service_init(void)
{
    uint32_t       err_code;
    err_code = ble_wechat_add_service(&m_ble_wechat);
    APP_ERROR_CHECK(err_code);
    err_code = ble_wechat_add_characteristics(&m_ble_wechat);
    APP_ERROR_CHECK(err_code);
}

int ble_wechat_send_data(uint32_t all_step)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t chunk_len = 4;
	uint8_t data[4] = {0x01,0x00,0x00,0x00};
    wechat_step = all_step;

    data[1] = (uint8_t )(all_step>>0x00);
    data[2] = (uint8_t )(all_step>>0x08);
    data[3] = (uint8_t )(all_step>>0x10);
 
    memset(&hvx_params, 0, sizeof(hvx_params));
    hvx_params.handle = m_ble_wechat.indicate_handles.value_handle;
    hvx_params.p_data = data;
    hvx_params.p_len  = &chunk_len;
    hvx_params.type   = BLE_GATT_HVX_INDICATION;

    if(m_ble_wechat.conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        return sd_ble_gatts_hvx(m_ble_wechat.conn_handle, &hvx_params); 
    }
    return 0;
}



