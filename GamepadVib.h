#ifndef GAMEPADVIB_H
#define GAMEPADVIB_H

#include "Gamepad.h"
#include <string>
extern const WORD xinputButtons[];

class GamepadVib : public Gamepad
{
public:
	GamepadVib(int gpadIndex);
	
	void update();
	bool isExit();

	std::string getProfileName();
	bool  getBtnPressed(int index);
	bool  getBtnPosEdge(int index);
	bool  getBtnMap(int index);
	int   getNumPulses(int index);
	float getPulseFreq(int index);
	float getPulseOffFreq(int index);
	float getLeftSpd(int index);
	float getRightSpd(int index);

	void setProfileName(std::string name);
	void setBtnMap(bool btn, int index);
	void setNumPulses(int num, int index);
	void setPulseFreq(float freq, int index);
	void setPulseOffFreq(float offFreq, int index);
	void setLeftSpd(float spd, int index);
	void setRightSpd(float spd, int index);

private:
	static const int _numDigitalButtons = 14; //number of digital buttons
	static const int _numTotalButtons = 16; //digital buttons and two triggers

	std::string _profileName;
	bool  _buttonsPressed[_numTotalButtons];
	bool  _buttonsPosEdge[_numTotalButtons];
	bool  _btnMap[_numTotalButtons];
	int   _numPulses[_numTotalButtons]; 
	float _pulseFreq[_numTotalButtons]; //0 means pulse continuously
	float _pulseOffFreq[_numTotalButtons];
	float _leftSpeed[_numTotalButtons];
	float _rightSpeed[_numTotalButtons];
	bool  _exitButtonMap[_numTotalButtons];

	void _getButtonsPressed();
	void _getButtonsPosEdge();
	bool _isMatching(bool array1[], bool array2[], int arraySize);
	void _setDefault();
};

#endif // GAMEPADVIB_H