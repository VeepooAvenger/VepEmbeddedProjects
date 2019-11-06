/*******************Copyright(c)*********************************
** Veepoo Tech
** COPYRIGHT (C) 2016-2019
**----------------------FileInfo---------------------------------
** File name:                  api_ext_flash.h
** Last modified Date:         2018-11-05
** Last SYS_VERSION:           0.0.1
** Descriptions:
**---------------------------------------------------------------
** Createdby:                  WJ
** Createddate:                2018-11-05
** SYS_VERSION:                0.0.1
** Descriptions:               �ⲿflash�ӿ�
**---------------------------------------------------------------
** Modified by:				
** Modified date:			
** SYS_VERSION:				
** Modified Descriptions:	
****************************************************************/

//עĿǰ��Ƶ�ȫ���ӿ���UI����������ֻ����UI�����е��ã�����������

#ifndef _API_EXT_FLASH_H_
#define _API_EXT_FLASH_H_

#include "data_manage.h"
#include "flash_data_type.h"


#define EXT_FLASH_TOTAL_SIZE        0x200000

//Ƭ��FLASH
#define EXT_FLASH_HEAD_BASE_ADDR    0x00000000

//Ƭ��FLASH---�洢ECG�Զ������������
#define ECG_EXT_FLASH_BASE_ADDR     0x00027000
#define ECG_EXT_FLASH_TOTAL_SIZE    0x2D000     // 180K
#define ECG_EXT_FLASH_END_SIZE      (ECG_EXT_FLASH_BASE_ADDR + ECG_EXT_FLASH_TOTAL_SIZE)

// �Ƿ����ô洢ECG�Զ��������ݣ���������ʱ����ֱ��ʹ�ã���Ҫ�޸ĺ�����������������������������
#define USED_EXT_FLASH_ECG_AUTO_ENABLED      1          //���ø����ã���Ҫ�����ж�����м����޸ģ�����
                                                        //ע��գɵĽ���λ��

#if USED_EXT_FLASH_ECG_AUTO_ENABLED


#define ECG_RECORD_MAX_CNT          0x1E        // 30��
#define ECG_ONE_RECORD_SIZE         0x1800      // 6K = 6144 Byte

#define ECG_ORIGINA_DATA_INDEX          11      //ԭʼ��������ʼ
#define ECG_ORIGINA_DATA_ADC_INDEX      14      //ԭʼ���ݿ�ʼ
#define ECG_ORIGINA_DATA_ADC_LEN        6000    //ԭʼ���ݳ���

#define ECG_RESULT_INDEX                6014    //�����������ʼ
#define ECG_RESULT_DATA_INDEX           6016    //���������ʼ
#define ECG_RESULT_DATA_LEN             25      //������ݳ���

#define ECG_EFFECTIVE_DATA_LEN          6048    //��Ч���ݳ���

#endif


// �ⲿFlash��ȡ���ݵ�ַ��
#define RECORD_EXT_FLASH_DATA_ADDR(block_id, offset)  (uint32_t)(EXT_FLASH_HEAD_BASE_ADDR + (block_id)*1024 + offset)
//��ʱ��Ҫ��ȡ�Զ������洢����
#define RECORD_EXT_FLASH_DATA_SIZE                    (ECG_EXT_FLASH_BASE_ADDR - EXT_FLASH_HEAD_BASE_ADDR) //(ECG_EXT_FLASH_END_SIZE - EXT_FLASH_HEAD_BASE_ADDR)  


#pragma pack(1)
typedef struct
{
    uint8_t     init:1;         // �ϵ��ʼ��
    uint8_t     factory_init:1; // �ָ�������ʼ��
    uint8_t     res1:6;         // ����
    
    uint8_t     write:1;        // д���
    uint8_t     read:1;         // �����
    uint8_t     res:6;          // ����
    
    uint8_t     *ecg_write_adc;         // д�����ַ
    uint32_t    ecg_write_adc_len;      // д���ݳ���    
    uint8_t     *ecg_write_result;      // д�����ַ
    uint32_t    ecg_write_result_len;   // д���ݳ���
     
    uint8_t     record_id;          // ��ȡ��¼ID
    uint16_t    offset_index;       // ƫ�Ƶ�ַ
    uint8_t     *read_data;         // �������ַ
    uint32_t    read_len;           // �����ݳ���
    
}ext_flash_ecg_opt_t;


