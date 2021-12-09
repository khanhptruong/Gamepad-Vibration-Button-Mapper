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
	bool isVibContPressed();
	bool isVibContPosEdge();
	bool isVibOncePosEdge();

	void vibCont();
	void vibOnce();

	int getVibMapIndex();
	int getVibFreq();

private:
	static const int _numDigitalButtons = 14; //number of digital buttons
	static const int _numTotalButtons = 16; //digital buttons and two triggers

	int _vibMapIndex;

	bool _buttonsPressed[16];
	bool _buttonsPosEdge[16];
	bool _vibContMap[_numTotalButtons];
	bool _vibOnceMap[_numTotalButtons];
	float _vibLeftSpeed[_numTotalButtons];
	float _vibRightSpeed[_numTotalButtons];
	int _vibFreq[_numTotalButtons];
	bool _exitButtonCombo[_numTotalButtons];

	void _setDefault();
	void _getButtonsPressed();
	void _getButtonsPosEdge();
	bool _isMatching(bool array1[], bool array2[], int arraySize);
	bool _isAnyMatching(bool array1[], bool array2[], int arraySize);
	int _getMaxSpdIndex(bool inputs[], bool mapping[], float left[], float right[], int arraySize);
};

#endif // GAMEPADVIB_H