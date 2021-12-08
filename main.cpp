#include "Gamepad.h"
#include "TickCounter.h"
#include <iostream>

using namespace std;

const int numDigitalButtons = 14; //number of digital buttons
const int numTotalButtons = 16; //digital buttons and two triggers
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

void updateButtonsPressed(bool btnsPressed[], Gamepad gpad);
void updateButtonsPosEdge(bool btnsPosEdge[], Gamepad gpad);
bool isInputMatching(bool btnCombo[], bool btnsPressed[]);
bool isAnyInputMatching(bool btnCombo[], bool btnsPressed[]);
int maxSpeed(bool btnComb[], bool btnsPressed[], float leftSpd[], float rightSpd[]);

int main(){
	const int fps = 16666; //microseconds
	const int pulseContRate = 0;
	const int gamepadIndex = 0; //which controller

	bool buttonsPressed[16];
	bool buttonsPosEdge[16];
	bool vibeCont[] = {
		false, //A
		false, //B
		false, //X
		false, //Y
		false, //DPAD UP
		false, //DPAD DOWN
		false, //DPAD LEFT
		false, //DPAD RIGHT
		false, //L SHOULDER
		false, //R SHOULDER
		false, //L THUMB
		false, //R THUMB
		false, //START
		false, //BACK
		false, //L TRIGGER
		true  //R TRIGGER
	};
	bool vibeOnce[] = {
		true, //A
		false, //B
		false, //X
		false, //Y
		false, //DPAD UP
		false, //DPAD DOWN
		false, //DPAD LEFT
		false, //DPAD RIGHT
		false, //L SHOULDER
		false, //R SHOULDER
		true, //L THUMB
		false, //R THUMB
		false, //START
		false, //BACK
		true, //L TRIGGER
		false  //R TRIGGER
	};
	bool exitButtonCombo[] = {
		false, //A
		false, //B
		false, //X
		false, //Y
		false, //DPAD UP
		false, //DPAD DOWN
		false, //DPAD LEFT
		false, //DPAD RIGHT
		false, //L SHOULDER
		false, //R SHOULDER
		false, //L THUMB
		false, //R THUMB
		true, //START
		true, //BACK
		false, //L TRIGGER
		false  //R TRIGGER
	};
	float vibLeftSpeed[] = {
		0.0f, //A
		0.0f, //B
		0.0f, //X
		0.0f, //Y
		0.0f, //DPAD UP
		0.0f, //DPAD DOWN
		0.0f, //DPAD LEFT
		0.0f, //DPAD RIGHT
		0.0f, //L SHOULDER
		0.0f, //R SHOULDER
		0.0f, //L THUMB
		0.0f, //R THUMB
		0.0f, //START
		0.0f, //BACK
		1.0f, //L TRIGGER
		0.7f  //R TRIGGER
	};
	float vibRightSpeed[] = {
		1.0f, //A
		0.0f, //B
		0.0f, //X
		0.0f, //Y
		0.0f, //DPAD UP
		0.0f, //DPAD DOWN
		0.0f, //DPAD LEFT
		0.0f, //DPAD RIGHT
		0.0f, //L SHOULDER
		0.0f, //R SHOULDER
		1.0f, //L THUMB
		0.0f, //R THUMB
		0.0f, //START
		0.0f, //BACK
		1.0f, //L TRIGGER
		0.7f  //R TRIGGER
	};

	Gamepad gamepad(gamepadIndex);
	TickCounter tickCounter(fps);
	TickCounter pulseLength(166666); //microseconds
	TickCounter pulseContinuous(pulseContRate);
	bool pulseContState = false;
	bool lastInputPosEdge = false; //was the last input pos edge?
	int vibIndex = 0;
	
	cout << "-------------------------------------" << endl;
	cout << "|     GAMEPAD VIBRATION PROGRAM     |" << endl;
	cout << "-------------------------------------" << endl;


	if (gamepad.isConnected() == true){
		cout << "gamepad connected" << endl;
	}else{
		cout << "gamepad disconnected" << endl;
		return 0;
	}

	while (1){
		if (tickCounter.isTick() == true){
			gamepad.update();
			updateButtonsPressed(buttonsPressed, gamepad);
			updateButtonsPosEdge(buttonsPosEdge, gamepad);

			if (gamepad.isConnected() == false){
				cout << "gamepad disconnected" << endl;
				break;
			}else if (isInputMatching(exitButtonCombo, buttonsPressed) == true){
				break;
			}else if (isAnyInputMatching(vibeCont, buttonsPressed) == true){
				vibIndex = maxSpeed(vibeCont, buttonsPressed, vibLeftSpeed, vibRightSpeed);
				if (isAnyInputMatching(vibeCont, buttonsPosEdge) == true){
					gamepad.vibrate(vibLeftSpeed[vibIndex], vibRightSpeed[vibIndex]);
					pulseContinuous.resetTick();
					pulseContState = true;
				}else if (pulseContinuous.isTick() == true && pulseContRate > 0){
					if (pulseContState == true){
						gamepad.vibrate(vibLeftSpeed[vibIndex], vibRightSpeed[vibIndex]);
						pulseContState = false;
					}else{
						gamepad.vibrate(0.0f, 0.0f);
						pulseContState = true;
					}
				}
			}else if (isAnyInputMatching(vibeOnce, buttonsPosEdge) == true){
				vibIndex = maxSpeed(vibeOnce, buttonsPosEdge, vibLeftSpeed, vibRightSpeed);
				gamepad.vibrate(vibLeftSpeed[vibIndex], vibRightSpeed[vibIndex]);
				pulseLength.resetTick();
			}else if (pulseLength.isTick() == true){
				gamepad.vibrate(0.0f, 0.0f);
			}
		}
	}

	cout << "--------------------------" << endl;
	cout << "|     END OF PROGRAM     |" << endl;
	cout << "--------------------------" << endl << endl;
	return 0;
}

