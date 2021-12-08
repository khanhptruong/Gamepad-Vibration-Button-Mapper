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