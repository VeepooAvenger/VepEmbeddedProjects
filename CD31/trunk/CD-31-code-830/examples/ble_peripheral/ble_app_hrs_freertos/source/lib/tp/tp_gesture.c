/***************************Copyright(c)***********************************
**                       Veepoo Tech
**                  COPYRIGHT (C) 2016-2019
**-----------File Info-----------------------------------------------------
** File        name:     tp_gesture.c
** Last modified Date:   2018-03-23
** Last SYS_VERSION:     0.0.0
** Descriptions:            
**-------------------------------------------------------------------------
** Created  by:          YX
** Created  date:        2018-03-23
** SYS_VERSION:          0.0.0
** Descriptions:         
**-------------------------------------------------------------------------
** Modified by:				
** Modified date:  			          
** SYS_VERSION:                 
** Descriptions:                        
***************************************************************************/

//V1.2�汾�޸ļ�¼��
//���ڣ�20180818 ���ߣ�YX
//1.�Ż�����Ч���������������ܲ�������Ч���������󴥵Ļ���
//�����磺�һ�����������������ʼ���ұ���������������������Ļ�������Ȼ�����󴥣�

//V1.1�汾�޸ļ�¼��
//���ڣ�20180817 ���ߣ�YX
//1.�Ż������ͳ�����ȡλ��
//2.ȥ��һЩû�õ�����

//1.�޸�����ʧ����״̬��ҲҪ�ܽ�������ʶ����������
//2.�޸�����ʧ�ſ�״̬��ҲҪ�ܽ�������ʶ����������
//3.���Ӷ����ݵ��жϣ������ݵ���һ��ֵ��5��������ʶ����������ֹ��ʶ��
//*/�Ӻ��޸�

//V1.0�汾�޸ļ�¼��
//���ڣ�20180425 ���ߣ�YX
//1.��ʼ�汾��ʵ�ֻ�����������������״̬

#include "tp_gesture.h"
#include "tp_cal_data.h"

tp_gesture_info_t tp_rsult_data = {0};      //���صĽ������

static uint32_t tp_data_hz = 0;             //���ݵ�Ƶ��
static uint32_t tp_part_time = 0;           //ƽ�����ƺ��������Ʒָ�ʱ��
static uint32_t tp_data_cnt = 0;            //���ݵĸ���
    
static uint16_t tp_area_max_x = 0;          //TP x���������
static uint16_t tp_area_max_y = 0;          //TP y���������
//static uint16_t tp_area_max_xy = 0;         //TPֱ�������

static uint8_t tp_x_margin_per = 0;         //X����ı�Ե�ٷֱ�
static uint8_t tp_y_margin_per = 0;         //Y����ı�Ե�ٷֱ�
static uint8_t tp_margin_state = 0;         //��Ե��״̬,0:���Ǳ�Ե������1:�ϱ�Ե������2���±�Ե������4:���Ե������8���ұ�Ե����
    
static uint16_t tp_click_max_dist1 = 0;      //������������
static uint16_t tp_click_max_dist2 = 0;      //������������

static uint16_t tp_long_press_time1 = 3000; //����ʱ��1
static uint16_t tp_long_press_time2 = 6000; //����ʱ��2

static uint16_t tp_current_stage = 0;               //�����㷨�����̽׶�
static tp_stage_data_t tp_stage_data_s[100] = {0};  //ÿ�δ������������
static uint8_t tp_stage_data_len = 0;
static uint32_t tp_stage_data_pos = 0;

//YX.20180718.���Ӳ���
//static uint32_t tp_no_data_cnt = 0;     //��������û�����ݵĴ���


