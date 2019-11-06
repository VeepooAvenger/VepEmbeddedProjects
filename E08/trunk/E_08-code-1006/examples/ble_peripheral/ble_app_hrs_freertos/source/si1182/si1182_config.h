#ifndef __SI117X_CONFIG_H
#define __SI117X_CONFIG_H


#include "si1182_functions.h"


#define SI117X_MAX_ADDRESS          PARAM_ECG_LDCONFIG      //si117x��������ַ

#define	SI1182_FIFO_BUFFER_SIZE		(100)					// si1182��fifo��ȡ���ݻ����С
#define SI1182_LD_BUFFER_SIZE		(20)					// si1182��ld��ȡ���ݻ����С
#define SI1182_FIFO_LAST_DATA_SIZE	(10)					// si1182��fifoδ�������ݵı���ռ�
#define SI1182_FIFO_DATA_TEST_SIZE	(5)						// si1182������Խ�����


typedef struct
{
    uint8_t     rw_flag;                        //��д�Ĵ����ı�־��0��ȫ���Ĵ�����1��һ����2дһ��
    uint8_t     address;                        //�Ĵ����ĵ�ַ
    uint8_t     value;                          //�Ĵ�����ֵ
    uint8_t     parameter[SI117X_MAX_ADDRESS + 1];  //si117x����
}si117x_config_t;


// ԭʼ���ݿ��Ʋ���
typedef struct
{
    uint32_t   time;                        // ��¼si1171�жϵ�ʱ��
    uint32_t   address;
    uint32_t   index;
    uint8_t    minute;
    uint8_t    second;
    uint8_t    raw_data_0;                  // 1����ԭʼ���ݲ�0����
    uint8_t    counter;                     // 1����ԭʼ���ݼ���
    uint8_t    copy_num;                    // ��������
    uint8_t    init_address;                // ��ʼ��flash��ַ
    uint8_t    second_0_num;               // Ԥ��0sʱ�̣�����2��si117x�����ж�
    uint8_t    si1182_enable:1;             // ������־
    uint8_t    si1182_suspend:1;            // ������Ʊ�־
    uint8_t    si1182_err:1;                 // �����־
    uint8_t    rsv:5;

}raw_data_control_t;


#endif
