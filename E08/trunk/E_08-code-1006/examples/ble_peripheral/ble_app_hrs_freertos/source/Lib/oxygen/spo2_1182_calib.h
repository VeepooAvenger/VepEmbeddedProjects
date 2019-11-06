#pragma once
#ifndef __SPO2_1182CALIB_
#define __SPO2_1182CALIB_

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "Blood_OxygenDetection.h"

void Spo2_1182CalibInit(
    detection_char_info_t * calib_in_s,
    bod_result_info_t * calib_out_s,
    uint8_t set_mode,
    uint8_t sw_avg_flag
);

uint8_t Spo2_1182CalibDetc(
    const int32_t * red_da,
    const int32_t * ird_da,
    detection_char_info_t * calib_in_s,
    bod_result_info_t * calib_out_s,
    uint32_t sc_calib_cnt,
    uint8_t sw_avg_flag
);






#endif
