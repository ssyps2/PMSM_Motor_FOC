/*
 * spwm.c
 *
 *      Author: pengyuan
 */

#include "spwm.h"


uint16_t cpuTimer1IntCount;

float32_t sawtooth_upper;
float32_t sawtooth_lower;


// initCPUTimers - This function initializes all three CPU timers to a known state.
void initCPUTimer1(void)
{
    // Initialize timer period to maximum
    CPUTimer_setPeriod(CPUTIMER1_BASE, 0xFFFFFFFF);

    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);

    // Make sure timer is stopped
    CPUTimer_stopTimer(CPUTIMER1_BASE);

    // Reload all counter register with period value
    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);

    // Reset interrupt counter
    cpuTimer1IntCount = 0;
}

//
// configCPUTimer - This function initializes the selected timer to the
// period specified by the "freq" and "period" parameters. The "freq" is
// entered as Hz and the period in uSeconds. The timer is held in the stopped
// state after configuration.
//
void configCPUTimer1(uint32_t cpuTimer, float freq, float period)
{
    // Initialize timer period:
    CPUTimer_setPeriod(cpuTimer, (freq / 1000000 * period)); // 10kHz

    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    CPUTimer_setPreScaler(cpuTimer, 0);

    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    // Additionally, the free and soft bits are set
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);

    //
    // Resets interrupt counters for the cpuTimer
    //
    if (cpuTimer == CPUTIMER1_BASE)
    {
        cpuTimer0IntCount = 0;
    }
}

void SPWM_generate(float32_t sinWave)
{

}
