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
#define VPH_NOT_WEAR                (VPH_STATE_BASE + 1)           // 未佩戴
#define VPH_NOT_VALID               (VPH_STATE_BASE + 3)           // 读取数据发生错误
#define VPH_NOT_PROCESS             (VPH_STATE_BASE + 4)           // 睡眠算法不计算
#define VPH_NOT_FOUND               (VPH_STATE_BASE + 5)           // 读取数据失败
#define VPH_CURVE_ERROR             (VPH_STATE_BASE + 6)           // （已删除）
#define VPH_QUIT_PROCESS            (VPH_STATE_BASE + 7)           // 开始进行退出睡眠判断（还未退出）
/**************************************** enter return ****************************************/
#define VPH_ENTER_BASE              20
//入睡时满足以下下条件直接退出睡眠判断
#define VPH_ENTER_INTERRUPT         (VPH_ENTER_BASE + 0)           // 在进行入睡判断的时候由于数据不连续不能入睡
#define VPH_ENTER_STEP_LEV1         (VPH_ENTER_BASE + 1)           // 在进行入睡判断的时候由于当前计步超过40不能入睡
#define VPH_ENTER_STEP_LEV2         (VPH_ENTER_BASE + 2)           // （已删除）
#define VPH_ENTER_MOV_LEV1          (VPH_ENTER_BASE + 3)           // 在进行入睡判断的时候由于当前运动量超过280不能入睡
#define VPH_ENTER_MOV_LEV2          (VPH_ENTER_BASE + 4)           // （已删除）
#define VPH_ENTER_MOV_LEV3          (VPH_ENTER_BASE + 5)           // （已删除）
#define VPH_ENTER_MOV_LEV4          (VPH_ENTER_BASE + 6)           // （已删除）
#define VPH_ENTER_MOV_LEV5          (VPH_ENTER_BASE + 7)           // （已删除）
#define VPH_ENTER_MOV_LEV6          (VPH_ENTER_BASE + 8)           // 在进行入睡判断的时候由于两个连续的五分钟运动量（当前的和上一个）之和超过250不能入睡
#define VPH_ENTER_MOV_LEV7          (VPH_ENTER_BASE + 9)           // （已删除）
#define VPH_ENTER_SAME_VALUE        (VPH_ENTER_BASE + 10)          // （已删除）
#define VPH_ENTER_MOV_LEV1_EXT      (VPH_ENTER_BASE + 11)          // （已删除）
#define VPH_ENTER_MOV_LEV2_EXT      (VPH_ENTER_BASE + 12)          // （已删除）
#define VPH_ENTER_STEP1_EXT         (VPH_ENTER_BASE + 13)          // （已删除）

//入睡判断函数的深度，LEV5表示已经满足入睡条件，还需与上次退出睡眠时间比较
#define VPH_ENTER_MEET_LEV1         (VPH_ENTER_BASE + 14)          // 运动量小于80的比例达不到78%
#define VPH_ENTER_MEET_LEV2         (VPH_ENTER_BASE + 15)          // 运动量小于13的比例达不到50%
#define VPH_ENTER_MEET_LEV3         (VPH_ENTER_BASE + 16)          // 开始点上方和下方共4各点运动量没有一个运动量大于20
#define VPH_ENTER_MEET_LEV4         (VPH_ENTER_BASE + 17)          // 后22个点，没有出现运动量大于20
#define VPH_ENTER_MEET_LEV5         (VPH_ENTER_BASE + 18)          // 进入入睡判定
#define VPH_ENTER_SLEEP             (VPH_ENTER_BASE + 19)
//**************************************** quit return ****************************************//
#define VPH_QUIT_BASE               40

//退出睡眠的方式
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
#define VPH_QUIT_MOV_LEV7           (VPH_QUIT_BASE + 15)            //连续4个点中有3个计步大于（等于）15
#define VPH_QUIT_MOV_LEV8           (VPH_QUIT_BASE + 16)            //4个点计步之和大于45
#define VPH_QUIT_MOV_LEV9           (VPH_QUIT_BASE + 17)            //连续4个运动量大于65
#define VPH_QUIT_MOV_LEV10           (VPH_QUIT_BASE + 18)           //四个点运动量之和大于300
#define VPH_QUIT_MOV_LEV11           (VPH_QUIT_BASE + 19)           //连续3个运动量大于等于80

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
#define VPH_New_Judgement_LEV3         (VPH_New_Judgement + 2)            //连续两个计步大于25步
#define VPH_New_Judgement_LEV4         (VPH_New_Judgement + 3)            //连续2个运动量大于等于100
#define VPH_New_Judgement_LEV5         (VPH_New_Judgement + 4)            //一个运动量大于200
#define VPH_New_Judgement_LEV6         (VPH_New_Judgement + 5)            //1个大于40步
//***************************************不判断入睡标志位****************************************//
#define VPH_Do_Not_JudgeSleep              200
#define VPH_Do_Not_Judge_flag         (VPH_Do_Not_JudgeSleep + 0)            //7:00-7:25不判断入睡标志
#define VPH_Quit_Time_Judge_flag         (VPH_Do_Not_JudgeSleep + 1)          //推出时间和显示时间校正

#define VPH_QUIT_MOV_LEV12                80                                  //1171波形异常退出睡眠标志
#define VPH_QUIT_MOV_LEV13                (VPH_QUIT_MOV_LEV12 +1)             //1171波形异常退出睡眠标志
#define VPH_QUIT_MOV_LEV14                (VPH_QUIT_MOV_LEV12 +2)             //1171波形异常退出睡眠标志
#define VPH_QUIT_DATE_DATA_WRONG           90                                 //(已删除)
#define VPH_FLASH_ZERO_5_MIN               91                                 //flash出现一行0标志
#define VPH_FLASH_ZERO_0_0_MIN             92                                 //(已删除)
#define VPH_DISABLE_SLEEP                  93                                 //(已删除)
#define VPH_BTN_QUIT_SLEEP                 94                                 //按键退出睡眠，未使用
#define VPH_TOO_CLOSE_VALID_SLEEP          208                                //与上一段睡眠的时间间隔不对，不能进入睡眠
#define VPH_BAD_SIG_QUIT_SLEEP             240                                //(已删除)入睡无效标志，0、1的比例较低
#define VPH_QUA_QUIT_SLEEP                 241                                //(已删除)入睡无效标志，算法一的睡眠状态比例较低
















