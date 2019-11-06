/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2016
**--------------File Info-------------------------------------------------------
** File name:               pave_infoalgorithm.h
** Last modified Date:       2016-07-27
** Last SYS_VERSION:         01.01.00.01
** Descriptions:
**
**------------------------------------------------------------------------------
** Created by:              FWT
** Created date:            2018-10-22
** SYS_VERSION:                 01.01.00.01
** Descriptions:
**------------------------------------------------------------------------------
** Modified by:             FWT
** Modified date:           2018-10-22
** SYS_VERSION:             0x01010002
** Descriptions:            1.change the struct define
							2.malloc��8000 local��4206 global��656
**------------------------------------------------------------------------------
** Modified by:             FWT
** Modified date:           2018-10-22
** SYS_VERSION:             0x01010003
** Descriptions:            1.change the struct define
2.malloc��8000 local��4206 global��656
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-02-28
** SYS_VERSION:             0x01010012
** Descriptions:            1.�滻Ѫѹ�㷨��0x02000101_20190226�汾
                            2.��test_mode_t�Ķ����vph_bp_24bit.h �ƶ������ļ���
                            3.�滻˯���㷨1��0x01010002_20190228�汾
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-01
** SYS_VERSION:             0x01010013
** Descriptions:            1.����PPG�������㷨�㷨(0x01010104_20190301),���ۺϵ����ļ�(pwave_infoalgorithm.c)��������㷨�����߼�
                            2.����ʱ�ü�������㷨,���ٴ�����
                            3.������һЩ�����ȡ��ַʹ�÷�ʽ(������鲻��Ҫ��������ȡ��ַ)
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-02
** SYS_VERSION:             0x01010014
** Descriptions:            1.�޸ĺ�������Ч������� (0,50] ----> [0,50]
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-05(���԰汾,�������ڷ���)
** SYS_VERSION:             0x01010015
** Descriptions:            1.�޸���ں���Pwave_InfoAlgorithm�������������,������һ�������ṩ���ִ������Ĵ���ʵʱֵ�Ľӿ�;
                            2.������������ΪSi1182ʱ���ýӿڵ�����Ϊsi1182_register_parameter_t*����,�Ҵ˲������ܴ���NULL;
                            3.�����޸�������㷨�����㷨��Si1182����㷨����ʵ��;
                            4.0x01010015�汾�������ڹ�Ƕ��ʽ���д����Ƕ��ʽ���򣬲����ڷ���.
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-06
** SYS_VERSION:             0x01010016
** Descriptions:            1.��������㷨��0x01030003_20190306�汾(���ԭ���ش�仯,ʹ�ú����ctr�������)
                            2.�ڵ����ļ�Pwave_InfoAlgorithm.c��������Ӧ�����޸�
                            3.������ں���Pwave_InfoAlgorithm�Ľӿڷ����仯,��Ƕ��ʽ���������Ա������Ӧ���䡣
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-16
** SYS_VERSION:             0x01010018
** Descriptions:            1.sleep_alg------->0x01010003_20190316 . 
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-03-21
** SYS_VERSION:             0x01010019
** Descriptions:            1.resp_alg------->0x01010105_20190321
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-03
** SYS_VERSION:             0x01010100
** Descriptions:            1.�޸�Ϊ��ʽ�汾�������״η���.
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-18
** SYS_VERSION:             0x01010101
** Descriptions:            1.�����ڲ�Ѫѹ�㷨��02000103_20190402�汾,�Ż�˽��ģʽ�µ�Ѫѹӳ�䷽ʽ.
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-19
** SYS_VERSION:             0x01010102
** Descriptions:            1.�����ڲ�˯���㷨1��0x01010004_20190419�汾,�޸Ķ���ʶ���һ���ӻ���
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-22
** SYS_VERSION:             0x01010200
** Descriptions:            1.�����ڲ�˯���㷨1��0x01010005_20190422�汾,����ʶ��ص���һ�汾������һ���ӻ��ܵ��޸�(sleep_state)
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-27
** SYS_VERSION:             0x01010202
** Descriptions:            1.�����ڲ�Ѫѹ�㷨��0x02000104_20190427�汾
                            2.�����ڲ��������㷨��0x01010106_20190427�汾
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-04-30
** SYS_VERSION:             0x01010203
** Descriptions:            1.�����ڲ�����㷨�㷨��0x00000004_20190429�汾,�������̹�+������������ⷽʽ;
                            2.si1182_register_parameter_t �ṹ�����Ͷ�������������㷨��Ҫ�ĵƹ�ģʽ�ֽ�,��Ӧ�жϴ���ҲҪ����((light_type == SI1182_WEAR_DETECTION_MODE_GREEN_AMB));
                            3.Pwave_InfoAlgorithm���Ⱥ�����,���Si1182��������ƹ�ģʽ�������������
                            4.���㷨��������Eϵ��(Si1182)����+������������ⷽʽ,����Ҫ�Եƹ�ģʽ�ֽڽ�������(light_type == SI1182_WEAR_DETECTION_MODE_IRED_AMB)��
                            5.���ݴ�����Ҫ�Ķ����ݼ�pwave_infoalgorithm.h�ļ���ĩβ(0x01010203),���ڻ����ĵ���ʽ������ȷ.
                            6.��Ӧ��������ʱwarnningҪ��,��˯���㷨1����,�ڲ��ļ����б���ʱwarnning�Ѿ�ȫ��������
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-05-07
** SYS_VERSION:             0x01010204
** Descriptions:            1.����TP�Ż�,ȡ���Ի���������ָ���ǿ�Ƽ��(��Ӧ����㷨���µ�0x01030005�汾)
20190508:ʹ�ú���������ظ����йض���
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-05-24
** SYS_VERSION:             0x01010205
** Descriptions:            1.�����ڲ�˯���㷨1��(0x01010006_20190524)�汾,���ڲ����㷨Ҳ��Ӧ�Ż�,����240Bytesȫ�ֱ���ռ��;
                            2.�޸�Si1182l�̹���ģʽ��CTR����(30-->28)
                            3.��Ӧ�ĵ����ظ���,��Ȼʹ��<<Si1182_PPG ��ʹ��˵����_V1.0.pdf>>
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-06-04
** SYS_VERSION:             0x01010206
** Descriptions:            1.�����ڲ�˯���㷨1��(01010200_20190604)�汾.
                            2.��ĳЩ�ṹ��ӱ�.h�ļ���ɾ��.
                            3.��Ӧ�ĵ����ظ���,��Ȼʹ��<<Si1182_PPG ��ʹ��˵����_V1.0.pdf>>
