# Reaction Timer
  In this project, we created a reaction timer that measures the reaction time of a person using a microcontroller. This reaction timer is a device used to measure a user’s reaction time when presented with a visual stimulus. The entire program is controlled by a button, in which the button is used to both start the timer and also to mark the user’s reaction. The visual stimulus is a LED, while the feedback is received via the LED, a buzzer, and the LCD where the time is displayed.
 The purpose of this creation is to improve people’s reaction speeds and how quickly they process information.

![20221207_174623](https://github.com/mothmanu/Reaction-Timer/assets/56203045/761ee8b9-0dc9-4577-8725-6d5494b3cde4)


## Contents
- [Instructions](#instructions)
- [Features](#features)

## Instructions:
- Plug the wall mount into a wall plug rated from 100-240V AC.
- The LCD will power on, and the menu will boot up saying “Press To Start”, followed by the fastest reaction time recorded during that session.
- Upon pushing the button to start, the LCD will display the text “Get Ready”.
- The system will then wait for a random duration of time, between 2-7 seconds, before the LED will briefly flash for 200 milliseconds.
- 
    * If the user presses the button before the LED flashes, the text “Wait for the LED” will appear on the LCD. The LED           will flash red and the buzzer will make a low-pitched buzz.
    * If the user presses the button within 300ms of the flash, the LED will flash green and the buzzer will play a happy         “victory” jingle. The LCD will display the reaction time, followed by the best time during that session.
    * If the user presses the button after 300ms of the flash, the LED will flash red and the buzzer will play a “losing”         jingle. The LCD will display the reaction time, followed by the fastest time recorded during that session.
    * If the LED flashes and the user waits 7 seconds without pressing the button to react, the LED will flash red and the         buzzer will play the “losing” jingle. The LCD will display the text “You waited too long”.
- The system will shortly return back to the “Press To Start” menu.
- When the device is ready to be powered off, unplug the device from the wall to power it off.


## Features:
*  **IC: ATmega328p**
*  * **Software**
   * MPLAB X IDE
*  * **Connectivity**
     * SPI Interface to connect to any Arduino compatible MCU
* **Specifications**
    * Operating Voltage: 1.8V - 5.4V
    * Temperature Range: -40°C - 80°C
    * Input Voltage: 100V - 240V
    * Input Current: 500mA
    * Power: 6W
    * Button's Input Force: 1.08N - 2.06N
    * Button's Contact Bounce: 5ms
    * Voltage on any Pin except RESET: -0.5V - 5.5V
    * Voltage on RESET pin: -0.5V - 13V
    * DC Current per I/O pin: 0A - 40mA
    * DC Current on Vcc and Ground Pins: 0A - 200mA
* **Other Features**
    *LCD display to show time data
    *Buzzer to indicate whether user pressed button in time

## Note!!:
  The reaction time displayed on the LCD has very little uncertainty. The 14.7456 MHz crystal allows for very accurate timing measurements down to ±34ns, however, the LCD only displays values to three decimal places. Rounding occurs in the program, which leads to an accuracy of ±500μs.



