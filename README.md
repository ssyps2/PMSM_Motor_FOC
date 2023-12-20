## Diode Clamped Converter for Vehicle AC machine Drive

#### Aim
The aim of this project is to simulate a 3-Phase Diode Clamped Inverter as a battery connected Permanent Magnet Synchronous Machine (PMSM) drive, and design a PCB to implement a low power version of this type of converter. The control algorithm will be tested in PLECS and eventually implemented in the F28379D micro-controller. Torque, speed, and mechanical position closed loop control of the PMSM motor will be included.

#### Objectives
A.	Simulation
The main topology of the circuit for simulation in the PLECS is shown in Figure below.


B.	PCB Design
The circuit designed in the simulation will be implemented in a 2-layer Printed Circuit Board (PCB), which is a medium used to integrate all the electrical components. Surface Mount Technology (SMT) will be applied for more compact layout, higher design flexibility, and reducing the noise interference. The microcontroller is selected to be TI’s LAUNCHXL-F28379D, which is a 32-bit MCU with frequency up to 200MHz and sufficient communication interface (like UART, CAN, I2C) and high-speed PWM channels. Also, current sensor will be used to detect the 3-phase current and sampled by ADC to complete the torque control loop.

The PCB layout is shown in Figrue below.
