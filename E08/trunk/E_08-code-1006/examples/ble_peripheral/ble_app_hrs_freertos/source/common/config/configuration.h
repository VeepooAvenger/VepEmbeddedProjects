#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <stdint.h>


/*
 *==========================================================
 *                  ������汾��Ϣ��
 *==========================================================
 */
#define VERSION_M                        0x00     
#define VERSION_H                        0x08   
#define VERSION_L                        0x00
#define VERSION_T                        0x04

/*
 *==========================================================
 *                  ���豸ID�š� 
 *==========================================================
 */
#define VEEPOO_DEVICE_ID_H               0      // �豸�Ÿ�8λ,��ʹ�ã�ֱ��ʹ�������
#define VEEPOO_DEVICE_ID_L               1006   // �豸�ŵ�8λ

/*
 *==========================================================
 *                  ����Ʒ������Ϣ��
 *==========================================================
 */
#define COMPANY                          0x5650        //VP
#define DEVICE_TYPE                      0x452D3038    //E-08
#define HARDWARE_TYPE                    0x11          //V1.1
#define SOFTWARE_TYPE                    0x0800        //V08.00
#define PRODUCT_DATE                     0x120C13      //2018.12.19
#define AGENCY_SERIAL_NUMBER             0x005650      //
#define PRODUCT_BATCH                    0x1300        //1900
#define PRODUCT_NUMBER                   0x000000      //000000

/*
 *==========================================================
 *                  ���ⲿFLASHͷ����Ϣ��
 *==========================================================
 */
#define FLASH_HEAD_LEN                  0x000E              //14
#define FLASH_HEAD_STR                  0x00564545504F4F00  //VEEPOO
#define FLASH_PROJ_TYPE                 0x4500              //Eϵ��
#define FLASH_VERSION_H                 0x01                //
#define FLASH_VERSION_L                 0x00                //

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

#define AFE4900_LOG_ENABLE              0
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
