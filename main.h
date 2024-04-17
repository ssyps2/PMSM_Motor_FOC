/*
 * main.h
 *
 *      Author: pengyuan
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "pid.h"
#include "adc_read.h"
#include "sensor_read.h"
#include "spwm.h"
#include <math.h>

#define DC_VOLTAGE  320.0f


//
// Variables - SPWM & DC Balance
//
float32_t sinWave[3] = {0.};
float32_t sawtooth_upper = 0.;
float32_t sawtooth_lower = 0.;

float32_t dcVoltage_1 = 0;
float32_t dcVoltage_2 = 0;
float32_t currA = 0;
float32_t currB = 0;

float32_t real_time0 = 0.;
//float32_t real_time1 = 0.;

uint32_t cpuTimer0IntCount = 0;
uint32_t cpuTimer1IntCount = 0;

uint16_t dcVoltage_adc = 0;
uint16_t currA_adc = 0;
uint16_t currB_adc = 0;

uint8_t timer1Flag = 1; // down=0, up=1;

//
// Variables - Motor
//
uint32_t encoder = 0;
float32_t angle = 0., speed = 0.;

//
// Variables - Controller
//
PIDController pidCurrent_Q;
PIDController pidCurrent_D;
PIDController pidSpeed;
PIDController pidBalanceDC;

const uint32_t  f_sw = 10e3;
const float32_t Ts = 1/f_sw;


//
// Function prototype
//
__interrupt void TIMER0ISR(void);
__interrupt void TIMER1ISR(void);
void initCPUTimers(uint32_t cpuTimer);
void configCPUTimers(uint32_t cpuTimer, float32_t freq, float32_t period);
void PIDParameters_Init(void);


#endif /* MAIN_H_ */
