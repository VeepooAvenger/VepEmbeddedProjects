
#ifndef __AFEXX_CALIB_
#define __AFEXX_CALIB_

#include <stdint.h>
#include <math.h>
#include "afe4900_init.h"
#include "afe4900_calibration.h"

extern afe4410_ppg_param_t afe4410_ppg_param_s;

void AFExx_Calib(int32_t gre_da, int32_t amb_da, int32_t red_da, int32_t ird_da);


#endif // !__AFEXX_CALIB_