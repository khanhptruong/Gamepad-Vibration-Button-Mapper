#include "GamepadVib.h"

GamepadVib::GamepadVib(int gpadIndex) : Gamepad(gpadIndex){
	_vibMapIndex = 0;
	_setDefault();
}

void GamepadVib::_setDefault(){
	_vibContMap[0]  = false; //A
	_vibContMap[1]  = false; //B
	_vibContMap[2]  = false; //X
	_vibContMap[3]  = false; //Y
	_vibContMap[4]  = false; //DPAD UP
	_vibContMap[5]  = false; //DPAD DOWN
	_vibContMap[6]  = false; //DPAD LEFT
	_vibContMap[7]  = false; //DPAD RIGHT
	_vibContMap[8]  = false; //L SHOULDER
	_vibContMap[9]  = false; //R SHOULDER
	_vibContMap[10] = false; //L THUMB
	_vibContMap[11] = false; //R THUMB
	_vibContMap[12] = false; //START
	_vibContMap[13] = false; //BACK
	_vibContMap[14] = false; //L TRIGGER
	_vibContMap[15] = true;  //R TRIGGER

	_vibOnceMap[0]  = true; //A
	_vibOnceMap[1]  = false; //B
	_vibOnceMap[2]  = false; //X
	_vibOnceMap[3]  = false; //Y
	_vibOnceMap[4]  = false; //DPAD UP
	_vibOnceMap[5]  = false; //DPAD DOWN
	_vibOnceMap[6]  = false; //DPAD LEFT
	_vibOnceMap[7]  = false; //DPAD RIGHT
	_vibOnceMap[8]  = false; //L SHOULDER
	_vibOnceMap[9]  = false; //R SHOULDER
	_vibOnceMap[10] = true; //L THUMB
	_vibOnceMap[11] = false; //R THUMB
	_vibOnceMap[12] = false; //START
	_vibOnceMap[13] = false; //BACK
	_vibOnceMap[14] = true; //L TRIGGER
	_vibOnceMap[15] = false;  //R TRIGGER

	_vibLeftSpeed[0]  = 0.0f; //A
	_vibLeftSpeed[1]  = 0.0f; //B
	_vibLeftSpeed[2]  = 0.0f; //X
	_vibLeftSpeed[3]  = 0.0f; //Y
	_vibLeftSpeed[4]  = 0.0f; //DPAD UP
	_vibLeftSpeed[5]  = 0.0f; //DPAD DOWN
	_vibLeftSpeed[6]  = 0.0f; //DPAD LEFT
	_vibLeftSpeed[7]  = 0.0f; //DPAD RIGHT
	_vibLeftSpeed[8]  = 0.0f; //L SHOULDER
	_vibLeftSpeed[9]  = 0.0f; //R SHOULDER
	_vibLeftSpeed[10] = 0.0f; //L THUMB
	_vibLeftSpeed[11] = 0.0f; //R THUMB
	_vibLeftSpeed[12] = 0.0f; //START
	_vibLeftSpeed[13] = 0.0f; //BACK
	_vibLeftSpeed[14] = 0.0f; //L TRIGGER
	_vibLeftSpeed[15] = 0.75f;  //R TRIGGER

	_vibRightSpeed[0]  = 1.0f; //A
	_vibRightSpeed[1]  = 0.0f; //B
	_vibRightSpeed[2]  = 0.0f; //X
	_vibRightSpeed[3]  = 0.0f; //Y
	_vibRightSpeed[4]  = 0.0f; //DPAD UP
	_vibRightSpeed[5]  = 0.0f; //DPAD DOWN
	_vibRightSpeed[6]  = 0.0f; //DPAD LEFT
	_vibRightSpeed[7]  = 0.0f; //DPAD RIGHT
	_vibRightSpeed[8]  = 0.0f; //L SHOULDER
	_vibRightSpeed[9]  = 0.0f; //R SHOULDER
	_vibRightSpeed[10] = 1.0f; //L THUMB
	_vibRightSpeed[11] = 0.0f; //R THUMB
	_vibRightSpeed[12] = 0.0f; //START
	_vibRightSpeed[13] = 0.0f; //BACK
	_vibRightSpeed[14] = 1.0f; //L TRIGGER
	_vibRightSpeed[15] = 0.75f;  //R TRIGGER

	//0 = continuous
	_vibFreq[0]  = 6; //A
	_vibFreq[1]  = 0; //B
	_vibFreq[2]  = 0; //X
	_vibFreq[3]  = 0; //Y
	_vibFreq[4]  = 0; //DPAD UP
	_vibFreq[5]  = 0; //DPAD DOWN
	_vibFreq[6]  = 0; //DPAD LEFT
	_vibFreq[7]  = 0; //DPAD RIGHT
	_vibFreq[8]  = 0; //L SHOULDER
	_vibFreq[9]  = 0; //R SHOULDER
	_vibFreq[10] = 6; //L THUMB
	_vibFreq[11] = 0; //R THUMB
	_vibFreq[12] = 0; //START
	_vibFreq[13] = 0; //BACK
	_vibFreq[14] = 6; //L TRIGGER
	_vibFreq[15] = 0;  //R TRIGGER

	_exitButtonCombo[0]  = false; //A
	_exitButtonCombo[1]  = false; //B
	_exitButtonCombo[2]  = false; //X
	_exitButtonCombo[3]  = false; //Y
	_exitButtonCombo[4]  = false; //DPAD UP
	_exitButtonCombo[5]  = false; //DPAD DOWN
	_exitButtonCombo[6]  = false; //DPAD LEFT
	_exitButtonCombo[7]  = false; //DPAD RIGHT
	_exitButtonCombo[8]  = false; //L SHOULDER
	_exitButtonCombo[9]  = false; //R SHOULDER
	_exitButtonCombo[10] = false; //L THUMB
	_exitButtonCombo[11] = false; //R THUMB
	_exitButtonCombo[12] = true; //START
	_exitButtonCombo[13] = true; //BACK
	_exitButtonCombo[14] = false; //L TRIGGER
	_exitButtonCombo[15] = false;  //R TRIGGER
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

bool GamepadVib::_isAnyMatching(bool array1[], bool array2[], int arraySize){
	for (int i = 0; i < arraySize; i++){
		if (array1[i] == true && array2[i] == true){ return true; }
	}
	return false;
}

int GamepadVib::_getMaxSpdIndex(bool inputs[], bool mapping[], float leftSpd[], float rightSpd[], int arraySize){
	float sum;
	float max = 0;
	int maxIndex = 0;
	for (int i = 0; i < arraySize; i++){
		if (inputs[i] == true && mapping[i] == true){
			sum = leftSpd[i] + rightSpd[i];
			if (sum > max){
				max = sum;
				maxIndex = i;
			}
		}
	}
	return maxIndex;
}

void GamepadVib::update(){
	Gamepad::update();
	_getButtonsPosEdge();
	_getButtonsPressed();
}

bool GamepadVib::isExit(){
	if (_isMatching(_exitButtonCombo, _buttonsPressed, _numTotalButtons) == true){
		return true;
	}else{
		return false;
	}
}

bool GamepadVib::isVibContPressed(){
	return _isAnyMatching(_buttonsPressed, _vibContMap, _numTotalButtons);
}

bool GamepadVib::isVibContPosEdge(){
	return _isAnyMatching(_buttonsPosEdge, _vibContMap, _numTotalButtons);
}

bool GamepadVib::isVibOncePosEdge(){
	return _isAnyMatching(_buttonsPosEdge, _vibOnceMap, _numTotalButtons);
}

void GamepadVib::vibCont(){
	_vibMapIndex = _getMaxSpdIndex(_buttonsPressed, _vibContMap, _vibLeftSpeed, _vibRightSpeed, _numTotalButtons);
	Gamepad::vibrate(_vibLeftSpeed[_vibMapIndex], _vibRightSpeed[_vibMapIndex]);
}

void GamepadVib::vibOnce(){
	_vibMapIndex = _getMaxSpdIndex(_buttonsPosEdge, _vibOnceMap, _vibLeftSpeed, _vibRightSpeed, _numTotalButtons);
	Gamepad::vibrate(_vibLeftSpeed[_vibMapIndex], _vibRightSpeed[_vibMapIndex]);
}

int GamepadVib::getVibMapIndex(){
	return _vibMapIndex;
}

int GamepadVib::getVibFreq(){
	return _vibFreq[_vibMapIndex];
}