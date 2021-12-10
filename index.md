# Gamepad Vibration Button Mapper

## Foreword

I came up with this project when I bought a new gamepad for PC games, only to realize many games do not have full gamepad support. Steam fortuneatly has a feature that allows you to remap keyboard and mouse controls onto gamepads, but their haptics feature only supports gamepads with linear actuators (ie steam controller and Nintendo Switch gamepad). But most standard gamepads, such as the ubiquitous Xbox gamepad, uses spinning motors with weights to produce rumble. To remedy this, I wrote this program in order to get vibration feedback on button presses. While not as ideal has having full controller support from the games, even having simple vibrations can make a game more enjoyable.

## Description

This program maps vibrations to gamepad button inputs. Vibrations can be continuous or pulsing, with control over frequency of pulses and number of pulses. This program only supports controllers using XINPUT.

## Design Notes

- This program "adjoins" overlapping vibrations together. For example, if the "A" button pulses the right motor, and the "B" button continuously vibrates the left motor, pressing both with simultaneously activate both motors.
  - If multiple buttons are attempting to activate the same motor, the faster/stronger vibration takes priority.
  - If one button is attempting to pulse and another is attempting to continuously vibrate the same motor, the continuous vibration takes priority.
- The program runs at a rate of 60 ticks-per-seconds. This slower rate is neccessary to properly detect positive-edge inputs, otherwise inputs will be dropped. 60 ticks-per-second is used because 60frames-per-second is standard in many video games, but other tick rates suck as 120tps or 30tps will work just as well.
- This program with built in VisualStudio2013 as a win32 console application. VisualStudio files have been omitted for convenience so the code can be easily adapted for use an other IDEs.

## Requirements

- Windows 10 (Windows 11 probably works, but has not been tested)
- Xinput9_1_0.lib library (included with most Windows installations)

## Future Improvements

- add support for multiple controllers
- ~~organize arrays better (tuples? vectors?)~~
- ~~add profiles support, saving and loading different profiles for different games~~
- create a UI rather than console application
- ~~give each button input individual pulse lengths~~
- ~~rename variables to be easier to understand~~
  - ~~name vibration pulses in terms of frequencey~~
- ~~reformat GamepadVib class to be more generic and more intuitive to use~~
- ~~"frequency" of pulses is actually represent 1/2 of a period, with the value being used for both the on and off halves of the period~~
  - ~~add more control over the length of the on and off halves of the pulse period~~

## Special Thanks

- [Katy's Code XINPUT Tutorial](https://katyscode.wordpress.com/2013/08/30/xinput-tutorial-part-1-adding-gamepad-support-to-your-windows-game/)
- [Lawrence McCauley Gamepad XINPUT Tutorial](https://lcmccauley.wordpress.com/2014/01/05/gamepad-input-tutorial/)
