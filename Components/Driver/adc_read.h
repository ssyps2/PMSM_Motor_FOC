/*
 * adc_read.h
 *
 *      Author: pengyuan
 */

#ifndef COMPONENTS_DRIVER_ADC_READ_H_
#define COMPONENTS_DRIVER_ADC_READ_H_


#include "driverlib.h"
#include "device.h"


// Function prototype
void initADCs(void);
void initADCSOCs(void);
void ADC_updateReading(uint16_t *dcVolt, uint16_t *currA, uint16_t *currB);


#endif /* COMPONENTS_DRIVER_ADC_READ_H_ */