**------------------------------------------------------------------------------
** Modified by:             YQW
** Modified date:           2019-06-15
** SYS_VERSION:             0x01010207
** Descriptions:            1.�����ڲ�����㷨��(01030006_20190615)�汾,������E28��Ŀ����.
                            2.�������������һ�����ֲ���,ʹ�÷�����Ƕ��ʽ�Ķ�<<Si1182_PPG ��ʹ��˵����_V1.1.pdf>>
*******************************************************************************/
#ifndef __PWAVE_INFOALGORITHM_H__
#define __PWAVE_INFOALGORITHM_H__

#include "stdlib.h"
#include "stdint.h"
#include "pedometer.h"

#pragma anon_unions

#define PPG_SENSOR_TYPE         ("SI1182")
#define SI1182_PPG_ALG_VERSION  (0x01010207)
#define BOD_MAX_WAVE_NUM        (60)             /* finding max pulse num  12 to 18DH_2017_0607 */



/* Si1182 light type macros definitions */
#ifndef SI1182_WEAR_DETECTION_MODE_IRED_AMB
#define SI1182_WEAR_DETECTION_MODE_IRED_AMB   (0x00)                                /* ired  + amb    (default value)   */
#define SI1182_WEAR_DETECTION_MODE_IRED       (0x00)                                /* ired only                        */
#define SI1182_WEAR_DETECTION_MODE_GREEN_AMB  (0x01)                                /* green + amb    (E02)             */
#define SI1182_WEAR_DETECTION_MODE_GREEN      (0x01)                                /* green only                       */
#define SI1182_WEAR_DETECTION_MODE_GREEN_IRED (0x02)                                /* green + ired   (reserve )        */
#define SI1182_WEAR_DETECTION_MODE_MAX        SI1182_WEAR_DETECTION_MODE_GREEN
#endif


