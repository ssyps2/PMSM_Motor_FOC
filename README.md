## Diode Clamped Converter for Vehicle AC machine Drive

### Aim
The aim of this project is to simulate a 3-Phase Diode Clamped Inverter as a battery connected Permanent Magnet Synchronous Machine (PMSM) drive, and design a PCB to implement a low power version of this type of converter. The control algorithm will be tested in PLECS and eventually implemented in the F28379D micro-controller. Torque, speed, and mechanical position closed loop control of the PMSM motor will be included.

### Simulation
The main topology of the circuit for simulation in the PLECS is shown in Figure below.
![image](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/a4a4ac1a-0cc7-43fa-9447-bb58e6956107)

### PCB Design
The circuit designed in the simulation will be implemented in a 4-layer Printed Circuit Board (PCB). Surface Mount Technology (SMT) will be applied for more compact layout, higher design flexibility, and reducing the noise interference. The microcontroller is selected to be TI’s LAUNCHXL-F28379D, which is a 32-bit MCU with frequency up to 200MHz and sufficient communication interface (like UART, CAN, I2C) and high-speed PWM channels. Also, current sensor will be used to detect the 3-phase current and sampled by ADC to complete the torque control loop.

The schematic of PCB is shown in Figure below.
![image](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/4039ada5-c2c6-417e-8efa-4022319978dd)

The PCB layout is shown in Figrue below.
![Diode_Clamped_Inverter_PCB](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/71951df7-d960-4b83-af4a-00a8979c3b92)

### TimeTable
![image](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/ddc33e16-4198-4893-84d1-e65fb9a2295d)
