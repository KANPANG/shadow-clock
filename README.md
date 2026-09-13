**ShadowPlay Clock**
An Arduino-based interactive clock that uses **light, shadows, and sensor detection** to display the current time.

## Overview
- The ShadowPlay Clock combines the concept of a traditional sundial with a modern analog clock. Instead of using physical clock hands or a digital display, the device uses **coloured LEDs and the user's finger as a shadow-casting object** to represent the hour, minute, and second.
- The system activates when a user's finger is detected approximately **7 cm** from the clock face. An Arduino Uno processes the sensor input, calculates the corresponding clock positions, and controls **36 LEDs** to create the appropriate shadows.
- This project was independently designed and built as a Computer Engineering project.

## How It Works
The clock uses three coloured light sources to represent the three hands:

* 🔵 **Blue light** — hour hand
* 🔴 **Red light** — minute hand
* 🟢 **Green light** — second hand

When the user places a finger near the center of the clock:

1. An ultrasonic sensor measures the distance to the finger.
2. The Arduino determines whether the finger is within the activation range.
3. The current time is calculated by the software clock.
4. The hour, minute, and second are converted into positions on the 12-position clock face.
5. The corresponding LEDs are activated.
6. The finger casts shadows that visually indicate the time.

When no object is detected within the activation range, the LEDs remain off.

## Technical Implementation

### Ultrasonic Sensor
- The clock uses an ultrasonic sensor to detect user interaction.
- The system targets a distance of approximately **7 cm**, with a tolerance of ±1 cm.
- To improve measurement stability, the program takes three sensor readings and uses the **median value** rather than relying on a single measurement.
**long readDistanceMed3()**
This helps reduce the effect of inconsistent sensor readings and accidental activations.

### LED Control
The project uses **36 LEDs** arranged into three groups of 12:

* 12 LEDs for hours
* 12 LEDs for minutes
* 12 LEDs for seconds

Five **74HC595 shift registers** are used to control the LEDs while reducing the number of Arduino output pins required.

The program maintains a 40-bit LED buffer:

**byte leds[5] = {0, 0, 0, 0, 0};**

Although five registers provide 40 available output bits, only 36 are required by the clock.

### Time Position Calculations

The clock face contains 12 positions.

For the hour hand:
Hour Position = (Hour mod 12 + 6) mod 12

For the minute hand:
Minute Position = ((Minute / 5) + 6) mod 12

For the second hand:
Second Position = (Second / 5) mod 12

The additional offset accounts for the physical arrangement of the LEDs and the direction of the resulting shadows.
Because the clock face contains 12 positions, the minute and second hands are represented in 5-minute and 5-second increments respectively.

## Hardware
* Arduino Uno × 1
* Ultrasonic sensor × 1
* Blue LEDs × 12
* Red LEDs × 12
* Green LEDs × 12
* 74HC595 shift registers × 5
* 220 Ω resistors × 36
* Breadboards × 2
* External power supply × 1
* Male-to-male jumper wires
* Female-to-male jumper wires
* USB 2.0 cable

## Software
* Arduino IDE
* Arduino / C++

### Key Programming Concepts
* Digital input and output
* Ultrasonic distance measurement
* Sensor data processing
* Median filtering
* Bit manipulation
* Shift-register communication
* LED state management
* Modular arithmetic
* Time tracking using `millis()`
* Serial debugging

## Timekeeping
The current version uses a **software-based clock** rather than a dedicated real-time clock (RTC) module.

The clock tracks elapsed time using Arduino's `millis()` function and updates the hour, minute, and second values accordingly.

The initial time can be configured directly in the source code.

## Challenges & Problem Solving

### Sensor Stability
Individual ultrasonic readings could vary slightly, which could cause unreliable activation.

To improve stability, three readings are collected and the median value is used to determine the detected distance.

### LED Control
Controlling 36 LEDs required more output connections than could be conveniently handled directly by the Arduino.

Five 74HC595 shift registers were used to control the LED array through serial data transmission.

### Physical Wiring
The prototype required more than 70 wire connections across two breadboards, the Arduino, and the LED assembly. This made testing and troubleshooting more challenging.

A future version could improve the hardware design by using:

* A custom PCB
* Shorter and colour-coded wires
* A more compact circuit layout

## Future Improvements
Potential improvements include:

* Adding a dedicated **RTC module** for automatic and more accurate timekeeping
* Designing a custom PCB
* Improving wire management and physical packaging
* Increasing the precision of the minute and second displays
* Improving the user interaction mechanism
* Making the device more compact and portable

## Project Result
The ShadowPlay Clock successfully combined concepts from:

* Programming
* Electronics
* Physics
* Sensor technology
* Colour theory
* Mathematical modelling

The project achieved the original goal of creating an interactive clock that displays time through coloured shadows.

## Author
**Chloe Pang**
Computer Engineering Project
2025–2026
