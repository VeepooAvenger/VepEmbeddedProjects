/********************************Copyright(c)***********************************
**                              Veepoo Tech
**                            COPYRIGHT (C) 2016
**----------------File Info-----------------------------------------------------
** File name:               Vph_CurrentState.h
** Last modified Date:      2017-01-03
** Last SYS_VERSION:
** Descriptions:
**
**------------------------------------------------------------------------------
** Created by:              SQH
** Created date:            2016-01-03
** SYS_VERSION:
** Descriptions:
** Occ:                     malloc: 0 global: 3107 local:8928
*******************************************************************************/

/******************* base value *******************/
#define VPH_STATE_BASE              0

#define VPH_SUCESS                  (VPH_STATE_BASE + 0)
#define VPH_NOT_WEAR                (VPH_STATE_BASE + 1)           // δ���
#define VPH_NOT_VALID               (VPH_STATE_BASE + 3)           // ��ȡ���ݷ�������
#define VPH_NOT_PROCESS             (VPH_STATE_BASE + 4)           // ˯���㷨������
#define VPH_NOT_FOUND               (VPH_STATE_BASE + 5)           // ��ȡ����ʧ��
#define VPH_CURVE_ERROR             (VPH_STATE_BASE + 6)           // ����ɾ����
#define VPH_QUIT_PROCESS            (VPH_STATE_BASE + 7)           // ��ʼ�����˳�˯���жϣ���δ�˳���
/**************************************** enter return ****************************************/
#define VPH_ENTER_BASE              20
//��˯ʱ��������������ֱ���˳�˯���ж�
#define VPH_ENTER_INTERRUPT         (VPH_ENTER_BASE + 0)           // �ڽ�����˯�жϵ�ʱ���������ݲ�����������˯
#define VPH_ENTER_STEP_LEV1         (VPH_ENTER_BASE + 1)           // �ڽ�����˯�жϵ�ʱ�����ڵ�ǰ�Ʋ�����40������˯
#define VPH_ENTER_STEP_LEV2         (VPH_ENTER_BASE + 2)           // ����ɾ����
#define VPH_ENTER_MOV_LEV1          (VPH_ENTER_BASE + 3)           // �ڽ�����˯�жϵ�ʱ�����ڵ�ǰ�˶�������280������˯
#define VPH_ENTER_MOV_LEV2          (VPH_ENTER_BASE + 4)           // ����ɾ����
#define VPH_ENTER_MOV_LEV3          (VPH_ENTER_BASE + 5)           // ����ɾ����
#define VPH_ENTER_MOV_LEV4          (VPH_ENTER_BASE + 6)           // ����ɾ����
#define VPH_ENTER_MOV_LEV5          (VPH_ENTER_BASE + 7)           // ����ɾ����
#define VPH_ENTER_MOV_LEV6          (VPH_ENTER_BASE + 8)           // �ڽ�����˯�жϵ�ʱ����������������������˶�������ǰ�ĺ���һ����֮�ͳ���250������˯
#define VPH_ENTER_MOV_LEV7          (VPH_ENTER_BASE + 9)           // ����ɾ����
#define VPH_ENTER_SAME_VALUE        (VPH_ENTER_BASE + 10)          // ����ɾ����
#define VPH_ENTER_MOV_LEV1_EXT      (VPH_ENTER_BASE + 11)          // ����ɾ����
#define VPH_ENTER_MOV_LEV2_EXT      (VPH_ENTER_BASE + 12)          // ����ɾ����
#define VPH_ENTER_STEP1_EXT         (VPH_ENTER_BASE + 13)          // ����ɾ����

//��˯�жϺ�������ȣ�LEV5��ʾ�Ѿ�������˯�������������ϴ��˳�˯��ʱ��Ƚ�
#define VPH_ENTER_MEET_LEV1         (VPH_ENTER_BASE + 14)          // �˶���С��80�ı����ﲻ��78%
#define VPH_ENTER_MEET_LEV2         (VPH_ENTER_BASE + 15)          // �˶���С��13�ı����ﲻ��50%
#define VPH_ENTER_MEET_LEV3         (VPH_ENTER_BASE + 16)          // ��ʼ���Ϸ����·���4�����˶���û��һ���˶�������20
#define VPH_ENTER_MEET_LEV4         (VPH_ENTER_BASE + 17)          // ��22���㣬û�г����˶�������20
#define VPH_ENTER_MEET_LEV5         (VPH_ENTER_BASE + 18)          // ������˯�ж�
#define VPH_ENTER_SLEEP             (VPH_ENTER_BASE + 19)
//**************************************** quit return ****************************************//
#define VPH_QUIT_BASE               40

