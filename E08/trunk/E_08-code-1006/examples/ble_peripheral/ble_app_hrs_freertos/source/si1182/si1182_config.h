#ifndef __SI117X_CONFIG_H
#define __SI117X_CONFIG_H


#include "si1182_functions.h"


#define SI117X_MAX_ADDRESS          PARAM_ECG_LDCONFIG      //si117x最大参数地址

#define	SI1182_FIFO_BUFFER_SIZE		(100)					// si1182的fifo读取数据缓存大小
#define SI1182_LD_BUFFER_SIZE		(20)					// si1182的ld读取数据缓存大小
#define SI1182_FIFO_LAST_DATA_SIZE	(10)					// si1182的fifo未解析数据的保存空间
#define SI1182_FIFO_DATA_TEST_SIZE	(5)						// si1182的数据越界测试


typedef struct
{
    uint8_t     rw_flag;                        //读写寄存器的标志，0读全部寄存器，1读一个，2写一个
    uint8_t     address;                        //寄存器的地址
    uint8_t     value;                          //寄存器的值
    uint8_t     parameter[SI117X_MAX_ADDRESS + 1];  //si117x参数
}si117x_config_t;


// 原始数据控制参数
typedef struct
{
    uint32_t   time;                        // 记录si1171中断的时间
    uint32_t   address;
    uint32_t   index;
    uint8_t    minute;
    uint8_t    second;
    uint8_t    raw_data_0;                  // 1分钟原始数据补0个数
    uint8_t    counter;                     // 1分钟原始数据计数
    uint8_t    copy_num;                    // 拷贝计数
    uint8_t    init_address;                // 初始化flash地址
    uint8_t    second_0_num;               // 预防0s时刻，出现2个si117x数据中断
    uint8_t    si1182_enable:1;             // 开启标志
    uint8_t    si1182_suspend:1;            // 意外灭灯标志
    uint8_t    si1182_err:1;                 // 错误标志
    uint8_t    rsv:5;

}raw_data_control_t;


#endif
