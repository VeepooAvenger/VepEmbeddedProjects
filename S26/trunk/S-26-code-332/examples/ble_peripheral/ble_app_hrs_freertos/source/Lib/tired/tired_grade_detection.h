/********************************Copyright(c)***********************************
**                           Veepoo Tech
**                         COPYRIGHT (C) 2014-2016
**--------------File Info------------------------------------------------------
** File name:               Tired_GradeDetection.h
**-----------------------------------------------------------------------------
** Modified by:             YQW
** Created date:            2018-05-23
** SYS_VERSION:             01010100
** Descriptions:            (4410)50Hz,24bit的疲劳度算法的第一个版本.
**-----------------------------------------------------------------------------
*******************************************************************************/
#ifndef __TIRED_DETECTION_H__
#define __TIRED_DETECTION_H__

#include <stdbool.h>
#include <stdint.h>

/*
* Memeory Usage(01010100_20190523)
* **********************************
* Global : 306(Bytes)
* Malloc : 800(Bytes)
* Local  : 223(Bytes)
*/

/* macros definitions */
#define VPH_TIRED_VERSION       (   0x01010100                              )
#define TIRED_SAMPLE_RATE       (   50                                      )
#define TIRED_SAMPLE_BITS       (   24                                      )
#define TIRED_CALC_SECONDS      (   4                                       )
#define TIRED_RENEW_SECONDS     (   4                                       )
#define TIRED_PULSE_MIN_AMP     (   150                                     )
#define TIRED_SIGNAL_LEN        (   TIRED_CALC_SECONDS*TIRED_SAMPLE_RATE    )
#define TIRED_MAX_PULSE_NUM     (   18                                      )
#define TIRED_MAX_HRV_NUM       (   150                                     )
#define TIRED_MAX_COUNT_TIMES   (   15                                      )

#ifndef TIRED_FLAG_CALCULATING
#define TIRED_FLAG_CALCULATING  (   0                                       )
#define TIRED_FLAG_CALCULATIED  (   1                                       )
#define TIRED_FLAG_INVALID      (   2                                       )
#define TIRED_FLAG_MALLOC_ERROR (   3                                       )
#define TIRED_FLAG_MIN          (   TIRED_FLAG_CALCULATING                  )
#define TIRED_FLAG_MAX          (   TIRED_FLAG_MALLOC_ERROR                 )
#endif 

/* algorithm result structure definitions */
typedef struct tag_tired_info
{
    uint8_t tired_flag      ;   /* 20170301 tired_flag = 0 表示计算状态；tired_flag = 1，表示给出计算结果；tired_flag = 2 表示测试无效；tired_flag = 3 表示内存分配不成功   */
    uint8_t tired_grade     ;   /* 疲劳度等级        */
    uint8_t wave_station    ;   /* 波形状态          */
    uint8_t progress        ;   /* 0~15              */
}tired_info_t;

/* make interface unchanged , make everyone happy*/
#define Get_TiredInfo       Tired_GetResult
#define Get_TiredVersion    Tired_GetAlgVersion

/*
* algorithm functions declarations for external call
*/
extern  void        Tired_DectInit          (   void                                );
extern  void        Tired_GradeDetection    (   const int32_t*  input_tired_data    );
extern  void        Tired_GetResult         (   tired_info_t*   tgr_tired_info_s    );
extern  uint32_t    Tired_GetAlgVersion     (   void                                );

#endif /* !__TIRED_DETECTION_H__ */

