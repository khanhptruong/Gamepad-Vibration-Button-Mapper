#include "GamepadVib.h"

GamepadVib::GamepadVib(int gpadIndex) : Gamepad(gpadIndex){
	_setDefault();
}

void GamepadVib::update(){
	Gamepad::update();
	_getButtonsPosEdge();
	_getButtonsPressed();
}

bool GamepadVib::isExit(){ return _isMatching(_exitButtonMap, _buttonsPressed, _numTotalButtons) == true; }

std::string GamepadVib::getProfileName()     { return _profileName; }
bool  GamepadVib::getBtnPressed(int index)   { return _buttonsPressed[index]; }
bool  GamepadVib::getBtnPosEdge(int index)   { return _buttonsPosEdge[index]; }
bool  GamepadVib::getBtnMap(int index)       { return _btnMap[index]; }
int   GamepadVib::getNumPulses(int index)    { return _numPulses[index]; }
float GamepadVib::getPulseFreq(int index)    { return _pulseFreq[index]; }
float GamepadVib::getPulseOffFreq(int index) { return _pulseOffFreq[index]; }
float GamepadVib::getLeftSpd(int index)      { return _leftSpeed[index]; }
float GamepadVib::getRightSpd(int index)     { return _rightSpeed[index]; }

void GamepadVib::setProfileName(std::string name){ _profileName = name; }
void GamepadVib::setBtnMap(bool btn, int index)             { _btnMap[index] = btn; }
void GamepadVib::setNumPulses(int num, int index)           { _numPulses[index] = num; }
void GamepadVib::setPulseFreq(float freq, int index)        { _pulseFreq[index] = freq; }
void GamepadVib::setPulseOffFreq(float offFreq, int index)  { _pulseOffFreq[index] = offFreq; }
void GamepadVib::setLeftSpd(float spd, int index)           { _leftSpeed[index] = spd; }
void GamepadVib::setRightSpd(float spd, int index)          { _rightSpeed[index] = spd; }

void GamepadVib::_getButtonsPressed(){
	for (int i = 0; i < _numDigitalButtons; i++){
		_buttonsPressed[i] = Gamepad::isPressed(xinputButtons[i]);
	}
	if (Gamepad::getLeftTrigger() > 0){
		_buttonsPressed[_numTotalButtons - 2] = true;
	}
	else{
		_buttonsPressed[_numTotalButtons - 2] = false;
	}
	if (Gamepad::getRightTrigger() > 0){
		_buttonsPressed[_numTotalButtons - 1] = true;
	}
	else{
		_buttonsPressed[_numTotalButtons - 1] = false;
	}
}

void GamepadVib::_getButtonsPosEdge(){
	for (int i = 0; i < _numDigitalButtons; i++){
		_buttonsPosEdge[i] = Gamepad::isPosEdge(xinputButtons[i]);
	}
	if (Gamepad::isLeftTriggerPosEdge() == true){
		_buttonsPosEdge[_numTotalButtons - 2] = true;
	}else{
		_buttonsPosEdge[_numTotalButtons - 2] = false;
	}
	if (Gamepad::isRightTriggerPosEdge() == true){
		_buttonsPosEdge[_numTotalButtons - 1] = true;
	}
	else{
		_buttonsPosEdge[_numTotalButtons - 1] = false;
	}
}

bool GamepadVib::_isMatching(bool array1[], bool array2[], int arraySize){
	int i = 0;
	while ((array1[i] == array2[i]) && i < arraySize){
		i++;
	}
	if (i == arraySize){
		return true;
	}
	else{
		return false;
	}
}

