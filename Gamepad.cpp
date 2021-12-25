#include "Gamepad.h"

#pragma comment(lib,"Xinput9_1_0.lib")

Gamepad::Gamepad(){
	_gamepadIndex          = 0;
	ZeroMemory(&_state, sizeof(XINPUT_STATE));
	ZeroMemory(&_prevState, sizeof(XINPUT_STATE));
	ZeroMemory(&_vibrationState, sizeof(XINPUT_VIBRATION));
}

Gamepad::Gamepad(int gamepadIndex){
	if (gamepadIndex >= 0 && gamepadIndex <= 3){
		_gamepadIndex = gamepadIndex;
	}else{
		_gamepadIndex = 0;
	}
	ZeroMemory(&_state, sizeof(XINPUT_STATE));
	ZeroMemory(&_prevState, sizeof(XINPUT_STATE));
	ZeroMemory(&_vibrationState, sizeof(XINPUT_VIBRATION));
}

void Gamepad::update(){
	_prevState = _state;
	XInputGetState(_gamepadIndex, &_state);
}

bool Gamepad::isConnected(){
	if (XInputGetState(_gamepadIndex, &_state) == ERROR_SUCCESS){
		return true;  //gamepad is connected
	}else{
		return false; //gamepad not connected
	}
}

bool Gamepad::isPressed(WORD xinputButton){
	if ((_state.Gamepad.wButtons & xinputButton) != 0){
		return true;  //button is pressed
	}else{
		return false; //button not pressed
	}
}

bool Gamepad::isPosEdge(WORD xinputButton){
	bool buttonPressed = ((_state.Gamepad.wButtons & xinputButton) != 0);
	bool prevButtonPressed = ((_prevState.Gamepad.wButtons & xinputButton) != 0);

	if (buttonPressed == true && prevButtonPressed == false)
	{
		return true;
	}else{
		return false;
	}
}

bool Gamepad::isLeftTriggerPosEdge(){
	bool triggerPulled = (_state.Gamepad.bLeftTrigger > 0);
	bool prevTriggerPulled = (_prevState.Gamepad.bLeftTrigger > 0);

	if (triggerPulled == true && prevTriggerPulled == false)
	{
		return true;
	}
	else{
		return false;
	}
}
bool Gamepad::isRightTriggerPosEdge(){
	bool triggerPulled = (_state.Gamepad.bRightTrigger > 0);
	bool prevTriggerPulled = (_prevState.Gamepad.bRightTrigger > 0);

	if (triggerPulled == true && prevTriggerPulled == false)
	{
		return true;
	}
	else{
		return false;
	}
}

//0-255 remapped to 0-1.0
float Gamepad::getLeftTrigger(){
	return (float)_state.Gamepad.bLeftTrigger / 255;
}
float Gamepad::getRightTrigger(){
	return (float)_state.Gamepad.bRightTrigger / 255;
}

//0-65535 remapped to 0-1.0
void Gamepad::vibrate(float speedLeft, float speedRight){
	int convertedSpeedLeft;
	int convertedSpeedRight;

	if (speedLeft >= 0 && speedLeft <= 1.0f){
		convertedSpeedLeft = int(speedLeft * 65535.0f);
	}else{
		convertedSpeedLeft = 0;
	}
	if (speedRight >= 0 && speedLeft <= 1.0f){
		convertedSpeedRight = int(speedRight * 65535.0f);
	}
	else{
		convertedSpeedRight = 0;
	}

	_vibrationState.wLeftMotorSpeed = convertedSpeedLeft;
	_vibrationState.wRightMotorSpeed = convertedSpeedRight;
	XInputSetState(_gamepadIndex, &_vibrationState);
}