/**
 * main.c
 *
 *      Author: pengyuan
 */


#include "adc_read.h"
#include "spwm.h"


// Variables
uint16_t cpuTimer0IntCount;
uint16_t adcResult[4] = {0};


// Function prototype
__interrupt void TIMER0ISR(void);
void initCPUTimer0(void);
void configCPUTimer0(uint32_t cpuTimer, float freq, float period);


void main(void)
{
    // Initialize device clock and peripherals
    Device_init();

    // Disable pin locks and enable internal pullups.
    Device_initGPIO();

    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    Interrupt_initModule();

    // Initialize the PIE vector table with pointers to the shell Interrupt Service Routines (ISR).
    Interrupt_initVectorTable();

    // ISRs for each CPU Timer interrupt
    Interrupt_register(INT_TIMER0, &TIMER0ISR);

    // Initialize the CPU Timers
    initCPUTimer0();

    // Set up ADCs, initializing the SOCs to be triggered by software
    initADCs();
    initADCSOCs();

    // Configure CPU-Timer 0 (in uSeconds)
    configCPUTimer0(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 100);
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    Interrupt_enable(INT_TIMER0);
    CPUTimer_startTimer(CPUTIMER0_BASE);

    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    EINT;
    ERTM;

    // IDLE loop
    while(1)
    {
    }
}

//
// Interrupt callback function
//
__interrupt void TIMER0ISR(void)
{
    ++cpuTimer0IntCount;

    // Update current & voltage reading
    ADC_updateReading(adcResult);

    // Update motor angle and speed reading


    // Control logic and calculations


    // Acknowledge the interrupt
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}


// initCPUTimers - This function initializes all three CPU timers to a known state.
void initCPUTimer0(void)
{
    // Initialize timer period to maximum
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);

    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);

    // Make sure timer is stopped
    CPUTimer_stopTimer(CPUTIMER0_BASE);

    // Reload all counter register with period value
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);

    // Reset interrupt counter
    cpuTimer0IntCount = 0;
}

//
// configCPUTimer - This function initializes the selected timer to the
// period specified by the "freq" and "period" parameters. The "freq" is
// entered as Hz and the period in uSeconds. The timer is held in the stopped
// state after configuration.
//
void configCPUTimer0(uint32_t cpuTimer, float freq, float period)
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
    if (cpuTimer == CPUTIMER0_BASE)
    {
        cpuTimer0IntCount = 0;
    }
}
