#ifndef GAMEPADVIB_H
#define GAMEPADVIB_H

#include "Gamepad.h"
#include <string>
extern const WORD xinputButtons[];

class GamepadVib : public Gamepad
{
public:
	GamepadVib(int gpadIndex);
	static const short int numDigitalButtons = 14; //number of digital buttons
	static const short int numTotalButtons = 16; //digital buttons and two triggers
	
	void update();
	bool isExit();

	std::string profileName;
	bool  buttonsPressed[numTotalButtons];
	bool  buttonsPosEdge[numTotalButtons];
	bool  buttonMap[numTotalButtons];
	short int numPulses[numTotalButtons];
	float pulseFreq[numTotalButtons]; 
	float pulseOffFreq[numTotalButtons];
	float leftSpeed[numTotalButtons];
	float rightSpeed[numTotalButtons];

private:
	bool _exitButtonMap[numTotalButtons];

	void _setButtonsPressed();
	void _setButtonsPosEdge();
	bool _isMatching(bool array1[], bool array2[], int arraySize);
	void _setDefault();
};

#endif // GAMEPADVIB_H