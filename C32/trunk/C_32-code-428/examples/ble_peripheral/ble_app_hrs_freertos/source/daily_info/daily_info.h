/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     daily_info.h
** Last modified Date:   2017-08-01
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-08-01
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef __DAILY_INFO_H
#define __DAILY_INFO_H

#define USE_DAILY_INFO  1       //�Ƿ���˯�߿���

#if USE_DAILY_INFO
#include <stdint.h>

#define DAILY_PKG_MAX_NUM   (358 - 2)

#pragma pack(1)
typedef struct{
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t steps_h;
    uint8_t steps_l;
    uint8_t movement_h;
    uint8_t movement_l;
    uint8_t heart;
    uint8_t bp_h;
    uint8_t bp_l;
    uint8_t sleep_posture;  //˯����̬
    uint8_t sleep_code;     //˯�߼������
    uint8_t sleep_posture_cnt;  //˯����̬����;
    uint8_t distance_h;
    uint8_t distance_l;
    uint8_t calories_h;
    uint8_t calories_l;
    uint8_t reserve6;       //��λ����
    uint8_t reserve7;       //��λʱ�����
}daily_data_format_t;       //size = 20   

typedef struct{
    uint8_t block;
    uint16_t pkg_cnt;
}daily_pkg_info_t;

typedef struct{
    uint8_t     block_flag;
    uint8_t     month;
    uint8_t     day;    
    uint8_t     all_step[3];
    uint16_t    pack_num;
    uint16_t    sleep_cnt;
    uint32_t    all_calories;  //�洢��·��
    uint32_t    all_distance;  //�洢����  
    uint8_t     reserve[22];
}daily_block_info_t;

typedef struct{
    uint32_t last_setp_num;
    uint32_t last_sport_momentum;
    uint32_t last_setp_num_5m;
    uint32_t last_setp_num_10m;
    uint32_t last_distance;
    uint32_t last_calories;
}daily_last_info_t;

#pragma pack()

/*********************************************************************
**  Functions declare
*/

//��ȡǰ����洢�ճ����ݵĿ��
uint8_t Daily_GetPreviousBlock(uint8_t previous_num);

//�������������
void Daily_SaveDailyData(void);

//get 5 minute period daily data
void Daily_GetDailyData(void);

//��ʼ����
void Daily_Init(void);

//���õ���Ĵ洢��źͼ�¼����
void Daily_SetDailyPkgInfo(uint8_t block, uint16_t pkg_cnt);

//���鴦��
void Daily_NextDay(void);

//�����ճ����ݵ�Ѫ�������ʡ�Ѫѹֵ
void Daily_SetDailyData(uint8_t oxgen, uint8_t heart, uint8_t bp_dp, uint8_t bp_sp);

//���õ�ǰ�����ϢΪ����ʹ�ã�����ͷ����Ϣ
void Daily_SeHeadtUsing(void);

//���õ�ǰ�����ϢΪʹ����ɣ�����ͷ����Ϣ
void Daily_DailyHeadSetUsed(const uint8_t month, const uint8_t day);

//���ָ����ŵ���Ϣ������Ϊ����Ĵ洢���
void Daily_ClearDailyByBlcak(uint8_t black);

//���������ճ����ݴ洢�����ڶ�ʱ
void Daily_ClearDailyCurrentBlcak(void);

// ��ȡ��ǰ�洢�ճ�������Ϣ
daily_pkg_info_t *Daily_GetPkgInfo(void);

//˯���㷨��ȡ�ճ����ݻص������ӿ�
uint32_t Daily_SleepGetDailyDataCallback(uint8_t *data, uint16_t current_pkg, uint16_t pre_offset);

//��ȡָ����ŵĴ洢ͷ����Ϣ
uint8_t Daily_GetBlockHeadInfo(daily_block_info_t *daily_block_info, uint8_t block_num);

//��ȡָ������ܼƲ�����
uint32_t Daily_GetAllWalkCntByDays(uint8_t ago_days);

//�������5���ӵļƲ�
void Daily_SetIntervalSteps5m(uint32_t setp_num );

//��ȡ���5���ӵļǲ�����
uint32_t Daily_GetIntervalSteps5m(void);

//��ȡ���10���ӵļǲ�����
uint32_t Daily_GetIntervalSteps10m(void);

//���ڲ���˯������ʹ�ã��洢���ż�1
void Daily_TestSleepAddPkg(void);

//����˯���㷨���ش��룬�洢����һ������Ӽ�¼��
void Daily_SetSleepCode(uint8_t code);

//��ȡ������õ�˯���㷨���ش���
uint8_t Daily_GetSleepCode(void);

//��ʼ���ճ���Ϣ
void Daily_InitFromFlash(void);


#endif  //end USE_DAILY_INFO


#endif  //end __DAILY_INFO_HSS

