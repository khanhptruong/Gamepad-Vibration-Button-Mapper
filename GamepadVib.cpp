#include "GamepadVib.h"

GamepadVib::GamepadVib(int gpadIndex) : Gamepad(gpadIndex){
	_setDefault();
}

void GamepadVib::update(){
	Gamepad::update();
	_setButtonsPosEdge();
	_setButtonsPressed();
}

bool GamepadVib::isExit(){ return _isMatching(_exitButtonMap, buttonsPressed, numTotalButtons) == true; }

void GamepadVib::_setButtonsPressed(){
	for (int i = 0; i < numDigitalButtons; i++){
		buttonsPressed[i] = Gamepad::isPressed(xinputButtons[i]);
	}
	if (Gamepad::getLeftTrigger() > 0){
		buttonsPressed[numTotalButtons - 2] = true;
	}
	else{
		buttonsPressed[numTotalButtons - 2] = false;
	}
	if (Gamepad::getRightTrigger() > 0){
		buttonsPressed[numTotalButtons - 1] = true;
	}
	else{
		buttonsPressed[numTotalButtons - 1] = false;
	}
}

void GamepadVib::_setButtonsPosEdge(){
	for (int i = 0; i < numDigitalButtons; i++){
		buttonsPosEdge[i] = Gamepad::isPosEdge(xinputButtons[i]);
	}
	if (Gamepad::isLeftTriggerPosEdge() == true){
		buttonsPosEdge[numTotalButtons - 2] = true;
	}else{
		buttonsPosEdge[numTotalButtons - 2] = false;
	}
	if (Gamepad::isRightTriggerPosEdge() == true){
		buttonsPosEdge[numTotalButtons - 1] = true;
	}
	else{
		buttonsPosEdge[numTotalButtons - 1] = false;
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
	profileName = "Default";

	buttonMap[0] = false; //A
	buttonMap[1] = false; //B
	buttonMap[2] = false; //X
	buttonMap[3] = false; //Y
	buttonMap[4] = false; //DPAD UP
	buttonMap[5] = false; //DPAD DOWN
	buttonMap[6] = false; //DPAD LEFT
	buttonMap[7] = false; //DPAD RIGHT
	buttonMap[8] = false; //L SHOULDER
	buttonMap[9] = false; //R SHOULDER
	buttonMap[10] = false; //L THUMB
	buttonMap[11] = false; //R THUMB
	buttonMap[12] = false; //START
	buttonMap[13] = false; //BACK
	buttonMap[14] = false; //L TRIGGER
	buttonMap[15] = false;  //R TRIGGER

	//if 1000 or more, just do it forever lol
	numPulses[0] = 0; //A
	numPulses[1] = 0; //B
	numPulses[2] = 0; //X
	numPulses[3] = 0; //Y
	numPulses[4] = 0; //DPAD UP
	numPulses[5] = 0; //DPAD DOWN
	numPulses[6] = 0; //DPAD LEFT
	numPulses[7] = 0; //DPAD RIGHT
	numPulses[8] = 0; //L SHOULDER
	numPulses[9] = 0; //R SHOULDER
	numPulses[10] = 0; //L THUMB
	numPulses[11] = 0; //R THUMB
	numPulses[12] = 0; //START
	numPulses[13] = 0; //BACK
	numPulses[14] = 0; //L TRIGGER
	numPulses[15] = 0;  //R TRIGGER

	//0 = continuous
	pulseFreq[0] = 0.0f; //A
	pulseFreq[1] = 0.0f; //B
	pulseFreq[2] = 0.0f; //X
	pulseFreq[3] = 0.0f; //Y
	pulseFreq[4] = 0.0f; //DPAD UP
	pulseFreq[5] = 0.0f; //DPAD DOWN
	pulseFreq[6] = 0.0f; //DPAD LEFT
	pulseFreq[7] = 0.0f; //DPAD RIGHT
	pulseFreq[8] = 0.0f; //L SHOULDER
	pulseFreq[9] = 0.0f; //R SHOULDER
	pulseFreq[10] = 0.0f; //L THUMB
	pulseFreq[11] = 0.0f; //R THUMB
	pulseFreq[12] = 0.0f; //START
	pulseFreq[13] = 0.0f; //BACK
	pulseFreq[14] = 0.0f; //L TRIGGER
	pulseFreq[15] = 0.0f;  //R TRIGGER

	pulseOffFreq[0] = 0.0f; //A
	pulseOffFreq[1] = 0.0f; //B
	pulseOffFreq[2] = 0.0f; //X
	pulseOffFreq[3] = 0.0f; //Y
	pulseOffFreq[4] = 0.0f; //DPAD UP
	pulseOffFreq[5] = 0.0f; //DPAD DOWN
	pulseOffFreq[6] = 0.0f; //DPAD LEFT
	pulseOffFreq[7] = 0.0f; //DPAD RIGHT
	pulseOffFreq[8] = 0.0f; //L SHOULDER
	pulseOffFreq[9] = 0.0f; //R SHOULDER
	pulseOffFreq[10] = 0.0f; //L THUMB
	pulseOffFreq[11] = 0.0f; //R THUMB
	pulseOffFreq[12] = 0.0f; //START
	pulseOffFreq[13] = 0.0f; //BACK
	pulseOffFreq[14] = 0.0f; //L TRIGGER
	pulseOffFreq[15] = 0.0f;  //R TRIGGER

	leftSpeed[0] = 0.0f; //A
	leftSpeed[1] = 0.0f; //B
	leftSpeed[2] = 0.0f; //X
	leftSpeed[3] = 0.0f; //Y
	leftSpeed[4] = 0.0f; //DPAD UP
	leftSpeed[5] = 0.0f; //DPAD DOWN
	leftSpeed[6] = 0.0f; //DPAD LEFT
	leftSpeed[7] = 0.0f; //DPAD RIGHT
	leftSpeed[8] = 0.0f; //L SHOULDER
	leftSpeed[9] = 0.0f; //R SHOULDER
	leftSpeed[10] = 0.0f; //L THUMB
	leftSpeed[11] = 0.0f; //R THUMB
	leftSpeed[12] = 0.0f; //START
	leftSpeed[13] = 0.0f; //BACK
	leftSpeed[14] = 0.0f; //L TRIGGER
	leftSpeed[15] = 0.0f;  //R TRIGGER

	rightSpeed[0] = 0.0f; //A
	rightSpeed[1] = 0.0f; //B
	rightSpeed[2] = 0.0f; //X
	rightSpeed[3] = 0.0f; //Y
	rightSpeed[4] = 0.0f; //DPAD UP
	rightSpeed[5] = 0.0f; //DPAD DOWN
	rightSpeed[6] = 0.0f; //DPAD LEFT
	rightSpeed[7] = 0.0f; //DPAD RIGHT
	rightSpeed[8] = 0.0f; //L SHOULDER
	rightSpeed[9] = 0.0f; //R SHOULDER
	rightSpeed[10] = 0.0f; //L THUMB
	rightSpeed[11] = 0.0f; //R THUMB
	rightSpeed[12] = 0.0f; //START
	rightSpeed[13] = 0.0f; //BACK
	rightSpeed[14] = 0.0f; //L TRIGGER
	rightSpeed[15] = 0.0f;  //R TRIGGER

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
