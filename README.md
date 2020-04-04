# In-Situ-Sampler

Different type of in-situ samplers are required to be designed/ adapted for an underwater laboratory of the type of BAAL (Bottom in-situ Acquisition Analysis Lab). One of the sampler is the precision water sampler that collects it in a very controlled manner and with a predefined volume and accuracy. This is akin to a volume medical syringe. This method could be adapted directly by suction techniques or motor driven or any suitable mechanism. The objectives are as follows: -
1. Develop a sampler that collect water only when commanded without any contamination.
2. Once sample is collected, it is retained without any loss.
3. The collected sample needs to be transferred into another chamber (for Analysis) through a tubular attachment or directly into the chamber.
4. Have the capability to wash it and dispose the water to the drain chamber and be ready for next sample.

#Project Components
ATMega328 (Micro Controller): -
The 8-bit microcontroller which consists of:
1. 32 KB flash memory with read-write capabilities
2. 1 KB ROM
3. 2 KB RAM
4. 23 general purpose I/O lines
5. 32 general purpose working registers
6. 3 flexible timer/counters with compare modes
7. Internal and external interrupts
8. Serial programmable USART
9. Byte-oriented 2-wire serial interface,
10. SPI (Serial Peripheral Bus Interface) serial port
11. 6-channel 10-bit A/D converter
12. Programmable watchdog timer with internal oscillator 13. Five software selectable power saving modes.

A3967 (Stepper Motor Driver): -
The A3967SLB is a complete micro stepping motor driver with built-in translator. It is designed to operate bipolar stepper motors in full, half, quarter and eighth-step modes, with output drive capability of 30 V and ±750 mA. The A3967 includes a fixed off-time current regulator that has the ability to operate in slow fast or mixed current-decay modes. This current-decay control scheme results in reduced audible motor noise, increased step accuracy and reduced power dissipation.

12 V DC Solenoid Valve: - A solenoid valve is an electro- mechanically operated valve. The valve is controlled by an electric current through a solenoid: in the case of a two-port valve the flow is switched on or off.

Medical Syringe: - A syringe is a simple pump consisting of a plunger that fits tightly in a tube. The plunger can be pulled and pushed along inside a cylindrical tube (called a barrel), allowing the syringe to take in and expel a liquid through an orifice at the open end of the tube. The open end of the syringe consists of a nozzle and a tubing arrangement (Fitted Additionally) to help direct the flow into and out of the barrel.

4-Wire Stepper Motor: - A stepper motor or step motor or stepping motor is a brushless DC electric motor that divides a full rotation into a number of equal steps. The motor's position can then be commanded to move and hold at one of these steps without any feedback sensor (an open-loop controller), as long as the motor is carefully sized to the application in respect to torque and speed.

12 V DC Battery: - A DC power supply is one that supplies a voltage of fixed polarity (either positive or negative) to its load. Depending on its design, a DC power supply may be powered from a DC source or from an AC source such as the power mains. 

#Procedure for sampling
Stage 1: - Flushing or cleaning the volume chamber.
1. First the position of the piston of the volume chamber is determined by the limit switches. If the position is not in the initial state, then the stepper motor rotates such that the piston of the volume chamber is set to the initial position.
2. The Distilled water comes in through the Distilled water chamber (Distilled water valve opens); the remaining 3 valves are closed.
3. This distilled water cleans the volume chamber and is transferred to the drain chamber (Via. Analysis Chamber). If the cleaning process is thorough and perfect; the conductivity sensed by the probe will be ‘1023’. If the value is below ‘1023’, that means the cleaning process is not thorough.

Stage 2: - Collection of known amount of water sample.
1. Depending upon the amount of sample required, the stepper motor rotates the number of steps required to take the sample water in the volume chamber through the piston action (LEAD - SCREW Arrangement).The configuration of stepper motor with the volume chamber is done such that for 4000 steps an approximate volume is 3cc is present in the chamber, which is 1333 steps for 1cc approximately of the sample taken.
2. The sample sea water comes in through the Sea water chamber (Sea water valve is opened); the remaining 3 valves are closed.
3. Thus, now the volume chamber has the programmed amount of water sample.

Stage 3: - Programmed amount of water sample to Analysis chamber.
1. Now the Analysis chamber valve is opened (remaining three valves are closed) and the programmed amount of water sample transfers to the analysis chamber through the same piston action of the volume chamber and further chemical tests are carried out at this chamber.

#Communication  
The controlling unit is communicated through the RFM12B module. It is a low costing ISM band transceiver module implemented with unique PLL. It works signal ranges from 433/868/915MHZ bands, comply with FCC, ETSI regulation. The SPI interface is used to communicate with microcontroller for parameter setting. As under-water communication cannot be done with RF; hence it can be done by wired communication. (Refer to RFM12B datasheet).

#Transistor as a switch
Darlington’s can be used to yield very high amplification of a control current (since a Darlington's total gain is
equal to the product of the gains of the two BJT transistors it is made from) These are generally used for high-current loads.
