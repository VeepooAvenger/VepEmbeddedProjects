/********************************Copyright(c)***********************************
**                          Veepoo Tech
**                        COPYRIGHT (C) 2016
** Modified by:             YQW
** Modified date:           2019-01-23
** DES :                    support afe4410
** SYS_VERSION:             0x01010200
** ******************************************************************************
** Version  :               0x01010201_20190517
** Author   :               YQW
** Des      :               1.增加红外光检测方式(接口)
                            2.修复了绿光寄存器参数获取错误的bug.
** ******************************************************************************
** Version  :               0x01010202_20190605
** Author   :               YQW
** Des      :               1.修复结果信息获取函数的bug.
**-----------------------------------------------------------------------------
*******************************************************************************/
#ifndef  __SIGNAL_WEAR_DETECTION_AFE4410_H__
#define  __SIGNAL_WEAR_DETECTION_AFE4410_H__
#include <stdint.h>
#include <stdlib.h>


#pragma anon_unions

/*
* Memory Usage:(Version 0x01010202)
*   Global: 6(Bytes)
*   Const:  9*2 = 18(Bytes)
*   Malloc: 0(Bytes)
*   Local:  53(Bytes)
*/

/*
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Attention:
* The following contents are not allowed to be modified for Embedded software engineer
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

/*
* macros definitions 
*/
#define SWD_VERSION             ( 0x01010202    )
/* detection type macros definitions    */
#define GREEN_WEAR_DETECT       ( 10            )   /* green signal test mode   */ 
#define IRED_WEAR_DETECT        ( 11            )   /* ired signal test mode    */
/* detection result macros definitions  */
#define WEAR_PASS               ( 0             )   /* wear pass                */
#define WEAR_NOT_PASS           ( 1             )   /* wear not pass            */
/* detection mode  macros definitions   */ 
#define HEART_DETECT_NORMAL     ( 0             )   /* nrmal heart detect   */
#define HEART_DETECT_CALI       ( 1             )   /* green calibration    */
#define OXY_DETECT_CALC         ( 2             )   /* 红外加环境光         */

// wear detection result infomation structure definition 
typedef struct tag_wear_detection_info
{
    int8_t  wear_station                     ;   /* [WEAR_PASS]:wear pass,[WEAR_NOT_PASS]:wear not pass,other:no definition */
    int8_t  err_code                         ;   /* reversed                                                                */
    float   ctr_value                        ;   /* reversed                                                                */
}wear_detection_info_t;

typedef struct tag_afe4410_wear_input_parameter
{
    uint8_t            wear_detect_mode         ;   /* see "detection type macros definitions"  GREEN_WEAR_DETECT or IRED_WEAR_DETECT, default 'GREEN_WEAR_DETECT' */
    union
    {
        struct
        {
            const int32_t*  green_data_input_buff    ;
            int8_t          green_dac_value          ;   /* green + amb mode light, dac register value              */
            uint8_t         green_gain_value         ;   /* green + amb mode light, gain register value             */
            uint8_t         green_elec_value         ;   /* green + amb mode light, elec value                      */
        };
        struct
        {
            const int32_t*  ired_data_input_buff     ;
            int8_t          ired_dac_value           ;   /* ired + amb mode light, ired dac register value          */
            uint8_t         ired_gain_value          ;   /* ired + amb mode light, ired gain register value         */
            uint8_t         ired_elec_value          ;   /* ired + amb mode light, ired elec value                  */
        };
    };
    const int32_t*     amb_data_input_buff           ;   /* amb sampling data */
    uint8_t            heart_detect_mode             ;   /* see "detection mode  macros definitions" , reversed, default:0   */
    uint8_t            level                         ;   /* default reversed                                                 */
    uint8_t            ifs_dac                       ;   /*  scale */
    uint8_t            amb_dac_mode                  ;   /* [0,3,5,7], 0 not open amb cut mode . not 0,open amb cut mode     */
    float              ctr_threshold                 ;   /* green: 100,  ired: reversed */
}afe4410_wear_input_parameter_t;


/*
* functions declatrations for external call
*/
extern void         Signal_WearInit         (   void                                                                ); 
extern uint32_t     Swd_GetVerion           (   void                                                                );
extern void         Signal_WearDetect       (   afe4410_wear_input_parameter_t* swd_afe4410_wear_input_parameter_s  );
extern void         Get_Wear_Detection_Info (   wear_detection_info_t*          gwdi_wear_detection_info_s          );

#endif


