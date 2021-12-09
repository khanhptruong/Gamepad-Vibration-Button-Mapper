#ifndef GAMEPADVIB_H
#define GAMEPADVIB_H

#include "Gamepad.h"
extern const WORD xinputButtons[];

class GamepadVib : public Gamepad
{
public:
	GamepadVib(int gpadIndex);
	
	void update();
	bool isExit();

	bool  getBtnPressed(int index);
	bool  getBtnPosEdge(int index);
	bool  getBtnMap(int index);
	int   getNumPulses(int index);
	float getPulseFreq(int index);
	float getPulseOffFreq(int index);
	float getLeftSpd(int index);
	float getRightSpd(int index);

private:
	static const int _numDigitalButtons = 14; //number of digital buttons
	static const int _numTotalButtons = 16; //digital buttons and two triggers

	bool  _buttonsPressed[16];
	bool  _buttonsPosEdge[16];
	bool  _exitButtonMap[_numTotalButtons];
	bool  _btnMap[_numTotalButtons];
	int   _numPulses[_numTotalButtons]; 
	float _pulseFreq[_numTotalButtons]; //0 means pulse continuously
	float _pulseOffFreq[_numTotalButtons];
	float _leftSpeed[_numTotalButtons];
	float _rightSpeed[_numTotalButtons];

	void _setDefault();
	void _getButtonsPressed();
	void _getButtonsPosEdge();
	bool _isMatching(bool array1[], bool array2[], int arraySize);
};

#endif // GAMEPADVIB_H