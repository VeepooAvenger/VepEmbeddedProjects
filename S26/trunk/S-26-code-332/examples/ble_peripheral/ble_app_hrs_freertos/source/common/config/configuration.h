#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <stdint.h>


/*
 *==========================================================
 *                  ������汾��Ϣ��
 *==========================================================
 */
#define VERSION_M                        0x00     
#define VERSION_H                        0x26   
#define VERSION_L                        0x00
#define VERSION_T                        0x18

/*
 *==========================================================
 *                  ���豸ID�š�
 *==========================================================
 */
#define VEEPOO_DEVICE_ID_H               0      // �豸�Ÿ�8λ
#define VEEPOO_DEVICE_ID_L               332    // �豸�ŵ�8λ

/*
 *==========================================================
 *                  ����Ʒ������Ϣ��
 *==========================================================
 */
#define COMPANY                          0x5650        //VP
#define DEVICE_TYPE                      0x532D3236    //S-26
#define HARDWARE_TYPE                    0x11          //V1.1
#define SOFTWARE_TYPE                    0x2600        //V26.00
#define PRODUCT_DATE                     0x130510      //2019.05.16
#define AGENCY_SERIAL_NUMBER             0x4A5900      //JY
#define PRODUCT_BATCH                    0x1300        //1900
#define PRODUCT_NUMBER                   0x000000      //000000

/*
 *==========================================================
 *                  ����־ģ��ʹ�ܿ��ء�
 *==========================================================
 */
#define TRACE_DEBUG_ENABLE              0    // ���ٵ�����Ϣ
 
#define ALGORITHM_LOG_ENABLE            0
#define COMMUNICATION_LOG_ENABLE        0
#define GUI_LOG_ENABLE                  0
#define BLE_LOG_ENABLE                  0
#define IDEL_LOG_ENABLE                 0

#define AFE4404_LOG_ENABLE              0
#define SLEEP_LOG_ENABLE                0
#define FLASH_LOG_ENABLE                0

/*
 *==========================================================
 *           ��BAT��CLK���������ط��������á�
 *==========================================================
 */
#define CLK_DATA_TIMEOUT_REPEAT_CNT     20        // CLK���������ϴ���ʱ�ȴ�����
#define BAT_DATA_TIMEOUT_REPEAT_CNT     20        // BAT���������ϴ����еȴ�����
#define UPLOAD_RESPONSE_SPLIT_CNT       3         // ������ӦHBandָ��ָ����

#endif
