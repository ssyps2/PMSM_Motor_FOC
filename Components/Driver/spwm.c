/*
 * spwm.c
 *
 *      Author: pengyuan
 */

#include "spwm.h"

void SPWM_initGPIOs(void)
{
    // A1
    GPIO_setPadConfig(11, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_11_GPIO11);
    GPIO_setDirectionMode(11, GPIO_DIR_MODE_OUT);
    GPIO_writePin(11, 0);

    // A2
    GPIO_setPadConfig(10, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_10_GPIO10);
    GPIO_setDirectionMode(10, GPIO_DIR_MODE_OUT);
    GPIO_writePin(10, 0);

    // A3
    GPIO_setPadConfig(9, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_9_GPIO9);
    GPIO_setDirectionMode(9, GPIO_DIR_MODE_OUT);
    GPIO_writePin(9, 0);

    // A4
    GPIO_setPadConfig(8, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_8_GPIO8);
    GPIO_setDirectionMode(8, GPIO_DIR_MODE_OUT);
    GPIO_writePin(8, 0);

    // B1
    GPIO_setPadConfig(5, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_5_GPIO5);
    GPIO_setDirectionMode(5, GPIO_DIR_MODE_OUT);
    GPIO_writePin(5, 0);

    // B2
    GPIO_setPadConfig(4, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_4_GPIO4);
    GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);
    GPIO_writePin(4, 0);

    // B3
    GPIO_setPadConfig(7, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_7_GPIO7);
    GPIO_setDirectionMode(7, GPIO_DIR_MODE_OUT);
    GPIO_writePin(7, 0);

    // B4
    GPIO_setPadConfig(6, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_6_GPIO6);
    GPIO_setDirectionMode(6, GPIO_DIR_MODE_OUT);
    GPIO_writePin(6, 0);

    // C1
    GPIO_setPadConfig(3, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_3_GPIO3);
    GPIO_setDirectionMode(3, GPIO_DIR_MODE_OUT);
    GPIO_writePin(3, 0);

    // C2
    GPIO_setPadConfig(2, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_2_GPIO2);
    GPIO_setDirectionMode(2, GPIO_DIR_MODE_OUT);
    GPIO_writePin(2, 0);

    // C3
    GPIO_setPadConfig(1, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_1_GPIO1);
    GPIO_setDirectionMode(1, GPIO_DIR_MODE_OUT);
    GPIO_writePin(1, 0);

    // C4
    GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_0_GPIO0);
    GPIO_setDirectionMode(0, GPIO_DIR_MODE_OUT);
    GPIO_writePin(0, 0);
}


void SPWM_generate_3level(float32_t *sinWave, float32_t sawtooth_upper, float32_t sawtooth_lower)
{
    uint8_t i;
    uint8_t pwm_upper[3] = {0};
    uint8_t pwm_lower[3] = {0};

    // Generate Symmetrical PWM
    for(i=0; i<3; ++i)
    {
        // Compare with upper sawtooth
        if(sinWave[i] >= sawtooth_upper) pwm_upper[i] = 1;
        else if(sinWave[i] < sawtooth_upper) pwm_upper[i] = 0;

        // Compare with lower sawtooth
        if(sinWave[i] >= sawtooth_lower) pwm_lower[i] = 1;
        else if(sinWave[i] < sawtooth_lower) pwm_lower[i] = 0;

        switch(i)
        {
        // Generate PWM for 4 gates in Phase A
        case 0:
            GPIO_writePin(11, pwm_upper[i]);
            GPIO_writePin(10, !(pwm_upper[i]));
            GPIO_writePin(9,  pwm_lower[i]);
            GPIO_writePin(8,  !(pwm_lower[i]));
            break;

        // Generate PWM for 4 gates in Phase B
        case 1:
            GPIO_writePin(5, pwm_upper[i]);
            GPIO_writePin(4, !(pwm_upper[i]));
            GPIO_writePin(7, pwm_lower[i]);
            GPIO_writePin(6, !(pwm_lower[i]));
            break;

        // Generate PWM for 4 gates in Phase C
        case 2:
            GPIO_writePin(3, pwm_upper[i]);
            GPIO_writePin(2, !(pwm_upper[i]));
            GPIO_writePin(1, pwm_lower[i]);
            GPIO_writePin(0, !(pwm_lower[i]));
            break;
        }
    }

//    // Generate PWM for 4 gates in Phase A
//    if(symmetry_pwm[0] > 0)
//    {
//        GPIO_writePin(11, 1);
//        GPIO_writePin(10, 1);
//        GPIO_writePin(9,  0);
//        GPIO_writePin(8,  0);
//    }
//    else if(symmetry_pwm[0] == 0)
//    {
//        GPIO_writePin(11, 0);
//        GPIO_writePin(10, 1);
//        GPIO_writePin(9,  1);
//        GPIO_writePin(8,  0);
//    }
//    else if(symmetry_pwm[0] < 0)
//    {
//        GPIO_writePin(11, 0);
//        GPIO_writePin(10, 0);
//        GPIO_writePin(9,  1);
//        GPIO_writePin(8,  1);
//    }
//
//    // Generate PWM for 4 gates in Phase B
//    if(symmetry_pwm[0] > 0)
//    {
//        GPIO_writePin(5, 1);
//        GPIO_writePin(4, 1);
//        GPIO_writePin(7, 0);
//        GPIO_writePin(6, 0);
//    }
//    else if(symmetry_pwm[0] == 0)
//    {
//        GPIO_writePin(5, 0);
//        GPIO_writePin(4, 1);
//        GPIO_writePin(7, 1);
//        GPIO_writePin(6, 0);
//    }
//    else if(symmetry_pwm[0] < 0)
//    {
//        GPIO_writePin(5, 0);
//        GPIO_writePin(4, 0);
//        GPIO_writePin(7, 1);
//        GPIO_writePin(6, 1);
//    }
//
//    // Generate PWM for 4 gates in Phase C
//    if(symmetry_pwm[0] > 0)
//    {
//        GPIO_writePin(3, 1);
//        GPIO_writePin(2, 1);
//        GPIO_writePin(1, 0);
//        GPIO_writePin(0, 0);
//    }
//    else if(symmetry_pwm[0] == 0)
//    {
//        GPIO_writePin(3, 0);
//        GPIO_writePin(2, 1);
//        GPIO_writePin(1, 1);
//        GPIO_writePin(0, 0);
//    }
//    else if(symmetry_pwm[0] < 0)
//    {
//        GPIO_writePin(3, 0);
//        GPIO_writePin(2, 0);
//        GPIO_writePin(1, 1);
//        GPIO_writePin(0, 1);
//    }
}
