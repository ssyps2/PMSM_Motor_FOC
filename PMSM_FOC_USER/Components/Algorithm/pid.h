/*
 * pid.h
 *
 *      Author: pengyuan
 */

#ifndef COMPONENTS_ALGORITHM_PID_H_
#define COMPONENTS_ALGORITHM_PID_H_


typedef struct {

    /* Controller gains */
    float Kp;
    float Ki;
    float Kd;

    /* Derivative low-pass filter time constant */
    float tau;

    /* Output limits */
    float limMin;
    float limMax;

    /* Integrator limits */
    float limMinInt;
    float limMaxInt;

    /* Sample time (in seconds) */
    float T;

    /* Controller "memory" */
    float integrator;
    float prevError;            /* Required for integrator */
    float differentiator;
    float prevMeasurement;      /* Required for differentiator */

    /* Controller output */
    float out;

} PIDController;

void  PIDController_Init(PIDController *pid);
float PIDController_Update(PIDController *pid, float setpoint, float measurement);


#endif /* COMPONENTS_ALGORITHM_PID_H_ */