typedef struct
{
    uint8_t     init:1;                 // �ϵ��ʼ��
    uint8_t     write_head:1;           // дͷ���ݱ��
    uint8_t     write_data:1;           // д���ݱ��
    uint8_t     read_head:1;            // ��ͷ���ݱ��
    uint8_t     read_data:1;            // �����ݱ��
    uint8_t     erase_block:1;          // ����һ���˶�ģʽ���ݱ��
    uint8_t     erase_all:1;            // ���������˶�ģʽ���ݱ��
    uint8_t     res:1;                  // ����
    
    uint8_t     *p_write_head;          // дͷ���ݵ�ַ
    uint8_t     write_head_block;       // дͷ���ݵĿ���
    uint32_t    write_head_len;         // дͷ���ݳ���
    
    uint8_t     *p_write_data;          // д���ݵ�ַ
    uint8_t     write_data_block;       // д���ݵĿ���
    uint8_t     write_data_number;      // д���ݵ�����
    uint32_t    write_data_len;         // д���ݳ���
     
    uint8_t     *p_read_head;           // ��ͷ���ݵ�ַ
    uint8_t     read_head_block;        // ��ͷ���ݿ���
    uint32_t    read_head_len;          // ��ͷ���ݳ���
    
    uint8_t     *p_read_data;           // �����ݵ�ַ
    uint8_t     read_data_block;        // �����ݵĿ���
    uint8_t     read_data_number;       // �����ݵ�����
    uint32_t    read_data_len;          // �����ݵĳ���
    
    uint8_t     erase_block_cnt;        // ����
}ext_flash_movement_opt_t;      //Ƭ��FLASH�洢���˶�ģʽ��������

typedef struct
{
    uint8_t     init:1;             // �ϵ��ʼ��
    uint8_t     write:1;            // д���
    uint8_t     read:1;             // �����
    uint8_t     erase_block:1;      // �������
    uint8_t     res:4;              // ����
    
    uint8_t     *p_write;           // д��ַ
    uint16_t    write_block;        // д����
    uint32_t    write_len;          // д����
    
     
    uint8_t     *p_read;            // ����ַ
    uint16_t    read_block;         // ������
    uint32_t    read_len;           // ������
    
    uint8_t     erase_block_id;     // �����Ŀ�ID
    uint8_t     erase_block_num;    // �����Ŀ�����
}ext_flash_ecg_rest_opt_t;      //Ƭ��FLASH�洢��ECG�����������

typedef struct
{
    uint8_t     init:1;             // �ϵ��ʼ��
    uint8_t     write:1;            // 
    uint8_t     read:1;             // 
    uint8_t     erase_block:1;      //
    uint8_t     erase_all:1;        //
    uint8_t     res:3;              // 
    
    uint8_t     *p_write;           // 
    uint8_t     write_day_id;       //
    uint16_t    write_block;        // 
    uint32_t    write_len;          // 
    
     
    uint8_t     *p_read;            // 
    uint8_t     read_day_id;        //
    uint16_t    read_block;         // 
    uint32_t    read_len;           // 
    
    uint8_t     erase_block_id;     //
    
    uint8_t     write_data_type;          // �������ͣ�0:Ĭ���ճ�����;1�����һ�����ڴ洢����ͳ�����ݣ� 
    uint8_t     read_data_type;          // �������ͣ�0:Ĭ���ճ�����;1�����һ�����ڴ洢����ͳ�����ݣ� 
    
}ext_flash_daily_opt_t;

typedef struct
{
    uint8_t     init:1;             // �ϵ��ʼ��
    uint8_t     write:1;            // д���
    uint8_t     read:1;             // �����
    uint8_t     res:5;              // ����
    
    uint8_t     *p_write;           // д��ַ
    uint32_t    write_offset;       // дƫ��
    uint32_t    write_len;          // д����
    
     
    uint8_t     *p_read;            // ����ַ
    uint32_t    read_offset;        // ��ƫ��
    uint32_t    read_len;           // ������
    
}ext_flash_sleep_opt_t;     //Ƭ��FLASH�洢��˯����������

typedef struct
{
    uint8_t     init:1;             // �ϵ��ʼ��
    uint8_t     write:1;            // д���
    uint8_t     read:1;             // �����
    uint8_t     erase:1;            // �������
    uint8_t     res:4;              // ����
    
    uint8_t     *p_write;           // д��ַ
    uint32_t    write_len;          // д����
    
    uint8_t     *p_read;            // ����ַ
    uint32_t    read_len;           // ������
}ext_flash_head_opt_t;      //Ƭ��FLASH�洢��FLASHͷ����������

typedef struct
{
    uint8_t     init:1;             // �ϵ��ʼ��
    uint8_t     write:1;            // д���
    uint8_t     read:1;             // �����
    uint8_t     erase:1;            // �������ֱ��
    uint8_t     erase_all:1;        // �������б��
    uint8_t     res:3;              // ����
    
    
    uint8_t     *p_write;           // д��ַ
    uint32_t    write_offset;       // дƫ��
    uint32_t    write_len;          // д����
    
    uint8_t     *p_read;            // ����ַ
    uint32_t    read_offset;        // ��ƫ��
    uint32_t    read_len;           // ������
    
    uint32_t     erase_offset;       // ����ƫ��
    uint32_t     erase_size;         // ���������ݴ�С
}ext_flash_all_opt_t;       //Ƭ��FLASH�洢��������������
#pragma pack()

//FLASH���в�����ʼ������
void Extflash_AllInfoInit(void);

//ECG����
void Extflash_EcgErrDeal(void);

//ECG ��ȡ�������� 
uint8_t Extflash_EcgReadSet(uint8_t record_id, uint16_t offset_index, uint8_t data_len, uint8_t *data_buf);

//ECG ��ȡ���
uint8_t Extflash_EcgReadIsComplete(void);

// ECG д��������
uint8_t Extflash_EcgWriteSet(uint8_t *adc_data, uint16_t adc_data_len, 
                            uint8_t *ecg_result, uint16_t ecg_result_len);

//ECG д��� 
uint8_t Extflash_EcgWriteIsComplete(void);

//������ʼ�� 
uint8_t Extflash_SetEcgIndexInit(void);

//�����ָ�����
uint8_t Extflash_SetEcgFactoryInit(void);
    
//Flash�����Ƿ�æµ
uint8_t Extflash_EcgIsBusy(void);    
    
//Movement
//�˶�ģʽ���ݴ���
void Extflash_MovementDeal(void);

//�˶�ģʽдͷ�������ò���
uint8_t Extflash_MovementWriteHeadSet(uint8_t block_count, uint8_t *head_data, uint16_t head_data_len);

//�˶�ģʽͷ���ݲ������
uint8_t Extflash_MovementWriteHeadIsComplete(void);

//�˶�ģʽд�����������ò���
uint8_t Extflash_MovementWriteDataSet(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t data_len);

//�˶�ģʽд�������
uint8_t Extflash_MovementWriteDataIsComplete(void);

//�˶�ģʽ��ȡͷ�������ò���
uint8_t Extflash_MovementReadHeadSet(uint8_t block_count, uint8_t *head_read_buf, uint16_t head_read_len);

//�˶�ģʽ��ȡͷ�������
uint8_t Extflash_MovementReadHeadIsComplete(void);

//�˶�ģʽ��ȡ�������ò���
uint8_t Extflash_MovementReadDataSet(uint8_t block_count, uint16_t number, uint8_t *data, uint16_t data_len);

//�˶�ģʽ��ȡ�������
uint8_t Extflash_MovementReadDataIsComplete(void);

//�˶�ģʽ����һ����������
uint8_t Extflash_MovementEraseBlockSet(uint8_t block_count);

//�˶�ģʽ����һ���������
uint8_t Extflash_MovementEraseBlockIsComplete(void);

//�˶�ģʽ����������������
uint8_t Extflash_MovementEraseAllSet(void );

//�˶�ģʽ���������������
uint8_t Extflash_MovementEraseAllIsComplete(void);

//�˶�ģʽ�����Ƿ���æ
uint8_t Extflash_MovementIsBusy(void);

//ECG result
//ECG���������
void Extflash_EcgRestDeal(void);

//����дECG�������
uint8_t Extflash_EcgRestWriteSet(uint16_t block_count, uint8_t *data, uint16_t data_len);

//дECG������
uint8_t Extflash_EcgRestWriteIsComplete(void);