/* Si1182 led layout type macros definitions  */
#ifndef SI1182_LAYOUT_TYPE_0
#define SI1182_LAYOUT_TYPE_0            (0)     /* symmetry 4 leds         */
#define SI1182_LAYOUT_TYPE_1            (1)     /* no symmetry 3 leds  ,E28 projict   */
#define SI1182_LAYOUT_TYPE_2            (2)     /* invalid in this version */
#define SI1182_LAYOUT_TYPE_VALID_MIN    SI1182_LAYOUT_TYPE_0
#define SI1182_LAYOUT_TYPE_VALID_MAX    SI1182_LAYOUT_TYPE_1
#endif


#ifndef PWAVE_INFO_T
#define PWAVE_INFO_T
typedef struct pwave_info
{
    uint8_t move_state                                  ;         /* �˶����ĵȼ���0��1��2��3;2bit */
    uint8_t sleep_state                                 ;        
    uint8_t turn_body_flag                              ;     
    uint8_t hr_state                                    ;           
    uint8_t beats_early                                 ;        
    uint8_t wrong_flag                                  ;         
    uint8_t slp_result_1min                             ;
}pwave_info_t;
#endif


#ifndef DATA_DC_CHAR_T
#define DATA_DC_CHAR_T
typedef struct tag_data_dc_char
{
    /* ���ֱ������   */
    uint8_t low_satu_flag                               ;
    uint8_t high_satu_flag                              ;
    uint8_t shake_flag                                  ;
    uint8_t mutation_flag                               ;
    uint8_t drift_flag                                  ;
    float   amp_mean                                    ;
    float   peak_diff_max                               ;
}data_dc_char_t;
#endif

/*
 * blood pressure private mode setting ( should match with vph_bp_24bit.h)
 */
#ifndef TEST_MODE_T
#define TEST_MODE_T
typedef struct tag_test_mode
{
    uint8_t test_mode   ;    /* 0:normal mode, 1:private mode                           */
    uint8_t set_sp      ;    /* Set the sp for testing.Setting valid when test_mode = 1 */
    uint8_t set_dp      ;    /* Set the dp for testing.Setting valid when test_mode = 1 */
}test_mode_t;
#endif 

/*
* si1182 register parameter(si1182 wear Alg need) structure
*/
#ifndef __SI1182_REGISTER_PARAMETER_T__
#define __SI1182_REGISTER_PARAMETER_T__
typedef struct tag_si1182_register_parameter
{
    uint8_t light_type  ;                     /* see "Si1182 light type macros definitions" in  pwave_infoalgorithm.h */
    uint8_t layout_type ;                     /* see "Si1182 led layout type macros definitions" in pwave_infoalgorithm.h*/
    union 
    {
        struct
        {
            uint8_t i_ird           ;       /* �����LED�����̶�    */
            uint8_t adc_range_ird   ;       /* �����ADC����1       */
            uint8_t clk_div_ird     ;       /* �����ADC����2       */
        };
        struct
        {
            uint8_t i_green         ;       /* �̹�LED�����̶�    */
            uint8_t adc_range_green	;       /* �̹�ADC����1       */
            uint8_t clk_div_green   ;       /* �̹�ADC����2       */
        };
    };
}si1182_register_parameter_t;
#endif


