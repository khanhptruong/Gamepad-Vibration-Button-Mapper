#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <Windows.h>
#include <Xinput.h>

class Gamepad
{
public:
	Gamepad();
	Gamepad(int gamepadIndex);

	void update();

	bool isConnected();
	bool isPressed(WORD xinputButton);
	bool isPosEdge(WORD xinputButton);
	bool isLeftTriggerPosEdge();
	bool isRightTriggerPosEdge();

	//returns 0-1.0 range
	float getLeftTrigger();
	float getRightTrigger();

	//speed in 0-1.0 range
	void vibrate(float speedLeft, float speedRight);
private:
	int _gamepadIndex;
	float _leftTriggerState, _rightTriggerState, _prevLeftTriggerState, _prevRightTriggerState;
	XINPUT_STATE _state, _prevState;
	XINPUT_VIBRATION _vibrationState;
};

#endif // GAMEPAD_H

static const WORD xinputButtons[] = {
	XINPUT_GAMEPAD_A,              //0
	XINPUT_GAMEPAD_B,              //1
	XINPUT_GAMEPAD_X,              //2
	XINPUT_GAMEPAD_Y,              //3
	XINPUT_GAMEPAD_DPAD_UP,        //4
	XINPUT_GAMEPAD_DPAD_DOWN,      //5
	XINPUT_GAMEPAD_DPAD_LEFT,      //6
	XINPUT_GAMEPAD_DPAD_RIGHT,     //7
	XINPUT_GAMEPAD_LEFT_SHOULDER,  //8
	XINPUT_GAMEPAD_RIGHT_SHOULDER, //9
	XINPUT_GAMEPAD_LEFT_THUMB,     //10
	XINPUT_GAMEPAD_RIGHT_THUMB,    //11
	XINPUT_GAMEPAD_START,          //12
	XINPUT_GAMEPAD_BACK            //13
};