void updateButtonsPressed(bool btnsPressed[], Gamepad gpad){
	for (int i = 0; i < numDigitalButtons; i++){
		btnsPressed[i] = gpad.isPressed(xinputButtons[i]);
	}
	if (gpad.getLeftTrigger() > 0){
		btnsPressed[numTotalButtons - 2] = true;
	}else{
		btnsPressed[numTotalButtons - 2] = false;
	}
	if (gpad.getRightTrigger() > 0){
		btnsPressed[numTotalButtons - 1] = true;
	}
	else{
		btnsPressed[numTotalButtons - 1] = false;
	}
}

void updateButtonsPosEdge(bool btnsPosEdge[], Gamepad gpad){
	for (int i = 0; i < numDigitalButtons; i++){
		btnsPosEdge[i] = gpad.isPosEdge(xinputButtons[i]);
	}
	if (gpad.isLeftTriggerPosEdge() == true){
		btnsPosEdge[numTotalButtons - 2] = true;
	}
	else{
		btnsPosEdge[numTotalButtons - 2] = false;
	}
	if (gpad.isRightTriggerPosEdge() == true){
		btnsPosEdge[numTotalButtons - 1] = true;
	}
	else{
		btnsPosEdge[numTotalButtons - 1] = false;
	}
}

bool isInputMatching(bool btnCombo[], bool btnsPressed[]){
	int i = 0;
	while ((btnCombo[i] == btnsPressed[i]) && i < numTotalButtons){
		i++;
	}
	if (i == numTotalButtons){
		return true;
	}else{
		return false;
	}
}

bool isAnyInputMatching(bool btnCombo[], bool btnsPressed[]){
	for (int i = 0; i < numTotalButtons; i++){
		if (btnCombo[i] == true && btnsPressed[i] == true){ return true; }
	}
	return false;
}

int maxSpeed(bool btnComb[], bool btnsPressed[], float leftSpd[], float rightSpd[]){
	float sum;
	float max = 0;
	int maxIndex = 0;
	for (int i = 0; i < numTotalButtons; i++){
		if (btnComb[i] == true && btnsPressed[i] == true){
			sum = leftSpd[i] + rightSpd[i];
			if (sum > max){
				max = sum;
				maxIndex = i;
			}
		}
	}
	return maxIndex;
}