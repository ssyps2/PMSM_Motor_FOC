/**
 * main.c
 *
 *      Author: pengyuan
 */


#include "main.h"

//
// Main
//
void main(void)
{
    // Initialize device clock and peripherals
    Device_init();

    // Disable pin locks and enable internal pullups.
    Device_initGPIO();
    SPWM_initGPIOs();

    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    Interrupt_initModule();

    // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
    Interrupt_initVectorTable();

    // ISRs for CPU Timer interrupt and ePWMs
    Interrupt_register(INT_TIMER0, &TIMER0ISR);

    Interrupt_register(INT_EPWM1, &epwm1ISR);
    Interrupt_register(INT_EPWM2, &epwm2ISR);
//    Interrupt_register(INT_EPWM3, &epwm3ISR);
//    Interrupt_register(INT_EPWM4, &epwm4ISR);
//    Interrupt_register(INT_EPWM5, &epwm5ISR);
//    Interrupt_register(INT_EPWM6, &epwm6ISR);

    // Initialize the CPU Timers
    initCPUTimers(CPUTIMER0_BASE);

    // Freeze clock to PWM
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    // Initialize ePWMs
    initEPWM(EPWM1_BASE);
    initEPWM(EPWM2_BASE);
//    initEPWM(EPWM3_BASE);
//    initEPWM(EPWM4_BASE);
//    initEPWM(EPWM5_BASE);
//    initEPWM(EPWM6_BASE);

    // Enable clock to PWM
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    // Configure CPU-Timer 0 (in uSeconds)
    configCPUTimers(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 100);   // 10kHz

    // Enables Timer interrupt and EPWM interrupt
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);

    Interrupt_enable(INT_TIMER0);

    Interrupt_enable(INT_EPWM1);
    Interrupt_enable(INT_EPWM2);
    Interrupt_enable(INT_EPWM3);
    Interrupt_enable(INT_EPWM4);
    Interrupt_enable(INT_EPWM5);
    Interrupt_enable(INT_EPWM6);

    // Starts CPU-Timer 0
    CPUTimer_startTimer(CPUTIMER0_BASE);

    // Set up ADCs, initializing the SOCs
    initADCs();
    initADCSOCs();

    // Initialize PID controllers and parameters
    PIDController_Init(&pidCurrent_Q);
    PIDController_Init(&pidCurrent_D);
    PIDController_Init(&pidSpeed);
    PIDController_Init(&pidBalanceDC);

    PIDParameters_Init();

    // Test GPIO Init
    testGPIOPin_Init();

    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    EINT;
    ERTM;

    // IDLE loop
    while(1)
    {
    }
}


// Interrupt callback function of Timer 0
__interrupt void TIMER0ISR(void)
{
    real_time0 = (float32_t)(++cpuTimer0IntCount) / 10000;

    // Update current & voltage ADC reading
    ADC_updateReading(&dcVoltage_adc, &currA_adc, &currB_adc);

    // Calculate the real current & voltage values
    dcVoltage_1 = (float32_t)dcVoltage_adc * DC_VOLTAGE / 4096;
    currA = (currA_adc * (3.3f/4095.0f) * (2.0f/3.0f) * (9.9f/4.3f) - 2.5f) / 0.1f;
    currB = (currB_adc * (3.3f/4095.0f) * (2.0f/3.0f) * (9.9f/4.3f) - 2.5f) / 0.1f;

    // Update motor angle and speed reading
//    angle_measure = getRotorAngle();          // in rad/s
//    speed_measure = getRotorAngle();          // in RPM

    // Speed control loop
    current_set_Q = PIDController_Update(&pidSpeed, speed_set, speed_measure);

    // DC balance control
    //current_set_balanceDC = PIDController_Update(&pidBalanceDC, dcVoltage_1, DC_VOLTAGE/2);

    // Current control loop: update 3 phase voltage_set
    currentLoopCalc();  // -> get phaseVoltage_set[3]

    // Only for testing:
    phaseVoltage_set[0] = EPWM_TIMER_TBPRD * sinf(100 * M_PI * real_time0);
    phaseVoltage_set[1] = EPWM_TIMER_TBPRD * sinf(100 * M_PI * real_time0 + M_PI/3);
    phaseVoltage_set[2] = EPWM_TIMER_TBPRD * sinf(100 * M_PI * real_time0 - M_PI/3);

    GPIO_togglePin(61); // testing the frequency in oscilloscope

    // Acknowledge the interrupt
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}


