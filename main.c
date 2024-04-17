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

    // ISRs for each CPU Timer interrupt
    Interrupt_register(INT_TIMER0, &TIMER0ISR);
    Interrupt_register(INT_TIMER1, &TIMER1ISR);

    // Initialize the CPU Timers
    initCPUTimers(CPUTIMER0_BASE);
    initCPUTimers(CPUTIMER1_BASE);

    // Configure CPU-Timer 0 and 1 (in uSeconds)
    configCPUTimers(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 100);   // 10kHz
    configCPUTimers(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ, 5);     // 200kHz

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in configCPUTimer and initCPUTimers, the below settings must also
    // be updated.
    //
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    CPUTimer_enableInterrupt(CPUTIMER1_BASE);

    //
    // Enables CPU interrupt which are connected to CPU-Timer 0 and CPU-Timer 1
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    Interrupt_enable(INT_TIMER0);
    Interrupt_enable(INT_TIMER1);

    //
    // Starts CPU-Timer 0 and CPU-Timer 1
    //
    CPUTimer_startTimer(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER1_BASE);

    //
    // Set up ADCs, initializing the SOCs
    //
    initADCs();
    initADCSOCs();

    //
    // Initialize PID controllers and parameters
    //
    PIDController_Init(&pidCurrent_Q);
    PIDController_Init(&pidCurrent_D);
    PIDController_Init(&pidSpeed);
    PIDController_Init(&pidBalanceDC);

    PIDParameters_Init();

    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    EINT;
    ERTM;

    // IDLE loop
    while(1)
    {
    }
}

//
// Interrupt callback function of Timer 0
//
__interrupt void TIMER0ISR(void)
{
    ++cpuTimer0IntCount;

    real_time0 = (float32_t)cpuTimer0IntCount / 10000;

    // Update current & voltage ADC reading
    ADC_updateReading(&dcVoltage_adc, &currA_adc, &currB_adc);

    // Calculate the real current & voltage values
    dcVoltage_1 = (float32_t)dcVoltage_adc * DC_VOLTAGE / 4096
    dcVoltage_2 = DC_VOLTAGE - dcVoltage_1;
    currA = (currA_adc * (3.3f/4095.0f) * (2.0f/3.0f) * (9.9f/4.3f) - 2.5f) / 0.1f;
    currB = (currB_adc * (3.3f/4095.0f) * (2.0f/3.0f) * (9.9f/4.3f) - 2.5f) / 0.1f;

    // Update motor angle and speed reading


    // Speed control loop & DC balance


    // Current control loop (FOC)




//    sinWave[0] = sinf(100 * M_PI * real_time0);
//    sinWave[1] = sinf(100 * M_PI * real_time0 + M_PI/3);
//    sinWave[2] = sinf(100 * M_PI * real_time0 - M_PI/3);

    // Acknowledge the interrupt
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}


//
// Interrupt callback function of Timer 1
//
__interrupt void TIMER1ISR(void)
{
    if(timer1Flag == 1) ++cpuTimer1IntCount;
    else if (timer1Flag == 0) --cpuTimer1IntCount;

    //real_time1 = (float32_t)cpuTimer1IntCount / 1000000;

    if(cpuTimer1IntCount >= 10) timer1Flag = 0;         // hit upper limit, go down
    else if (cpuTimer1IntCount == 0) timer1Flag = 1;    // hit lower limit, go up

    // Generate the sawtooth waves
    sawtooth_upper = (float32_t)cpuTimer1IntCount / 10;
    sawtooth_lower = sawtooth_upper - 1;

    // Generate PWM to control each IGBT
    SPWM_generate_3level(sinWave, sawtooth_upper, sawtooth_lower);
}


// initCPUTimers - This function initializes all three CPU timers to a known state.
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
    else if(cpuTimer == CPUTIMER1_BASE)
    {
        cpuTimer1IntCount = 0;
        timer1Flag = 1;
    }
}


//
// configCPUTimer - This function initializes the selected timer to the
// period specified by the "freq" and "period" parameters. The "freq" is
// entered as Hz and the period in uSeconds. The timer is held in the stopped
// state after configuration.
//
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
    pidCurrent_Q.Kp = 26.1f;
    pidCurrent_Q.Ki = 4952.4f;

    pidCurrent_Q.limMin = 0.0f;
    pidCurrent_Q.limMax = 100.0f;

    pidCurrent_Q.limMinInt = 0.0f;
    pidCurrent_Q.limMaxInt = 100.0f;

    /* ----------- Current Controller: D Axis----------- */
    pidCurrent_D.Kp = 26.1f;
    pidCurrent_D.Ki = 4952.4f;

    pidCurrent_D.limMin = 0.0f;
    pidCurrent_D.limMax = 100.0f;

    pidCurrent_D.limMinInt = 0.0f;
    pidCurrent_D.limMaxInt = 100.0f;

    /* ----------- Speed Controller -----------*/
    pidSpeed.Kp = 0.1f;
    pidSpeed.Ki = 0.01f;

    pidSpeed.limMin = 0.0f;
    pidSpeed.limMax = 10.0f;

    pidSpeed.limMinInt = 0.0f;
    pidSpeed.limMaxInt = 10.0f; // Maximum current

    /* ----------- DC Balance Controller ----------- */
    pidBalanceDC.Kp = 0.1f;
    pidBalanceDC.Ki = 0.01f;

    pidBalanceDC.limMin = 0.0f;
    pidBalanceDC.limMax = 5.0f;

    pidBalanceDC.limMinInt = 0.0f;
    pidBalanceDC.limMaxInt = 5.0f;
}
