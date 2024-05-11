/*
 * adc_read.c
 *
 *      Author: pengyuan
 */

#include "adc_read.h"


// Function to configure and power up ADCs
void initADCs(void)
{
    // Set resolution and signal mode (see #defines above) and load corresponding trims.
    ADC_setMode(ADCA_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    ADC_setMode(ADCB_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    ADC_setMode(ADCC_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);

    // Set pulse positions to late
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(ADCB_BASE, ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(ADCC_BASE, ADC_PULSE_END_OF_CONV);

    // Power up the ADC and then delay for 1 ms
    ADC_enableConverter(ADCA_BASE);
    ADC_enableConverter(ADCB_BASE);
    ADC_enableConverter(ADCC_BASE);

    DEVICE_DELAY_US(1000);
}

// Function to configure SOCs of ADCs
void initADCSOCs(void)
{
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_CPU1_TINT0, ADC_CH_ADCIN3, 15);
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_CPU1_TINT0, ADC_CH_ADCIN3, 15);
    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_CPU1_TINT0, ADC_CH_ADCIN3, 15);
}

void ADC_updateReading(uint16_t *dcVolt, uint16_t *currA, uint16_t *currB)
{
    // Store results
    *dcVolt = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);    // DC voltage
    *currA  = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER0);    // Phase A current
    *currB  = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER0);    // Phase B current
}
