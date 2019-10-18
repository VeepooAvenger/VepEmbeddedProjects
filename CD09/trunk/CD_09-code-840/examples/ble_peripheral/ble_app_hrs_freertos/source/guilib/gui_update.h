/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_update.h
** Last modified Date:   2017-08-08
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2017-08-08
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _GUI_UPDATE_H_
#define _GUI_UPDATE_H_

#include <stdint.h>

typedef struct
{	
    uint8_t     cache_buf[200]; //��������
    uint8_t     store_buf[200]; //�洢����
    uint8_t     buf_cnt;        //���ݵĳ���
    uint32_t    now_sub;        //��ǰ�洢���±�����
    uint32_t    revice_sum;     //�Ѿ��������ݵ��ܳ���
    uint32_t    addr;           //��ʼ��ַ
    uint32_t    len;            //���ݵ��ܳ���
}gui_data_update_t;

typedef struct {
    uint32_t start_addr;         //��ʼ��ַ 
    uint32_t total_len;          //�ܳ���
    uint32_t read_len;           //�Ѿ���ȡ�ĳ���
}gui_flash_data_t; 

/* ����UI���ݵ���ز�����ʼ�� */
void Gui_UpdateInit(void);
/* ����UI���ݵ���ز�����ʼ�� */
uint8_t Gui_UpdateCheck(uint8_t * p_data);
/* ����FLASH��UI������ */
uint8_t Gui_UpdateFlash(uint8_t * p_data);
/* ����UI���� */
uint8_t Gui_EraseUiData(uint8_t * p_data);
/* ��ȡUI���ݵ���ز�����ʼ�� */
uint8_t Gui_ReadUiData(uint8_t * p_data);
/* �������ݳ�ʱ��ʱ����ʼ�� */
void Gui_UpdateTimeoutInit(void);
/* �������ݳ�ʱ��ʱ���ر� */
void Gui_UpdateTimeoutStop(void);
/* �������ݳ�ʱ��ʱ������ */
void Gui_UpdateTimeoutStart(void);
/* ��ʱ���������� */
void Gui_UpdateTimeoutHandler(void);
/* ����UI������ */
uint8_t GUI_SendUiDataHandler(void);

//crcУ��
uint8_t Gui_UiCrcCheck(const uint16_t crc_value, const uint32_t ui_len, uint16_t *out_crc);

#endif

