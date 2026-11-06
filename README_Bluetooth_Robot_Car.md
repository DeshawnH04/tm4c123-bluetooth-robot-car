# Bluetooth-Controlled Robot Car

**CECS 447 — Project 3 | Embedded C | TM4C123GH6PM (ARM Cortex-M4)**

A Bluetooth-controlled, two-motor robot car programmed in **embedded C** for the **Texas Instruments TM4C123 LaunchPad**. The firmware receives character commands through **UART1** from a Bluetooth serial terminal, drives the motors with **hardware PWM**, and supports both free driving and preprogrammed movement patterns.

## Features

- **Wireless control:** Bluetooth serial commands received through a UART-connected module.
- **Two operating modes:** Switch between demo patterns and free driving with the onboard **SW1 (PF4)** button.
- **Preprogrammed demonstrations:** Figure-eight, circle, square, and four-segment zigzag routes.
- **Directional control:** Forward, reverse, left, right, and stop.
- **Adjustable motor speed:** Four PWM duty settings (nominally 35%, 60%, 80%, and 98%).
- **Mode feedback:** Onboard RGB LED shows **green for Demo** and **blue for Free Driving**.
- **Clock and timing:** 50 MHz system clock configured using the PLL; SysTick delays for movement timing.

## System overview

```text
Bluetooth serial terminal (phone / computer)
                    |
         Bluetooth serial module
                    | UART1 (PB0 / PB1)
                    v
       TM4C123 LaunchPad (50 MHz)
              |            |
       PWM PB6 / PB7    GPIO PE0–PE3
              |            |
              +------ Motor control interface
                             |
                       Two DC motors
```

> **Hardware note:** The CECS 447 assignment specifies an **HC-05** Bluetooth module and an **L298N** motor driver. The supplied firmware's UART driver comments instead mention **HC-06**, and its motor-control code is organized around **DIR/SLP** signals (a Romi-style motor interface). Check the actual Bluetooth module, motor driver, voltage ratings, and pin wiring before applying power. The diagram above describes the firmware's control paths, not a verified L298N wiring schematic.

## Hardware

- TM4C123GH6PM Tiva C Series LaunchPad
- Two-DC-motor robot chassis (Romi-style control code)
- Compatible motor driver / motor-control interface
- Bluetooth serial module (HC-05 per assignment; confirm compatibility with the configured UART settings)
- Power supply for the robot and controller, suitable for the selected hardware
- Bluetooth serial terminal app on a phone or computer

### Firmware pin assignments

| TM4C123 pin | Purpose |
| --- | --- |
| **PB0** | UART1 RX — connect to Bluetooth module TX |
| **PB1** | UART1 TX — connect to Bluetooth module RX |
| **PB6 / M0PWM0** | Right motor PWM |
| **PB7 / M0PWM1** | Left motor PWM |
| **PE0** | Right motor DIR |
| **PE1** | Right motor SLP |
| **PE2** | Left motor DIR |
| **PE3** | Left motor SLP |
| **PF4 / SW1** | Toggle Demo / Free Driving mode |
| **PF1–PF3** | Onboard RGB mode indicator |

The Bluetooth module and LaunchPad must share a **common ground**. Follow the module's specifications for power and UART logic levels, and use a proper motor driver rather than connecting motors directly to the microcontroller pins.

## Operating modes

On startup, the firmware stops the car, selects **Demo mode**, and turns the onboard LED **green**. Press **SW1** to switch modes; the LED turns **blue** in Free Driving mode. Switching modes also stops the car.

### Mode 1 — Demo (green LED)

| Bluetooth character | Action |
| --- | --- |
| `8` | Drive a figure-eight pattern, then stop |
| `C` | Drive one circle, then stop |
| `S` | Drive a square, then stop |
| `Z` | Drive a four-segment zigzag, then stop |

The code also accepts lowercase `c`, `s`, and `z`. Pattern shape and distance depend on motor behavior, surface friction, battery level, and the **tunable timing and speed constants in `Demo.c`**. SW1 can interrupt a running demo pattern.

