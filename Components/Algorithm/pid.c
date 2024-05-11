/*
 * pid.c
 *
 *      Author: pengyuan
 */


#include "PID.h"

void PIDController_Init(PIDController *pid) {

    /* Clear controller variables */
    pid->Kd = 0.0f;

    pid->T = 0.0001f; // Switching period

    pid->integrator = 0.0f;
    pid->prevError  = 0.0f;
    pid->differentiator  = 0.0f;
    pid->prevMeasurement = 0.0f;

    pid->out = 0.0f;
}

float PIDController_Update(PIDController *pid, float setpoint, float measurement)
{
    float error = setpoint - measurement;

    // Proportional
    float proportional = pid->Kp * error;

    // Integral
    pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

    // Anti-wind-up via integrator clamping
    if (pid->integrator > pid->limMaxInt) pid->integrator = pid->limMaxInt;
    else if (pid->integrator < pid->limMinInt) pid->integrator = pid->limMinInt;


    // Compute output and apply limits
    pid->out = proportional + pid->integrator;

    if (pid->out > pid->limMax) pid->out = pid->limMax;
    else if (pid->out < pid->limMin) pid->out = pid->limMin;

    // Store error and measurement for later use
    pid->prevError       = error;
    pid->prevMeasurement = measurement;

    // Return controller output
    return pid->out;
}