// initCPUTimers - This function initializes all three CPU timers to a known state
void initCPUTimers(uint32_t cpuTimer)
{
    // Initialize timer period to maximum
    CPUTimer_setPeriod(cpuTimer, 0xFFFFFFFF);

    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_setPreScaler(cpuTimer, 0);

    // Make sure timer is stopped
    CPUTimer_stopTimer(cpuTimer);

    // Reload all counter register with period value
    CPUTimer_reloadTimerCounter(cpuTimer);

    // Reset interrupt counter
    if(cpuTimer == CPUTIMER0_BASE)
    {
        cpuTimer0IntCount = 0;
    }
}


// initEPWMs - Configure ePWM1 to ePWM6
void initEPWM(uint32_t epwmTimer)
{
    // Set up counter mode (EPWMCLK = 100 MHz)
    EPWM_setTimeBaseCounterMode(epwmTimer, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_disablePhaseShiftLoad(epwmTimer);
    EPWM_setClockPrescaler(epwmTimer,
                           EPWM_CLOCK_DIVIDER_4,
                           EPWM_HSCLOCK_DIVIDER_10);    // 100 MHz / (4*10) = 2.5 MHz

    // Set-up TBCLK to 10 kHz
    EPWM_setTimeBasePeriod(epwmTimer, EPWM_TIMER_TBPRD); // EPWM_TIMER_TBPRD = 125
    EPWM_setPhaseShift(epwmTimer, 0U);
    EPWM_setTimeBaseCounter(epwmTimer, 0U);

    // Set Actions
    EPWM_setActionQualifierAction(epwmTimer,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(epwmTimer,
                                  EPWM_AQ_OUTPUT_A,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(epwmTimer,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_LOW,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(epwmTimer,
                                  EPWM_AQ_OUTPUT_B,
                                  EPWM_AQ_OUTPUT_HIGH,
                                  EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);

    // Set up shadowing
    EPWM_setCounterCompareShadowLoadMode(epwmTimer,
                                         EPWM_COUNTER_COMPARE_A,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareShadowLoadMode(epwmTimer,
                                         EPWM_COUNTER_COMPARE_B,
                                         EPWM_COMP_LOAD_ON_CNTR_ZERO);


    EPWM_setInterruptSource(epwmTimer, EPWM_INT_TBCTR_ZERO);
    EPWM_enableInterrupt(epwmTimer);
    EPWM_setInterruptEventCount(epwmTimer, 6U);
}


__interrupt void epwm1ISR(void)
{
    // Update Compare values
    EPWM_setCounterCompareValue(EPWM1_BASE,
                                EPWM_COUNTER_COMPARE_A,
                                EPWM_TIMER_TBPRD + phaseVoltage_set[2]);
    EPWM_setCounterCompareValue(EPWM1_BASE,
                                EPWM_COUNTER_COMPARE_B,
                                phaseVoltage_set[2]);

    // Clear INT flag for this timer
    EPWM_clearEventTriggerInterruptFlag(EPWM1_BASE);

    // Acknowledge interrupt group
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
}

__interrupt void epwm2ISR(void)
{
    // Update Compare values
    EPWM_setCounterCompareValue(EPWM2_BASE,
                                EPWM_COUNTER_COMPARE_A,
                                EPWM_TIMER_TBPRD + phaseVoltage_set[2]);
    EPWM_setCounterCompareValue(EPWM2_BASE,
                                EPWM_COUNTER_COMPARE_B,
                                phaseVoltage_set[2]);

    // Clear INT flag for this timer
    EPWM_clearEventTriggerInterruptFlag(EPWM2_BASE);

    // Acknowledge interrupt group
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
}


// configCPUTimer - This function initializes the selected timer to the
// period specified by the "freq" and "period" parameters. The "freq" is
// entered as Hz and the period in uSeconds. The timer is held in the stopped
// state after configuration.
void configCPUTimers(uint32_t cpuTimer, float32_t freq, float32_t period)
{
    // Initialize timer period:
    CPUTimer_setPeriod(cpuTimer, (freq / 2000000 * period));

    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    // Additionally, the free and soft bits are set
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);
}

//
// Initialize the PID parameters
//
void PIDParameters_Init(void)
{
    /* ----------- Current Controller: Q Axis----------- */
    pidCurrent_Q.Kp = 2.08f;
    pidCurrent_Q.Ki = 1328.39f;

    pidCurrent_Q.limMin = 0.0f;
    pidCurrent_Q.limMax = 100.0f;

    pidCurrent_Q.limMinInt = 0.0f;
    pidCurrent_Q.limMaxInt = 100.0f;

    /* ----------- Current Controller: D Axis----------- */
    pidCurrent_D.Kp = 2.08f;
    pidCurrent_D.Ki = 1328.39f;

    pidCurrent_D.limMin = 0.0f;
    pidCurrent_D.limMax = 100.0f;

    pidCurrent_D.limMinInt = 0.0f;
    pidCurrent_D.limMaxInt = 100.0f;

    /* ----------- Speed Controller -----------*/
    pidSpeed.Kp = 0.2f;
    pidSpeed.Ki = 1.27f;

    pidSpeed.limMin = 0.0f;
    pidSpeed.limMax = 10.0f;

    pidSpeed.limMinInt = 0.0f;
    pidSpeed.limMaxInt = 10.0f; // Maximum current

    /* ----------- DC Balance Controller ----------- */
    pidBalanceDC.Kp = 0.1f;
    pidBalanceDC.Ki = 1.0f;

    pidBalanceDC.limMin = 0.0f;
    pidBalanceDC.limMax = 5.0f;

    pidBalanceDC.limMinInt = 0.0f;
    pidBalanceDC.limMaxInt = 5.0f;
}


// Current loop calculation
void currentLoopCalc(void)
{
    float32_t electrical_angle_measure = angle_measure * POLE_PAIRS;
    float32_t electrical_speed_measure = speed_measure * POLE_PAIRS;

    // Clark Transformation
    float32_t curr_Alpha1 = currA;
    float32_t curr_Beta1  = (currA + 2.0f * currB) * sqrtf(3) / 3.0f;

    // Park Transformation
    float32_t curr_D1 = cosf(electrical_angle_measure)*curr_Alpha1
            + sinf(electrical_angle_measure)*curr_Beta1;
    float32_t curr_Q1 = -sinf(electrical_angle_measure)*curr_Alpha1
            + cosf(electrical_angle_measure)*curr_Beta1;

    // Parallel PI loop: D-axis and Q-axis
    float32_t curr_D2 = PIDController_Update(&pidCurrent_D, current_set_D, curr_D1)
            - (curr_Q1 * STATOR_L * electrical_speed_measure);
    float32_t curr_Q2 = PIDController_Update(&pidCurrent_Q, current_set_Q, curr_Q1)
            + (FLUX * electrical_speed_measure) + (curr_D1 * STATOR_L * electrical_speed_measure);

    // Inverse Park Transformation
    float32_t curr_Alpha2 = cosf(electrical_angle_measure) * curr_D2 - sinf(electrical_angle_measure) * curr_Q2;
    float32_t curr_Beta2  = sinf(electrical_angle_measure) * curr_D2 + cosf(electrical_angle_measure) * curr_Q2;

    // Inverse Clark Transformation
    phaseVoltage_set[0] = EPWM_TIMER_TBPRD * (curr_Alpha2 + current_set_balanceDC) / DC_VOLTAGE;
    phaseVoltage_set[1] = EPWM_TIMER_TBPRD * (0.5f * (sqrtf(3) * curr_Beta2 - curr_Alpha2) + current_set_balanceDC) / DC_VOLTAGE;
    phaseVoltage_set[2] = EPWM_TIMER_TBPRD * (-0.5 * (curr_Alpha2 + sqrtf(3) * curr_Beta2) + current_set_balanceDC) / DC_VOLTAGE;
}

void testGPIOPin_Init(void)
{
    // Timer 0 frequency monitor
    GPIO_setPadConfig(61, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_61_GPIO61);
    GPIO_setDirectionMode(61, GPIO_DIR_MODE_OUT);
    GPIO_writePin(61, 0);
}