//���ö�ȡECG����Ĳ���
uint8_t Extflash_EcgRestReadSet(uint16_t block_count, uint8_t *read_buf, uint16_t read_len);

//��ȡECG������
uint8_t Extflash_EcgRestReadIsComplete(void);

//����ECG�������
uint8_t Extflash_EcgRestEraseBlockSet(uint8_t block_id, uint8_t block_num);

//����ECG����������
uint8_t Extflash_EcgRestEraseBlockIsComplete(void);

//ECG������ݲ�������
uint8_t Extflash_EcgRestIsBusy(void);

// Daily
void Extflash_DailyDeal(void);

uint8_t Extflash_DailyWriteSet(uint8_t day_id, uint16_t block_count, uint8_t *data, uint16_t data_len, uint8_t data_type);

uint8_t Extflash_DailyWriteIsComplete(void);

uint8_t Extflash_DailyReadSet(uint8_t day_id, uint16_t block_count, uint8_t *data, uint16_t data_len, uint8_t data_type);

uint8_t Extflash_DailyReadIsComplete(void);

uint8_t Extflash_DailyEraseBlockSet(uint8_t block_id);

uint8_t Extflash_DailyEraseBlockIsComplete(void);

uint8_t Extflash_DailyEraseAllSet(void);

uint8_t Extflash_DailyEraseAllIsComplete(void);

uint8_t Extflash_DailyIsBusy(void);

//sleep
//˯�߽��������
void Extflash_SleepDeal(void);

//����д˯�߽���Ĳ���
uint8_t Extflash_SleepWriteSet(uint32_t offset, uint8_t *data, uint16_t data_len);

//д˯�߽�����
uint8_t Extflash_SleepWriteIsComplete(void);

//���ö�ȡ˯�߽���Ĳ���
uint8_t Extflash_SleepReadSet(uint32_t offset, uint8_t *data, uint16_t data_len);

//��ȡ˯�߽�����
uint8_t Extflash_SleepReadIsComplete(void);

//˯�߽��������æ
uint8_t Extflash_SleepIsBusy(void);


//head
//�ⲿFLASHͷ����Ϣ������
void Extflash_HeadDeal(void);

//����д�ⲿFLASHͷ����Ϣ�Ĳ���
uint8_t Extflash_HeadWriteSet(uint8_t *data, uint16_t data_len);

//д�ⲿFLASHͷ����Ϣ���
uint8_t Extflash_HeadWriteIsComplete(void);

//���ö�ȡ�ⲿFLASHͷ����Ϣ�Ĳ���
uint8_t Extflash_HeadReadSet(uint8_t *data, uint16_t data_len);

//��ȡ�ⲿFLASHͷ����Ϣ���
uint8_t Extflash_HeadReadIsComplete(void);

//���ò����ⲿFLASHͷ����Ϣ
uint8_t Extflash_HeadEraseSet(void);

//�����ⲿFLASHͷ����Ϣ���
uint8_t Extflash_HeadEraseIsComplete(void);

//�ⲿFLASHͷ����Ϣ������æ
uint8_t Extflash_HeadIsBusy(void);


//all FLASH
//�ⲿFLASH������
void Extflash_AllOptDeal(void);

//�����ⲿFLASHд����
uint8_t Extflash_AllOptWriteSet(uint32_t write_offset, uint8_t *data, uint16_t data_len);

//�ⲿFLASHд���
uint8_t Extflash_AllOptWriteIsComplete(void);

//�����ⲿFLASH������
uint8_t Extflash_AllOptReadSet(uint32_t read_offset, uint8_t *read_buf, uint16_t read_len);

//�ⲿFLASH�����
uint8_t Extflash_AllOptReadIsComplete(void);

//�����ⲿFLASH�����Ĳ���
uint8_t Extflash_AllOptEraseSizeSet(uint32_t erase_offset, uint32_t erase_size);

//�ⲿFLASH�������
uint8_t Extflash_AllOptEraseBlockIsComplete(void);

//�����ⲿFLASH������������
uint8_t Extflash_AllOptEraseAllSet(void);

//�ⲿFLASH���������������
uint8_t Extflash_AllOptEraseAllIsComplete(void);

//�ⲿFLASH������æ
uint8_t Extflash_AllOptIsBusy(void);
#endif
