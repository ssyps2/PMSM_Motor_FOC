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

// Motor Parameters (low-inertia servo BSM90N-175AA)
#define DC_VOLTAGE      300.0f
#define POLE_PAIRS      4
#define STATOR_L        0.00415f
#define TORQUE_CONST    0.853f
#define FLUX            (2 * TORQUE_CONST / (3 * POLE_PAIRS))

//
#define EPWM_TIMER_TBPRD 125

//
// Variables - SPWM & DC Balance
//
uint16_t epwmTimerIntCount[6] = {0};

float32_t phaseVoltage_set[3] = {0.};

float32_t dcVoltage_1 = 0;
float32_t currA = 0;
float32_t currB = 0;

float32_t real_time0 = 0.;

uint32_t cpuTimer0IntCount = 0;

uint16_t dcVoltage_adc = 0;
uint16_t currA_adc = 0;
uint16_t currB_adc = 0;

//
// Variables - Motor
//
uint32_t encoder_measure = 0;
float32_t angle_measure = 0.0f;
float32_t speed_measure = 0.0f;

const float32_t speed_set = 300.0f;

//
// Variables - Controller
//
PIDController pidCurrent_Q;
PIDController pidCurrent_D;
PIDController pidSpeed;
PIDController pidBalanceDC;

const float32_t current_set_D = 0.0f;

float32_t current_set_Q;
float32_t current_set_balanceDC;


//
// Function prototype
//
__interrupt void TIMER0ISR(void);
__interrupt void epwm1ISR(void);
__interrupt void epwm2ISR(void);
__interrupt void epwm3ISR(void);
__interrupt void epwm4ISR(void);
__interrupt void epwm5ISR(void);
__interrupt void epwm6ISR(void);

void initCPUTimers(uint32_t cpuTimer);
void initEPWM(uint32_t epwmTimer);
void configCPUTimers(uint32_t cpuTimer, float32_t freq, float32_t period);
void PIDParameters_Init(void);
void currentLoopCalc(void);
void testGPIOPin_Init(void);


#endif /* MAIN_H_ */