void GamepadVib::_setDefault(){
	_profileName = "Default";

	_btnMap[0] = false; //A
	_btnMap[1] = false; //B
	_btnMap[2] = false; //X
	_btnMap[3] = false; //Y
	_btnMap[4] = false; //DPAD UP
	_btnMap[5] = false; //DPAD DOWN
	_btnMap[6] = false; //DPAD LEFT
	_btnMap[7] = false; //DPAD RIGHT
	_btnMap[8] = false; //L SHOULDER
	_btnMap[9] = false; //R SHOULDER
	_btnMap[10] = false; //L THUMB
	_btnMap[11] = false; //R THUMB
	_btnMap[12] = false; //START
	_btnMap[13] = false; //BACK
	_btnMap[14] = false; //L TRIGGER
	_btnMap[15] = false;  //R TRIGGER

	//if 1000 or more, just do it forever lol
	_numPulses[0] = 0; //A
	_numPulses[1] = 0; //B
	_numPulses[2] = 0; //X
	_numPulses[3] = 0; //Y
	_numPulses[4] = 0; //DPAD UP
	_numPulses[5] = 0; //DPAD DOWN
	_numPulses[6] = 0; //DPAD LEFT
	_numPulses[7] = 0; //DPAD RIGHT
	_numPulses[8] = 0; //L SHOULDER
	_numPulses[9] = 0; //R SHOULDER
	_numPulses[10] = 0; //L THUMB
	_numPulses[11] = 0; //R THUMB
	_numPulses[12] = 0; //START
	_numPulses[13] = 0; //BACK
	_numPulses[14] = 0; //L TRIGGER
	_numPulses[15] = 0;  //R TRIGGER

	//0 = continuous
	_pulseFreq[0] = 0.0f; //A
	_pulseFreq[1] = 0.0f; //B
	_pulseFreq[2] = 0.0f; //X
	_pulseFreq[3] = 0.0f; //Y
	_pulseFreq[4] = 0.0f; //DPAD UP
	_pulseFreq[5] = 0.0f; //DPAD DOWN
	_pulseFreq[6] = 0.0f; //DPAD LEFT
	_pulseFreq[7] = 0.0f; //DPAD RIGHT
	_pulseFreq[8] = 0.0f; //L SHOULDER
	_pulseFreq[9] = 0.0f; //R SHOULDER
	_pulseFreq[10] = 0.0f; //L THUMB
	_pulseFreq[11] = 0.0f; //R THUMB
	_pulseFreq[12] = 0.0f; //START
	_pulseFreq[13] = 0.0f; //BACK
	_pulseFreq[14] = 0.0f; //L TRIGGER
	_pulseFreq[15] = 0.0f;  //R TRIGGER

	_pulseOffFreq[0] = 0.0f; //A
	_pulseOffFreq[1] = 0.0f; //B
	_pulseOffFreq[2] = 0.0f; //X
	_pulseOffFreq[3] = 0.0f; //Y
	_pulseOffFreq[4] = 0.0f; //DPAD UP
	_pulseOffFreq[5] = 0.0f; //DPAD DOWN
	_pulseOffFreq[6] = 0.0f; //DPAD LEFT
	_pulseOffFreq[7] = 0.0f; //DPAD RIGHT
	_pulseOffFreq[8] = 0.0f; //L SHOULDER
	_pulseOffFreq[9] = 0.0f; //R SHOULDER
	_pulseOffFreq[10] = 0.0f; //L THUMB
	_pulseOffFreq[11] = 0.0f; //R THUMB
	_pulseOffFreq[12] = 0.0f; //START
	_pulseOffFreq[13] = 0.0f; //BACK
	_pulseOffFreq[14] = 0.0f; //L TRIGGER
	_pulseOffFreq[15] = 0.0f;  //R TRIGGER

	_leftSpeed[0] = 0.0f; //A
	_leftSpeed[1] = 0.0f; //B
	_leftSpeed[2] = 0.0f; //X
	_leftSpeed[3] = 0.0f; //Y
	_leftSpeed[4] = 0.0f; //DPAD UP
	_leftSpeed[5] = 0.0f; //DPAD DOWN
	_leftSpeed[6] = 0.0f; //DPAD LEFT
	_leftSpeed[7] = 0.0f; //DPAD RIGHT
	_leftSpeed[8] = 0.0f; //L SHOULDER
	_leftSpeed[9] = 0.0f; //R SHOULDER
	_leftSpeed[10] = 0.0f; //L THUMB
	_leftSpeed[11] = 0.0f; //R THUMB
	_leftSpeed[12] = 0.0f; //START
	_leftSpeed[13] = 0.0f; //BACK
	_leftSpeed[14] = 0.0f; //L TRIGGER
	_leftSpeed[15] = 0.0f;  //R TRIGGER

	_rightSpeed[0] = 0.0f; //A
	_rightSpeed[1] = 0.0f; //B
	_rightSpeed[2] = 0.0f; //X
	_rightSpeed[3] = 0.0f; //Y
	_rightSpeed[4] = 0.0f; //DPAD UP
	_rightSpeed[5] = 0.0f; //DPAD DOWN
	_rightSpeed[6] = 0.0f; //DPAD LEFT
	_rightSpeed[7] = 0.0f; //DPAD RIGHT
	_rightSpeed[8] = 0.0f; //L SHOULDER
	_rightSpeed[9] = 0.0f; //R SHOULDER
	_rightSpeed[10] = 0.0f; //L THUMB
	_rightSpeed[11] = 0.0f; //R THUMB
	_rightSpeed[12] = 0.0f; //START
	_rightSpeed[13] = 0.0f; //BACK
	_rightSpeed[14] = 0.0f; //L TRIGGER
	_rightSpeed[15] = 0.0f;  //R TRIGGER

	_exitButtonMap[0] = false; //A
	_exitButtonMap[1] = false; //B
	_exitButtonMap[2] = false; //X
	_exitButtonMap[3] = false; //Y
	_exitButtonMap[4] = false; //DPAD UP
	_exitButtonMap[5] = false; //DPAD DOWN
	_exitButtonMap[6] = false; //DPAD LEFT
	_exitButtonMap[7] = false; //DPAD RIGHT
	_exitButtonMap[8] = false; //L SHOULDER
	_exitButtonMap[9] = false; //R SHOULDER
	_exitButtonMap[10] = false; //L THUMB
	_exitButtonMap[11] = false; //R THUMB
	_exitButtonMap[12] = true; //START
	_exitButtonMap[13] = true; //BACK
	_exitButtonMap[14] = false; //L TRIGGER
	_exitButtonMap[15] = false;  //R TRIGGER
}
