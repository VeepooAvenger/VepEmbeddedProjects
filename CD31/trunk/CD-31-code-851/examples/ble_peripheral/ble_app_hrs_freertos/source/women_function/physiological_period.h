/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     physiological_period.h
** Last modified Date:   2017-07-28
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          WJ
** Created  date:        2017-10-18
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

#ifndef APP_PHYSIOLOGICAL_PERIOD_H_
#define APP_PHYSIOLOGICAL_PERIOD_H_

#include <stdint.h>
      
#define RMIND_INFO_MAX_SIZE   200      //��ʾ��Ϣ����󳤶�Ϊ200

typedef enum
{
    PHYS_PERIOD_MESTRUAL = 1,           //����
    PHYS_PERIOD_FOR_PREGNANCY,          //������
    PHYS_PERIOD_PREGNANCY,              //����
    PHYS_PERIOD_HOT_MAMA,               //������
    PHYS_PERIOD_MAX
    
}PHYS_PERIOD_TYPE_E;

typedef enum
{
    PHYS_MENSTRUAL_NULL = 0,                   //�Ƿ�
    PHYS_MENSTRUAL_MENSTRUAL_PERIOD,           //����
    PHYS_MENSTRUAL_SAFETY_PERIOD,              //��ȫ��
    PHYS_MENSTRUAL_OVIPOSIT_PERIOD,            //������
    PHYS_MENSTRUAL_OVIPOSIT_DAY,               //������   
   
}PHYS_MENSTRUAL_TYPE_E;

typedef enum
{
    PHYS_REMIND_NULL = 0,                   //�Ƿ�
    PHYS_REMIND_MENSTRUAL_PERIOD,           //����
    PHYS_REMIND_SAFETY_PERIOD,              //��ȫ��
    PHYS_REMIND_SAFETY_PERIOD1,             //��ȫ��1�����뾭�� <=3
    PHYS_REMIND_OVIPOSIT_PERIOD,            //������
    PHYS_REMIND_OVIPOSIT_DAY,               //������
    PHYS_REMIND_OVIPOSIT_PERIOD1,           //������������
    PHYS_REMIND_OVIPOSIT_DAY1,              //������������
    PHYS_REMIND_FORECAST_MENSTRUAL_PERIOD,  //Ԥ�⾭��    
    
    PHYS_REMIND_PREGNANCY_ONE,              //���е�һ�׶Σ�1~3���£�
    PHYS_REMIND_PREGNANCY_TWO,              //���еڶ��׶Σ�3~6���£�
    PHYS_REMIND_PREGNANCY_THU,              //���е����׶Σ�6����~����ǰ��
    PHYS_REMIND_PREGNANCY_BIRTH,            //���е��Ľ׶Σ�����������
    PHYS_REMIND_PREGNANCY_ERROR,            //������Ϣ���ڣ�������Ϣ
    PHYS_REMIND_BABY_BORN,                  //����������
   
}PHYS_REMIND_TYPE_E;

#pragma pack(1)
//�洢
typedef struct
{
	uint8_t  phys_period_time;          //����ʲô����ʱ��,0:û�м�¼,1:����,2:������,3:������,4:����
	uint16_t phys_period_year;          //���һ�����¾�:��
	uint8_t  phys_period_month;         //���һ�����¾�:��
	uint8_t  phys_period_day;           //���һ�����¾�:��
	uint8_t  phys_period_cycle;         //�¾�����
	uint8_t  phys_mens_days;            //��������
	uint8_t  phys_mens_actual_days;     //ʵ�ʾ�������
    
	uint16_t baby_birthday_year;        //������������:��
	uint8_t  baby_birthday_month;       //������������:��
	uint8_t  baby_birthday_day;         //������������:��
	uint8_t  baby_birthday_sex;         //���������Ա�
    uint8_t  retain[3];                 //���뱣��
} phys_period_info_t;

//����
typedef struct
{
	uint8_t   menstrual_period_type;         //������������(��ȫ��/�����ڵ�)
	uint16_t  menstrual_period_year;         //��һ�����¾�:��
	uint8_t   menstrual_period_month;        //��һ�����¾�:��    
	uint8_t   menstrual_period_day;          //��һ�����¾�:��
	uint8_t   menstrual_period_actual_days;  //�¾��ڳ���ʱ�䣬ע��"ʵ�ʾ���"Ϊ0ʱ,ʹ��"��������"
    uint8_t   menstrual_period_oviposit_days;//�ڼ������������     
    uint16_t   remind_type;                   //��������
	uint16_t   remind_days;                   //��������
    
    uint16_t  baby_birthday_days;            //������������    
}next_menstrual_period_t;

typedef struct
{
    uint8_t  remind_info_buf[RMIND_INFO_MAX_SIZE];
    uint16_t data_size;   
    uint8_t  update;  
    
    uint8_t  is_reading;
    uint8_t  read_index; 
    uint8_t  read_len; 
    uint8_t  read_page;    
    uint16_t read_index_buf[5];  
    uint8_t  total_page;
    
    uint8_t  language;
    uint8_t  hours;
    uint8_t  minutes;    
    uint8_t  type;
    uint16_t days;      //WJ.20180816.������������������ڳ���һ���꣬�������Ժ���ʾBug 
}phys_remind_info_t;

#pragma pack()


extern phys_remind_info_t phys_remind_info_s;   //Ů�Թ���������Ϣ

void Phys_InitParam(void);

// ������ݳ�ʼ��
void Phys_ResetInitParam(void);

//Ů�����ݱ���
void Phys_DelaySave(void);

//Ů�����ݹػ�����
void Phys_PowerOffSave(void);

void Phys_ClearRemindInfo(void);

//����Ů�Թ���Э��
void Phys_ParsingProtocol(uint8_t *in_data, uint8_t *out_data);

//��ȡ��һҳ��Ϣ����
uint8_t Phys_DisplayRemindByNextPage(uint8_t reading_flag);


//����δ����Ϣ
uint8_t Phys_ExistUnreadMessage(void);

//ʱ�䴦�������1����һ�Σ�
void Phys_Monitor(void);

//��ȡ���ڴ���ʲô�׶�
uint8_t Phys_GetMenstrualPeriodType(void);

void Phys_UpdataInfo(void);

void Phys_UpdataParam(void);

void Phys_Test(void);

#endif //APP_PHYSIOLOGICAL_PERIOD_H_