### Mode 2 — Free Driving (blue LED)

| Bluetooth character | Action |
| --- | --- |
| `F` | Move forward |
| `B` | Move backward |
| `L` | Make a wide left turn |
| `R` | Make a wide right turn |
| `S` | Stop |
| `U` | Increase speed one step |
| `D` | Decrease speed one step |

Lowercase letters are also accepted in Free Driving mode. Free-driving speed levels are defined in `RomiCar.h` as `3500`, `6000`, `8000`, and `9800` out of a PWM period of `10000`.

## UART configuration

The runtime firmware in `UART1.c` configures the Bluetooth link as follows:

| Setting | Value |
| --- | --- |
| Peripheral | UART1 |
| System clock | 50 MHz |
| Baud rate | **57,600** |
| Data bits | 8 |
| Parity | **Odd** |
| Stop bits | 1 |
| GPIO | PB0 (RX), PB1 (TX) |

Configure your Bluetooth module's **data mode** to match these settings. The original assignment includes an HC-05 **AT-command setup** exercise, but the provided source files contain only the **robot-driving/data-mode firmware**; a separate AT-configuration program is not included in this repository.

## Build and run

1. Create a TM4C123GH6PM project in **Keil µVision / MDK-ARM** or another compatible ARM embedded development environment.
2. Add these seven source files to the project: `RomiCarTest.c`, `RomiCar.c`, `Mode.c`, `Demo.c`, `PWM.c`, `UART1.c`, and `SysTick.c`. Add the accompanying `.h` files to the project's include path. Use the device's appropriate startup file and build configuration.
3. Configure the Bluetooth module for **57,600 baud, 8 data bits, odd parity, 1 stop bit** in its **data mode**. Pair it with the Bluetooth terminal on your phone/computer.
4. Wire the UART pins and motor-control signals as described above, **after confirming they match your motor driver**. Use an appropriate power supply and shared ground.
5. Build and flash the firmware to the LaunchPad. The application entry point is `main()` in `RomiCarTest.c`.
6. Open the Bluetooth terminal, send one-character commands, and use onboard SW1 to change between modes.
7. If the car's forward/reverse orientation is wrong, adjust `ROMI_MOTOR_INVERTED` in `RomiCar.h`. Calibrate movement times and duty values for your actual robot and test surface.

**Important:** The repository contains source and header files, **not** a ready-to-open Keil `.uvprojx` project or an HC-05 AT-setup program. It has not been independently compiled or physically tested as part of this GitHub packaging.

## Code organization

| File | Responsibility |
| --- | --- |
| `RomiCarTest.c` | `main()`, initialization, and Bluetooth command dispatch |
| `RomiCar.c` / `RomiCar.h` | Motor direction, driving, turn logic, and speed levels |
| `Demo.c` / `Demo.h` | Timed figure-eight, circle, square, and zigzag routines |
| `Mode.c` / `Mode.h` | SW1 mode toggling, debounce, RGB status LED |
| `PWM.c` / `PWM.h` | PLL clock setup, PWM generation, motor GPIO configuration; includes some reference/example routines |
| `UART1.c` / `UART1.h` | UART1 data-mode initialization and serial input |
| `SysTick.c` / `SysTick.h` | Millisecond delays for demonstration patterns |
| `tm4c123gh6pm.h` | Texas Instruments MCU register definitions |

## Skills demonstrated

`Embedded C` · `ARM Cortex-M4` · `TM4C123` · `UART` · `Bluetooth serial communication` · `Hardware PWM` · `GPIO` · `DC motor control` · `SysTick` · `Modular firmware`

## Acknowledgments

Developed as a **CECS 447** coursework project. Some supporting driver/reference files contain original attribution and redistribution notices for **Jonathan W. Valvano**, **Dr. Min He**, and **Texas Instruments**. Those notices have been preserved in the source files. The original assignment document is not redistributed here.