/****************************************************************
** Function name:       Tp_CalPointInMargin
** Descriptions:        ������Ƿ��ڱ�Ե����
** input parameters:    coord:�������
** output parameters: 	��
** Returned value:      ��
** Created by:          YX
** Created Date:        2018-03-26
****************************************************************/
uint8_t Tp_CalPointInMargin(coord_info_t coord)
{
    uint8_t margin_result= 0;
    
    //���Ե
	if(Tp_SubAbs(coord.x,0) < tp_x_margin_per*tp_area_max_x/100)
    {
        margin_result |= TP_LEFT_MARGIN_STATE;
    }
    //�ұ�Ե
    if(Tp_SubAbs(coord.x,tp_area_max_x) < tp_x_margin_per*tp_area_max_x/100)
    {
        margin_result |= TP_RIGHT_MARGIN_STATE;
    }
    //�±�Ե
    if(Tp_SubAbs(coord.y,0) < tp_y_margin_per*tp_area_max_y/100)
    {
        margin_result |= TP_DOWN_MARGIN_STATE;
    }
    //�ϱ�Ե
    if(Tp_SubAbs(coord.y,tp_area_max_y) < tp_y_margin_per*tp_area_max_y/100)
    {
        margin_result |= TP_UP_MARGIN_STATE;
    }

	return margin_result;
}

/****************************************************************
** Function name:       Tp_CalTpPointInMargin
** Descriptions:        ������Ƿ��ڱ�Ե����
** input parameters:    coord:Tp�������
** output parameters: 	��
** Returned value:      ��
** Created by:          YX
** Created Date:        2018-03-26
****************************************************************/
uint8_t Tp_CalTpPointInMargin(TP_COORD_DATA_T coord)
{
    coord_info_t coord_s = {0};
    coord_s.x = coord.x;
    coord_s.y = coord.y;
    
    return Tp_CalPointInMargin(coord_s);
}

/****************************************************************
** Function name:       Tp_CalTpTwoPointParam
** Descriptions:        ��������һЩ����������;���
** input parameters:    coord_a:��ʼ���꣬coord_b����������
** output parameters: 	direct������dist������
** Returned value:      ��
** Created by:          YX
** Created Date:        2018-03-26
****************************************************************/
void Tp_CalTpTwoPointParam(TP_COORD_DATA_T coord_a, TP_COORD_DATA_T coord_b, move_direct_info_t* direct, move_dist_info_t* dist)
{
    direct->x_direct = Tp_CompareTwoDataSize(coord_a.x,coord_b.x);
//    direct->y_direct = Tp_CompareTwoDataSize(coord_a.y,coord_b.y);
    direct->y_direct = Tp_CompareTwoDataSize(coord_b.y,coord_a.y);

    dist->x_dist = Tp_SubAbs(coord_a.x,coord_b.x);
    dist->y_dist = Tp_SubAbs(coord_a.y,coord_b.y);
    dist->xy_dist = Tp_CalTwoDataDist(coord_a,coord_b);

    direct->xy_direct = Tp_CompareTwoDataSize(dist->x_dist,dist->y_dist);
}

