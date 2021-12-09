#include "GamepadVib.h"

GamepadVib::GamepadVib(int gpadIndex) : Gamepad(gpadIndex){
	_setDefault();
}

void GamepadVib::update(){
	Gamepad::update();
	_getButtonsPosEdge();
	_getButtonsPressed();
}

bool GamepadVib::isExit(){
	return _isMatching(_exitButtonMap, _buttonsPressed, _numTotalButtons) == true;
}

bool  GamepadVib::getBtnPressed(int index){ return _buttonsPressed[index]; }
bool  GamepadVib::getBtnPosEdge(int index){ return _buttonsPosEdge[index]; }
bool  GamepadVib::getBtnMap(int index)    { return _btnMap[index]; }
int   GamepadVib::getNumPulses(int index) { return _numPulses[index]; }
int   GamepadVib::getPulseFreq(int index) { return _pulseFreq[index]; }
float GamepadVib::getLeftSpd(int index)   { return _leftSpeed[index]; }
float GamepadVib::getRightSpd(int index)  { return _rightSpeed[index]; }

void GamepadVib::_setDefault(){
	_btnMap[0]  = true; //A
	_btnMap[1]  = true; //B
	_btnMap[2]  = true; //X
	_btnMap[3]  = true; //Y
	_btnMap[4]  = false; //DPAD UP
	_btnMap[5]  = false; //DPAD DOWN
	_btnMap[6]  = false; //DPAD LEFT
	_btnMap[7]  = false; //DPAD RIGHT
	_btnMap[8]  = true; //L SHOULDER
	_btnMap[9]  = true; //R SHOULDER
	_btnMap[10] = true; //L THUMB
	_btnMap[11] = false; //R THUMB
	_btnMap[12] = false; //START
	_btnMap[13] = false; //BACK
	_btnMap[14] = true; //L TRIGGER
	_btnMap[15] = true;  //R TRIGGER

	//if 1000 or more, just do it forever lol
	_numPulses[0]  = 1; //A
	_numPulses[1]  = 3; //B
	_numPulses[2]  = 1000; //X
	_numPulses[3]  = 1000; //Y
	_numPulses[4]  = 0; //DPAD UP
	_numPulses[5]  = 0; //DPAD DOWN
	_numPulses[6]  = 0; //DPAD LEFT
	_numPulses[7]  = 0; //DPAD RIGHT
	_numPulses[8]  = 0; //L SHOULDER
	_numPulses[9]  = 0; //R SHOULDER
	_numPulses[10] = 1; //L THUMB
	_numPulses[11] = 0; //R THUMB
	_numPulses[12] = 0; //START
	_numPulses[13] = 0; //BACK
	_numPulses[14] = 1; //L TRIGGER
	_numPulses[15] = 0;  //R TRIGGER

	_leftSpeed[0]  = 0.0f; //A
	_leftSpeed[1]  = 1.0f; //B
	_leftSpeed[2]  = 1.0f; //X
	_leftSpeed[3]  = 0.3f; //Y
	_leftSpeed[4]  = 0.0f; //DPAD UP
	_leftSpeed[5]  = 0.0f; //DPAD DOWN
	_leftSpeed[6]  = 0.0f; //DPAD LEFT
	_leftSpeed[7]  = 0.0f; //DPAD RIGHT
	_leftSpeed[8]  = 0.5f; //L SHOULDER
	_leftSpeed[9]  = 0.0f; //R SHOULDER
	_leftSpeed[10] = 0.0f; //L THUMB
	_leftSpeed[11] = 0.0f; //R THUMB
	_leftSpeed[12] = 0.0f; //START
	_leftSpeed[13] = 0.0f; //BACK
	_leftSpeed[14] = 0.0f; //L TRIGGER
	_leftSpeed[15] = 0.75f;  //R TRIGGER

	_rightSpeed[0]  = 1.0f; //A
	_rightSpeed[1]  = 1.0f; //B
	_rightSpeed[2]  = 1.0f; //X
	_rightSpeed[3]  = 0.0f; //Y
	_rightSpeed[4]  = 0.0f; //DPAD UP
	_rightSpeed[5]  = 0.0f; //DPAD DOWN
	_rightSpeed[6]  = 0.0f; //DPAD LEFT
	_rightSpeed[7]  = 0.0f; //DPAD RIGHT
	_rightSpeed[8]  = 0.0f; //L SHOULDER
	_rightSpeed[9]  = 1.0f; //R SHOULDER
	_rightSpeed[10] = 1.0f; //L THUMB
	_rightSpeed[11] = 0.0f; //R THUMB
	_rightSpeed[12] = 0.0f; //START
	_rightSpeed[13] = 0.0f; //BACK
	_rightSpeed[14] = 1.0f; //L TRIGGER
	_rightSpeed[15] = 0.75f;  //R TRIGGER

	//0 = continuous
	_pulseFreq[0]  = 6; //A
	_pulseFreq[1]  = 4; //B
	_pulseFreq[2]  = 1; //X
	_pulseFreq[3]  = 2; //Y
	_pulseFreq[4]  = 0; //DPAD UP
	_pulseFreq[5]  = 0; //DPAD DOWN
	_pulseFreq[6]  = 0; //DPAD LEFT
	_pulseFreq[7]  = 0; //DPAD RIGHT
	_pulseFreq[8]  = 0; //L SHOULDER
	_pulseFreq[9]  = 0; //R SHOULDER
	_pulseFreq[10] = 6; //L THUMB
	_pulseFreq[11] = 0; //R THUMB
	_pulseFreq[12] = 0; //START
	_pulseFreq[13] = 0; //BACK
	_pulseFreq[14] = 6; //L TRIGGER
	_pulseFreq[15] = 0;  //R TRIGGER

	_exitButtonMap[0]  = false; //A
	_exitButtonMap[1]  = false; //B
	_exitButtonMap[2]  = false; //X
	_exitButtonMap[3]  = false; //Y
	_exitButtonMap[4]  = false; //DPAD UP
	_exitButtonMap[5]  = false; //DPAD DOWN
	_exitButtonMap[6]  = false; //DPAD LEFT
	_exitButtonMap[7]  = false; //DPAD RIGHT
	_exitButtonMap[8]  = false; //L SHOULDER
	_exitButtonMap[9]  = false; //R SHOULDER
	_exitButtonMap[10] = false; //L THUMB
	_exitButtonMap[11] = false; //R THUMB
	_exitButtonMap[12] = true; //START
	_exitButtonMap[13] = true; //BACK
	_exitButtonMap[14] = false; //L TRIGGER
	_exitButtonMap[15] = false;  //R TRIGGER
}

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