//�˳�˯�ߵķ�ʽ
#define VPH_QUIT_STEP_LEV1          (VPH_QUIT_BASE + 1)             // the steps over 15 more than 4 times
#define VPH_QUIT_STEP_LEV2          (VPH_QUIT_BASE + 2)             // steps over 20 more than 3 times
#define VPH_QUIT_STEP_LEV3          (VPH_QUIT_BASE + 3)             // steps over 30 more than 2 times
#define VPH_QUIT_STEP_LEV4          (VPH_QUIT_BASE + 4)             // steps more than 70
#define VPH_QUIT_STEP_LEV5          (VPH_QUIT_BASE + 5)             // sum of steps over 50 when hour >= 6
#define VPH_QUIT_MOV_LEV1           (VPH_QUIT_BASE + 6)             // momentum over 100 continued 4 times
#define VPH_QUIT_MOV_LEV2           (VPH_QUIT_BASE + 7)             // momentum over 150 continued 3 times
#define VPH_QUIT_MOV_LEV3           (VPH_QUIT_BASE + 8)             // momentum over 200 continued 2 times
#define VPH_QUIT_MOV_LEV4           (VPH_QUIT_BASE + 9)             // momentum over 400
#define VPH_QUIT_MOV_LEV5           (VPH_QUIT_BASE + 10)            // sum of momentum over 460(6:00 AM later) or over 700(before 6:00AM)
#define VPH_QUIT_SAME_VALUE         (VPH_QUIT_BASE + 11)            // momentum keep same
#define VPH_QUIT_LOW_BATTERY        (VPH_QUIT_BASE + 12)            // low battery quit
#define VPH_QUIT_FORCE              (VPH_QUIT_BASE + 13)            // quit sleep at 16:00
#define VPH_QUIT_MOV_LEV6           (VPH_QUIT_BASE + 14)            // 6 continues momentum over or equal 25
#define VPH_QUIT_MOV_LEV7           (VPH_QUIT_BASE + 15)            //����4��������3���Ʋ����ڣ����ڣ�15
#define VPH_QUIT_MOV_LEV8           (VPH_QUIT_BASE + 16)            //4����Ʋ�֮�ʹ���45
#define VPH_QUIT_MOV_LEV9           (VPH_QUIT_BASE + 17)            //����4���˶�������65
#define VPH_QUIT_MOV_LEV10           (VPH_QUIT_BASE + 18)           //�ĸ����˶���֮�ʹ���300
#define VPH_QUIT_MOV_LEV11           (VPH_QUIT_BASE + 19)           //����3���˶������ڵ���80

//**************************************** 18:00 - 22:00 return ****************************************//
#define VPH_CHECK_BASE              60
#define VPH_CHECK_STEP_LEV1         (VPH_CHECK_BASE + 0)            //steps more than 25
#define VPH_CHECK_STEP_LEV2         (VPH_CHECK_BASE + 1)            //2 steps more than 10
#define VPH_CHECK_MOV_LEV1          (VPH_CHECK_BASE + 2)            //movements more than 280
#define VPH_CHECK_MOV_LEV2          (VPH_CHECK_BASE + 3)            //2 movements more than 100
#define VPH_CHECK_MOV_LEV3          (VPH_CHECK_BASE + 4)            //6 movements more than 13
#define VPH_CHECK_SAME_VALUE        (VPH_CHECK_BASE + 5)            //6 movements more than 13
//***************************************new_sleep_judge_condition **************************************//
#define VPH_New_Judgement              100
#define VPH_New_Judgement_LEV1         (VPH_New_Judgement + 0)            //10 movements more than 1,break sleep judge
#define VPH_New_Judgement_LEV2         (VPH_New_Judgement + 1)            //steps more than 10 and movements more than 40,break sleep judge;
#define VPH_New_Judgement_LEV3         (VPH_New_Judgement + 2)            //���������Ʋ�����25��
#define VPH_New_Judgement_LEV4         (VPH_New_Judgement + 3)            //����2���˶������ڵ���100
#define VPH_New_Judgement_LEV5         (VPH_New_Judgement + 4)            //һ���˶�������200
#define VPH_New_Judgement_LEV6         (VPH_New_Judgement + 5)            //1������40��
//***************************************���ж���˯��־λ****************************************//
#define VPH_Do_Not_JudgeSleep              200
#define VPH_Do_Not_Judge_flag         (VPH_Do_Not_JudgeSleep + 0)            //7:00-7:25���ж���˯��־
#define VPH_Quit_Time_Judge_flag         (VPH_Do_Not_JudgeSleep + 1)          //�Ƴ�ʱ�����ʾʱ��У��

#define VPH_QUIT_MOV_LEV12                80                                  //1171�����쳣�˳�˯�߱�־
#define VPH_QUIT_MOV_LEV13                (VPH_QUIT_MOV_LEV12 +1)             //1171�����쳣�˳�˯�߱�־
#define VPH_QUIT_MOV_LEV14                (VPH_QUIT_MOV_LEV12 +2)             //1171�����쳣�˳�˯�߱�־
#define VPH_QUIT_DATE_DATA_WRONG           90                                 //(��ɾ��)
#define VPH_FLASH_ZERO_5_MIN               91                                 //flash����һ��0��־
#define VPH_FLASH_ZERO_0_0_MIN             92                                 //(��ɾ��)
#define VPH_DISABLE_SLEEP                  93                                 //(��ɾ��)
#define VPH_BTN_QUIT_SLEEP                 94                                 //�����˳�˯�ߣ�δʹ��
#define VPH_TOO_CLOSE_VALID_SLEEP          208                                //����һ��˯�ߵ�ʱ�������ԣ����ܽ���˯��
#define VPH_BAD_SIG_QUIT_SLEEP             240                                //(��ɾ��)��˯��Ч��־��0��1�ı����ϵ�
#define VPH_QUA_QUIT_SLEEP                 241                                //(��ɾ��)��˯��Ч��־���㷨һ��˯��״̬�����ϵ�
