//����30ms����һ��(���ֽ���״̬ʱ,���Ͻ���һ�μ���,��ֹ���β�������һ�μ�����)
/****************************************************************
** Function name:       Tp_GestureProcess
** Descriptions:        ���ƴ�����
** input parameters:    tp_data:tp����    len:����
** output parameters: 	��
** Returned value:      ��
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
//TP_COORD_DATA_T test_point[2] = {0};
//uint32_t test_len = 0;
uint32_t Tp_GestureProcess(TP_ALL_DATA_T * tp_data ,uint32_t len, uint8_t resv)
{
    uint32_t error_code = 0;
    uint16_t now_mode = 0;
    TP_COORD_DATA_T point_start = {0};
    TP_COORD_DATA_T last_point = {0};
    uint32_t click_max_dist = 0;
    
    tp_rsult_data.tp_state = 0;
	if(len == 0)
	{
		return 4;
	}
    if(tp_current_stage == 0)
    {
        if(tp_data[0].status != TP_START_STATE)
        {
            return 3;
        }
    }
    //��һ������(��һ�ε����һ�����꣬��һ�ν��������ǵ�һ���������)
    if(tp_data_cnt != 0)
    {
        tp_rsult_data.last_coord = tp_rsult_data.now_coord;
    }
    else
    {
        tp_rsult_data.last_coord.x = tp_data[0].coord.x;
        tp_rsult_data.last_coord.y = tp_data[0].coord.y;
    }
    
    tp_data_cnt += len;
    tp_rsult_data.tp_state = 1;
    
    if(tp_data[0].status == TP_START_STATE)
    {
        //������ʼ
        tp_current_stage |= 0x01;
        //�Ƿ��ڱ�Ե
        tp_margin_state = Tp_CalTpPointInMargin(tp_data[0].coord);
        if(tp_margin_state != 0)
        {
            tp_rsult_data.total_mode |= TP_MODE_MARGIN_SLIDE;
            tp_rsult_data.margin_state = tp_margin_state;
        }
        //��ʼ����
        tp_rsult_data.start_coord.x = tp_data[0].coord.x;
        tp_rsult_data.start_coord.y = tp_data[0].coord.y;
    }
    
    if(tp_data[len-1].status == TP_STOP_STATE)
    {
        //���ƽ���
        tp_current_stage |= 0x80;
        tp_rsult_data.tp_state = 2;
//        return error_code;
    }
    
    last_point.x = tp_rsult_data.last_coord.x;
    last_point.y = tp_rsult_data.last_coord.y;
	//�ڵ�����Χ��
    if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
    {
        click_max_dist = tp_click_max_dist1;
    }
    else
    {
        click_max_dist = tp_click_max_dist2;
    }
    
    if(Tp_CalTwoDataDist(last_point,tp_data[len-1].coord) <= click_max_dist)
    {
        now_mode |= TP_MODE_CLICK;
//		//�����ķ�Χ��
//		for(uint32_t i = 0; i < len-2; i++)
//		{
//			if(Tp_CalTwoDataDist(tp_data[i].coord[0],tp_data[i+1].coord[0]) > click_max_dist)
//			{
//                //���ֳ���������Χ�Ķ���,Ӧ����Ϊ����������
//                now_mode |= TP_MODE_SLIDE;
//                now_mode &= ~TP_MODE_CLICK;
//                break;
//			}
//		}
    }
    else
    {
//        test_point[0] = last_point;
//        test_point[1] = tp_data[len-1].coord;
//        test_len = len;
//        tp_rsult_data.now_mode = test_len;
        //�����¼�
        now_mode |= TP_MODE_SLIDE;
    }
    
    tp_rsult_data.now_mode = now_mode;
//    if(now_mode == TP_MODE_SLIDE)
//    {
//        //���㵱���ƶ�����ͻ�������
//        Tp_CalTpTwoPointParam(tp_data[0].coord[0], tp_data[len-1].coord[0], &tp_rsult_data.now_direct, &tp_rsult_data.each_move_dist);
//    }
     
    if((tp_rsult_data.now_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE)
    {
        tp_rsult_data.total_mode |= TP_MODE_SLIDE;
        tp_rsult_data.total_mode &= ~TP_MODE_CLICK;
    }
    else
    {
        if((tp_rsult_data.total_mode & TP_MODE_SLIDE) != TP_MODE_SLIDE)
        {
            tp_rsult_data.total_mode |= TP_MODE_CLICK;
        }
    }

    if((tp_rsult_data.total_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE)
    {
        //���㵱���ƶ�����ͻ�������
        Tp_CalTpTwoPointParam(tp_data[0].coord, tp_data[len-1].coord, &tp_rsult_data.now_direct, &tp_rsult_data.each_move_dist);
    }
    
    //����С��Χ�ƶ����ﵽ��������;�������
    if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
    {
        last_point.x = tp_rsult_data.start_coord.x;
        last_point.y = tp_rsult_data.start_coord.y;
        if(Tp_CalTwoDataDist(last_point,tp_data[len-1].coord) > click_max_dist)
        {
//            test_point[0] = last_point;
//            test_point[1] = tp_data[len-1].coord;
//            test_len = len;
//            tp_rsult_data.now_mode = test_len;
            
            tp_rsult_data.total_mode |= TP_MODE_SLIDE;
            tp_rsult_data.total_mode &= ~TP_MODE_CLICK;
            //�ӿ�ʼ�����������ľ���ȫ���㷨�����ƶ��ľ���
            Tp_CalTpTwoPointParam(last_point, tp_data[len-1].coord, &tp_rsult_data.now_direct, &tp_rsult_data.each_move_dist);
        }
    }
    
    if(tp_data_cnt * 1000/tp_data_hz > tp_part_time)
    {
        //�ﵽƽ�Ƶ����ʱ��
        if((tp_rsult_data.total_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE)
        {
            tp_rsult_data.total_mode |= TP_MODE_TRANSLATION;
        }
    }
    
    //��ǰ����(���һ������)
    tp_rsult_data.now_coord.x = tp_data[len-1].coord.x;
    tp_rsult_data.now_coord.y = tp_data[len-1].coord.y;
    
    //�洢���μ�����
    tp_stage_data_s[tp_stage_data_pos%100].mode = tp_rsult_data.now_mode;
    tp_stage_data_s[tp_stage_data_pos%100].last_coord = tp_rsult_data.last_coord;
    tp_stage_data_s[tp_stage_data_pos%100].now_coord = tp_rsult_data.now_coord;
    tp_stage_data_s[tp_stage_data_pos%100].direct = tp_rsult_data.now_direct;
    tp_stage_data_s[tp_stage_data_pos%100].move_dist = tp_rsult_data.each_move_dist;
    tp_stage_data_pos++;
    tp_stage_data_len++;
    if(tp_stage_data_len > 100)tp_stage_data_len = 100;
    
    //�����ܾ�����ܷ���
    point_start.x = tp_rsult_data.start_coord.x;
    point_start.y = tp_rsult_data.start_coord.y;
    Tp_CalTpTwoPointParam(point_start, tp_data[len-1].coord, &tp_rsult_data.total_direct, &tp_rsult_data.total_move_dist);
    
    if((tp_rsult_data.total_mode & TP_MODE_LONG_PRESS_3S) != TP_MODE_LONG_PRESS_3S)
    {
        if(tp_data_cnt * 1000/tp_data_hz > tp_long_press_time1)
        {
            if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
            {
                //3S����
                tp_rsult_data.total_mode = TP_MODE_LONG_PRESS_3S;
//                tp_rsult_data.total_mode &= ~TP_MODE_CLICK;
            }
        }
    }
        
    if((tp_rsult_data.total_mode & TP_MODE_LONG_PRESS_6S) != TP_MODE_LONG_PRESS_6S)
    {
        
        if(tp_data_cnt * 1000/tp_data_hz > tp_long_press_time2)
        {
            if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
            {
                //3S����
                tp_rsult_data.total_mode = TP_MODE_LONG_PRESS_6S;
//                tp_rsult_data.total_mode &= ~TP_MODE_CLICK;
            }
        }
    }
    return error_code;
}

//��ȡ���ƽ��
/****************************************************************
** Function name:       Tp_GetGesture
** Descriptions:        ��ȡ���ƽ��
** input parameters:    ��
** output parameters: 	��
** Returned value:      ���ƽ���ṹ��
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
tp_gesture_info_t * Tp_GetGesture(void)
{
    return &tp_rsult_data;
}

//����֧�ֵĲ���,ʹ�÷�ʽ����
/****************************************************************
** Function name:       Tp_SetGestureParam
** Descriptions:        ���ƴ�����
** input parameters:    state:֧�ֵ�����
** output parameters: 	��
** Returned value:      ������������
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
tp_result_t Tp_SetGestureParam(uint32_t state)
{
    tp_result_t result_s = {0};
    uint8_t margin_state = 0;
    uint8_t margin_x = 0;
    uint8_t margin_y = 0;
    
    if(state == 0 || tp_rsult_data.tp_state == 0)
    {
        return result_s;
    }
    if(tp_rsult_data.tp_state == 2)
    {
        //����
        if((tp_rsult_data.total_mode & TP_MODE_CLICK) == TP_MODE_CLICK)
        {
            if((state & TP_GEST_CLICK)== TP_GEST_CLICK)
            {
                result_s.mode = TP_MODE_CLICK;
                result_s.start_coord = tp_rsult_data.start_coord;
                result_s.stop_coord = tp_rsult_data.now_coord;
                return result_s;
            }
        }
    }
    
    //����3s
    if((tp_rsult_data.total_mode & TP_MODE_LONG_PRESS_3S) == TP_MODE_LONG_PRESS_3S)
    {
        if((state & TP_GEST_LONG_PRESS_3S)== TP_GEST_LONG_PRESS_3S)//TP_GEST_LONG_PRESS_6S
        {
            result_s.mode = TP_MODE_LONG_PRESS_3S;
            result_s.start_coord = tp_rsult_data.start_coord;
            result_s.stop_coord = tp_rsult_data.now_coord;
            return result_s;
        }
    }
    
    //����6s
    if((tp_rsult_data.total_mode & TP_MODE_LONG_PRESS_6S) == TP_MODE_LONG_PRESS_6S)
    {
        if((state & TP_GEST_LONG_PRESS_6S)== TP_GEST_LONG_PRESS_6S)//TP_GEST_LONG_PRESS_6S
        {
            result_s.mode = TP_MODE_LONG_PRESS_6S;
            result_s.start_coord = tp_rsult_data.start_coord;
            result_s.stop_coord = tp_rsult_data.now_coord;
            return result_s;
        }
    }
    
    if((state & TP_GEST_TRANSLATION)== TP_GEST_TRANSLATION)
    {
        //����ƽ�ƣ��򴰿ڸ��ţ������ƶ������ܷ���;���
        result_s.mode = TP_MODE_TRANSLATION;
        result_s.start_coord = tp_rsult_data.last_coord;
        result_s.stop_coord = tp_rsult_data.now_coord;
        return result_s;
    }
    else
    {
        if(((tp_rsult_data.total_mode & TP_MODE_MARGIN_SLIDE) == TP_MODE_MARGIN_SLIDE)
           && ((tp_rsult_data.total_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE))
        {
            if((state & TP_GEST_LEFT_MARGIN_SLIDE)== TP_GEST_LEFT_MARGIN_SLIDE)
            {
                if((tp_rsult_data.margin_state & TP_LEFT_MARGIN_STATE) == TP_LEFT_MARGIN_STATE)
                {
                    //�鿴�Ƿ������Ե���鿴X������ƶ�����;���
                    margin_state |= 1;
                    margin_x = TP_LEFT_MARGIN_STATE;
                }
            }
            if((state & TP_GEST_RIGHT_MARGIN_SLIDE)== TP_GEST_RIGHT_MARGIN_SLIDE)
            {
                if((tp_rsult_data.margin_state & TP_RIGHT_MARGIN_STATE) == TP_RIGHT_MARGIN_STATE)
                {
                    //�鿴�Ƿ����ұ�Ե���鿴X������ƶ�����;���
                    margin_state |= 1;
                    margin_x = TP_RIGHT_MARGIN_STATE;
                }
            }
            if((state & TP_GEST_UP_MARGIN_SLIDE)== TP_GEST_UP_MARGIN_SLIDE)
            {
                if((tp_rsult_data.margin_state & TP_UP_MARGIN_STATE) == TP_UP_MARGIN_STATE)
                {
                    //�鿴�Ƿ����ϱ�Ե���鿴Y������ƶ�����;���
                    margin_state |= 2;
                    margin_y = TP_UP_MARGIN_STATE;
                }
            }
            if((state & TP_GEST_DOWN_MARGIN_SLIDE)== TP_GEST_DOWN_MARGIN_SLIDE)
            {
                if((tp_rsult_data.margin_state & TP_DOWN_MARGIN_STATE) == TP_DOWN_MARGIN_STATE)
                {
                    //�鿴�Ƿ����±�Ե���鿴Y������ƶ�����;���
                    margin_state |= 2;
                    margin_y = TP_DOWN_MARGIN_STATE;
                }
            }
            
            if((margin_state == 1) || ((margin_state == 3)&&(tp_rsult_data.now_direct.xy_direct == 1)))
            {
                result_s.margin = margin_x;
                result_s.mode = TP_MODE_SLIDE;
                result_s.direct = tp_rsult_data.now_direct.x_direct;
                result_s.dist = tp_rsult_data.each_move_dist.x_dist;
                result_s.start_coord = tp_rsult_data.last_coord;
                result_s.stop_coord = tp_rsult_data.now_coord;
                return result_s;
            }
            else if((margin_state == 2) || ((margin_state == 3)&&(tp_rsult_data.now_direct.xy_direct == 2)))
            {
                result_s.margin = margin_y;
                result_s.mode = TP_MODE_SLIDE;
                result_s.direct = tp_rsult_data.now_direct.y_direct;
                result_s.dist = tp_rsult_data.each_move_dist.y_dist;
                result_s.start_coord = tp_rsult_data.last_coord;
                result_s.stop_coord = tp_rsult_data.now_coord;
                return result_s;
            }

        }
        //��֧�ֱ�Ե����
        if(margin_state == 0)
        {
            //�鿴��ǰģʽ�Ƿ��ǻ��������ǵĻ��������κβ���
            if((tp_rsult_data.total_mode & TP_MODE_SLIDE) == TP_MODE_SLIDE)
            {
                if((state & TP_GEST_X_SLIDE)== TP_GEST_X_SLIDE)
                {
                    //�鿴X�����Ƿ����ƶ����ƶ��ķ���;���ֱ��Ƕ���
                    if(tp_rsult_data.now_direct.x_direct != TP_NULL_SLIDE_STATE)
                    {
                        result_s.margin = margin_x;
                        result_s.mode = TP_MODE_SLIDE;
                        result_s.direct = tp_rsult_data.now_direct.x_direct;
                        result_s.dist = tp_rsult_data.each_move_dist.x_dist;
                        result_s.start_coord = tp_rsult_data.last_coord;
                        result_s.stop_coord = tp_rsult_data.now_coord;
                        return result_s;
                    }
                }
                if((state & TP_GEST_Y_SLIDE)== TP_GEST_Y_SLIDE)
                {
                    //�鿴Y�����Ƿ����ƶ����ƶ��ķ���;���ֱ��Ƕ���
                    if(tp_rsult_data.now_direct.y_direct != TP_NULL_SLIDE_STATE)
                    {
                        result_s.margin = margin_y;
                        result_s.mode = TP_MODE_SLIDE;
                        result_s.direct = tp_rsult_data.now_direct.y_direct;
                        result_s.dist = tp_rsult_data.each_move_dist.y_dist;
                        result_s.start_coord = tp_rsult_data.last_coord;
                        result_s.stop_coord = tp_rsult_data.now_coord;
                        return result_s;
                    }
                }
            }
        }
    }
    return result_s;
}

//��ʼ��(�ɼ����ݵ�Ƶ��:data_hz,��Ե�ٷֱ�x��y:margin_x��margin_y,ƽ�����ƺ�������������ʱ��:time)
/****************************************************************
** Function name:       Tp_GestureInit
** Descriptions:        ���ƴ�����
** input parameters:    data_hz:���ݵ�Ƶ��
                        max_x_coord:x������������
                        max_y_coord:y������������
                        margin_x:x����ı�Ե�ٷֱ�
                        margin_y:y����ı�Ե�ٷֱ�
                        time:ƽ���������������Ƶ�����ʱ��
                        slide_min_dist:������С���루ֱ�߾���ıȣ�
** output parameters: 	��
** Returned value:      ��
** Created by:          YX
** Created Date:        2018-03-23
****************************************************************/
uint32_t Tp_GestureInit(uint32_t data_hz, uint16_t max_x_coord, uint16_t max_y_coord, uint8_t margin_x, uint8_t margin_y, uint32_t time, uint32_t slide_min_dist)
{
//    coord_info_t a = {0};
//    coord_info_t b = {0};
    if(data_hz == 0 || max_x_coord == 0 || max_y_coord == 0 
       || slide_min_dist == 0)
    {
        return 0;
    }
    tp_current_stage = 0;
    tp_area_max_x = max_x_coord;
    tp_area_max_y = max_y_coord;
//    b.x = tp_area_max_x;
//    b.y = tp_area_max_y;
//    tp_area_max_xy = Tp_CalTwoPointDist(a, b);
    tp_click_max_dist1 = slide_min_dist;//tp_area_max_xy/slide_min_dist;
    tp_click_max_dist2 = slide_min_dist;//tp_click_max_dist1;
    tp_data_hz = data_hz;
    tp_part_time = time;
    tp_x_margin_per = margin_x;
    tp_y_margin_per = margin_y;
    tp_data_cnt = 0;
    memset(&tp_rsult_data, 0, sizeof(tp_gesture_info_t));
    tp_stage_data_len = 0;
    tp_stage_data_pos = 0;
    memset(&tp_stage_data_s, 0, sizeof(tp_stage_data_t));
    return 1;
}

