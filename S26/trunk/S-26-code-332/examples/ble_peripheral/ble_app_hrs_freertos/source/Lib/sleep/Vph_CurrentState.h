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
**
*******************************************************************************/

/******************* base value *******************/
#define VPH_STATE_BASE              0

#define VPH_SUCESS                  (VPH_STATE_BASE + 0)
#define VPH_NOT_WEAR                (VPH_STATE_BASE + 1)           // don't wear
#define VPH_NOT_VALID               (VPH_STATE_BASE + 3)           // the read data is not valid  
#define VPH_NOT_PROCESS             (VPH_STATE_BASE + 4)           // at that time, sleep algorithm do not calculate
#define VPH_NOT_FOUND               (VPH_STATE_BASE + 5)           // read data faild
#define VPH_CURVE_ERROR             (VPH_STATE_BASE + 6)           // draw curve error
#define VPH_QUIT_PROCESS            (VPH_STATE_BASE + 7)           // quit sleep process, but do not quit sleep
/**************************************** enter return ****************************************/
#define VPH_ENTER_BASE              20
//入睡时满足以下下条件直接退出睡眠判断
#define VPH_ENTER_INTERRUPT         (VPH_ENTER_BASE + 0)           // the judge paramter is not continues
#define VPH_ENTER_STEP_LEV1         (VPH_ENTER_BASE + 1)           // more than 20 steps 
#define VPH_ENTER_STEP_LEV2         (VPH_ENTER_BASE + 2)           // more than 10 steps at 2 times
#define VPH_ENTER_MOV_LEV1          (VPH_ENTER_BASE + 3)           // movement value more than 280
#define VPH_ENTER_MOV_LEV2          (VPH_ENTER_BASE + 4)           // movement 90 - 280 more than 2 times
#define VPH_ENTER_MOV_LEV3          (VPH_ENTER_BASE + 5)           // movement 150 - 280 more than 3 times
#define VPH_ENTER_MOV_LEV4          (VPH_ENTER_BASE + 6)           // movement 80 - 150 more than 5 times
#define VPH_ENTER_MOV_LEV5          (VPH_ENTER_BASE + 7)           // one of the continued momentum, one bigger than 100, the leave is less than 50  
#define VPH_ENTER_MOV_LEV6          (VPH_ENTER_BASE + 8)           // sum of two continued momentum is not bigger than 250
#define VPH_ENTER_MOV_LEV7          (VPH_ENTER_BASE + 9)           // continued momentum bigger than 13 at 6 times
#define VPH_ENTER_SAME_VALUE        (VPH_ENTER_BASE + 10)          // continued 18 momentum keep the value
#define VPH_ENTER_MOV_LEV1_EXT      (VPH_ENTER_BASE + 11)          // the times that momentum less than 10 not bigger than 50% at the first hour
#define VPH_ENTER_MOV_LEV2_EXT      (VPH_ENTER_BASE + 12)          // sum of two continued momentum is not bigger than 200
#define VPH_ENTER_STEP1_EXT         (VPH_ENTER_BASE + 13)          // the times that steps more than 10 is not mote than 3
       

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