/*
 * PPG Alg result structure definitions
*/
#ifndef PWAVE_ONEMIN_INFO_T
#define PWAVE_ONEMIN_INFO_T
typedef struct pwave_onemin_info
{
    /* ˯�����     */
    uint8_t  one_min_result         ;   /* ÿ����˯��״̬����Ӧ�ճ�����˯��״̬1~5��      */
    uint8_t  one_min_wrong_flag     ;   /* ÿ����˯�߱�־����Ӧ�ճ�����˯��״̬6��        */
    /* Ѫѹ��� */
    uint8_t  bp_flag                ;   /* Ѫѹ�����Ч��־λ                             */
    uint8_t  high_bp_per_sec        ;   /* ÿ����Ѫѹ-��ѹ	                              */
    uint8_t  low_bp_per_sec         ;   /* ÿ����Ѫѹ-��ѹ	                              */
    uint8_t  high_bp_per_25sec      ;   /* ÿ25����Ѫѹ-��ѹ	                          */
    uint8_t  low_bp_per_25sec       ;   /* ÿ25����Ѫѹ-��ѹ                              */
    uint8_t  high_bp_per_5min       ;   /* ÿ�����Ѫѹ-��ѹ����Ӧ�ճ�����Ѫѹ-��ѹ��     */
    uint8_t  low_bp_per_5min        ;   /* ÿ�����Ѫѹ-��ѹ����Ӧ�ճ�����Ѫѹ-��ѹ��     */
    /* �������             */
    uint8_t  hr_ppg_per_sec_result  ;   /* ÿ��HR                                         */
    uint8_t  hr_ppg_per_min_result  ;   /* ÿ����HR����Ӧ�ճ���������1~5��                */
    /* HRV���        */
    uint8_t  hrv_ppg_per_sec_result ;   /* ÿ��HRV                                        */
    int32_t  heart_load_1m          ;   /* ���ฺ�� define in hrv alg result              */
    /* RR����         */
    uint8_t  rr_per_sec_result      ;   /* ÿ��RR                                         */
    uint8_t  rr_per_6sec_result     ;   /* ÿ6��RR                                        */
                                        
    uint8_t  wear_flag              ;   /* �����־                                       */
    /* ������        */
    uint8_t  resp_rate_1sec         ;   /* 0~50,>50--invalid value,[0,50]--valid value    */
    uint8_t  resp_rate_1min         ;   /* 0~50,>50--invalid value,[0,50]--valid value    */
    uint8_t  resp_rate_5min         ;   /* 0~50,>50--invalid value,[0,50]--valid value    */
    /* ������,����λ */
    uint8_t  ppg_err1               ;
    uint32_t ppg_err2               ;
    uint32_t ppg_err3               ;
    uint32_t ppg_err4               ;
    uint32_t ppg_err5               ;
}pwave_onemin_info_t;
#endif


/*
* function declarations for external call
*/
extern void                 Pwave_InfoInit      ( void                                          );
extern uint16_t             Pwave_InfoAlgorithm ( const int32_t* green_data                  ,
                                                  const int32_t* ird_data                    ,  
                                                  const int32_t* amb_data                    ,  /* NULL in Si1182 type */
                                                  uint16_t       time_point                  ,
                                                  uint32_t       times_cnt                   ,
                                                  move_index_t*  movement_struct_s           ,
                                                  test_mode_t*   vbd_mode_info               ,
                                                  void*          sensor_register_parameter      /* ������������ΪSi1182ʱ,�˲�������Ϊ si1182_register_parameter_t* */
                                                );
extern pwave_onemin_info_t* Get_PwaveInfo       ( void                                          );
extern uint32_t             Get_PwaveVersion    ( void                                          );

#endif // !__PWAVE_INFOALGORITHM_H__
