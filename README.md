# PSM
Pulse skip modulation (PSM) code for Arduino

This project explores ways to manipulate vibe pump flow using simple components. 

The basic setup consists of an Arduino Nano board and AC Dimmer board with an AC phase sensing circuit. 

Most popular AC Dimmer boards have a bridge rectifier that allows zero-crossing detection. 
This project requires modification of such board by replacing bridge rectifier with a diode, so the circuit detects full negative part of AC wave.

![Arduino_PSM_Schematic](https://github.com/banoz/banoz.github.io/blob/2faac361628262cd8dd6e8c73d1f20ef78227812/repository/img/Arduino_PSM_Schematic.png)

Each time a negative half-wave is detected, the code calculates whether the next positive pulse has to be skipped, and sets a TRIAC control output accordingly.

