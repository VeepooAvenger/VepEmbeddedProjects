/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     gui_traverse.h
** Last modified Date:   2017-09-21
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2017-09-21
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/
#ifndef _GUI_TRAVERSE_H_
#define _GUI_TRAVERSE_H_

extern uint8_t test_dis_ui_flg;

//设置UI遍历状态
void Gui_SetTraverseUiState(uint8_t state);

//获取UI遍历状态
uint8_t Gui_GetTraverseUiState(void);

//遍历切换界面参数
void Gui_EventAndTaskTraverse(uint8_t state);

//主任务事件界面处理
void Gui_ThreadTestManage(void);




#endif

