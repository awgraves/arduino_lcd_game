# Arduino LCD Game
A basic platformer style game on a 16x2 character LCD screen powered by an Arduino Uno.

Hardware drivers for the joystick and LCD display were written from scratch for this project - no 3rd party libraries used!

## Hardware list
1. [Arduino Uno R3](https://docs.arduino.cc/hardware/uno-rev3/)
2. [KY-023 Breakout Module 2-Axis joystick](https://naylampmechatronics.com/img/cms/Datasheets/000036%20-%20datasheet%20KY-023-Joy-IT.pdf)
3. C1602A 16x2 LCD Display (powered by a [HD44780](https://www.alldatasheet.com/datasheet-pdf/view/63673/HITACHI/HD44780.html) under the hood)
4. 10k resistor
5. 10k potentiometer

## Schematics
![schematics](/schematics/schematics.png)

Schematic drawing was created using [kicad](https://www.kicad.org/).
See the `/schematics/kicad` dir in this project.

## Software
### Prerequisites:
1. [PlatformIO Core CLI](https://docs.platformio.org/en/latest/core/index.html)
2. [GNU Make](https://www.gnu.org/software/make/)

### Usage
| Command | Description |
|---------|---------------|
|`make setup`| generates the `compile_commands.json` for clangd so IDE know proj dependency paths. |
|`make upload`| compiles and uploads code to the arduino uno over USB connection. |
|`make monitor`| opens a serial monitor connection to the arduino. Useful for debugging during development.|
