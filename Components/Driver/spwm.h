/*
 * spwm.h
 *
 *      Author: pengyuan
 */

#ifndef COMPONENTS_DRIVER_SPWM_H_
#define COMPONENTS_DRIVER_SPWM_H_


#include "driverlib.h"
#include "device.h"


void SPWM_initGPIOs(void);
void SPWM_generate_3level(float32_t *sinWave, float32_t sawtooth_upper, float32_t sawtooth_lower);


#endif /* COMPONENTS_DRIVER_SPWM_H_ */
