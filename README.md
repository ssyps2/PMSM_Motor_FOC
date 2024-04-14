## Diode Clamped Converter for Vehicle AC machine Drive

### Aim
The aim of this project is to simulate a 3-Phase Diode Clamped Inverter as a battery connected Permanent Magnet Synchronous Machine (PMSM) drive, and design a PCB to implement a low power version of this type of converter. The control algorithm will be tested in PLECS and eventually implemented in the F28379D micro-controller. Torque, speed, and mechanical position closed loop control of the PMSM motor will be included.

### Simulation
The main topology of the circuit for simulation in the PLECS is shown in Figure below.
![image](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/a4a4ac1a-0cc7-43fa-9447-bb58e6956107)

### PCB Design
The circuit designed in the simulation will be implemented in a 4-layer Printed Circuit Board (PCB). Surface Mount Technology (SMT) will be applied for more compact layout, higher design flexibility, and reducing the noise interference. The microcontroller is selected to be TI’s LAUNCHXL-F28379D, which is a 32-bit MCU with frequency up to 200MHz and sufficient communication interface (like UART, CAN, I2C) and high-speed PWM channels. Also, current sensor will be used to detect the 3-phase current and sampled by ADC to complete the torque control loop.

The schematic of PCB is shown in Figure below.
![image](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/bc733961-c39d-4b9a-a299-4fcb73997e5e)

The PCB layout is shown in Figrue below.
![Diode_Clamped_Inverter_PCB](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/9c493f7f-d6ee-4b29-b8ef-86e003d67d15)

3D Model:
![3D Model_1](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/a65b89d8-199c-4125-88df-353d036784a7)
![3D Model_2](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/4a0fd71a-1d68-4f6e-a3c0-ef6d30fd30ff)

Diode Clamped Inverter:
![Diode Clamped Inverter](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/26e4de7f-756b-4ba4-b1e8-3f6025c14516)


### TimeTable
![image](https://github.com/ssyps2/PMSM_Motor_FOC/assets/72872247/961163a5-0949-4bd6-bfe1-eb36d0dc882a)

### Proposal
[Final Year Project Proposal - Pengyuan SHU.pdf](https://github.com/ssyps2/PMSM_Motor_FOC/files/14595623/Final.Year.Project.Proposal.-.Pengyuan.SHU.pdf)

