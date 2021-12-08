Khanh Truong
github id: khanhptruong

Gamepad Vibration Button Mapper
2021 Dec 7

For more detailed info, visit github page: [COMING SOONG]

-----------
Description
-----------
This program maps controller vibrations to gamepad buttons. Vibrations can pulse once when a button is pressed, or vibrate continuously when button is held.

--------------
Included Files
--------------
Gamepad.h               -class for interfacing XINPUT gamepads
Gamepad.cpp
TickCounter.h   	-class for controlling tick rates and timing things
TickCounter.cpp
main.cpp		-main code
README.txt		-this file

-----------------
Required Software
-----------------
Windows 10 (Windows 11 should work but has not been tested)
Xinput9_1_0.lib library (included with most Windows installations)

-----
Notes
-----
-This program with built in VisualStudio2013 as a win32 console application. VisualStudio files have been omitted for convenience so the code can be easily adapted for use an other IDEs.
