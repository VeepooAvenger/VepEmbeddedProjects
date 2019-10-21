#ifndef __Afe_4404Calib_
#define __Afe_4404Calib_
#include <stdint.h>

#ifndef __BLOOD_OXYGENDETECTION_
#define IFS_DAC                  	(2)
#define AMB_DAC_MODE							(3)
#define DAC_IFS_STEPS							(0.125f)
#else
#include "Blood_OxygenDetection.h"
#endif


//#define ILED_IFS_50_HR_CALIB
#define ILED_IFS_100_HR_CALIB

#ifdef ILED_IFS_100_HR_CALIB
// ��ʼ���ĵ����̶� 100mA��ѡ��35�̶�  50mA��ѡ��70�̶�
#define INIT_ILED_SCALES_HR_CALIB      (35)
// ILED��1�̶ȶ�Ӧ���ٺ����ĵ���
#define ILED_IFS_STEPS_HR_CALIB        (0.392f)
#endif // DEBUG

#ifdef ILED_IFS_50_HR_CALIB 
#define INIT_ILED_SCALES_HR_CALIB      (70)
// ILED��1�̶ȶ�Ӧ���ٺ����ĵ���
#define ILED_IFS_STEPS_HR_CALIB        (0.195f)
#endif


#define TARGER_DAC_ADC_VALUE_HR_CALIB  (140000)


extern uint8_t                     current_set;
extern uint8_t                     rf_set;
extern uint8_t                     dac_set;

void Hr_CalibInit(void);

uint8_t Ad_StatuFlag(int32_t * sig, uint16_t sig_len);

uint8_t Hr_Calib
(
    const int32_t   sig,
    const int32_t   amb_signal
);




 
#endif
    

