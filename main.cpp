#include "GamepadVib.h"
#include "TickCounter.h"
#include <iostream>

void adjoinSpd(float &spd, float spdToAdjoin);

int main(){
	const int updateFreq = 60; //Hz
	const int gamepadIndex = 0; //which controller
	const int numButtons = 16;

	GamepadVib gamepad(gamepadIndex);
	TickCounter tickCounter(updateFreq);
	float lSpd, rSpd;

	bool pulseActive[numButtons];
	TickCounter pulseTimer[numButtons];
	TickCounter pulseOffTimer[numButtons];
	int pulseCount[numButtons];

	for (int i = 0; i < numButtons; i++){ 
		pulseActive[i] = false;
		pulseTimer[i].setFreq(0);
		pulseOffTimer[i].setFreq(0);
		pulseCount[i] = 0; 
	}
	
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "|     GAMEPAD VIBRATION PROGRAM     |" << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	if (gamepad.isConnected() == true){
		std::cout << "gamepad connected" << std::endl;
	}else{
		std::cout << "gamepad disconnected" << std::endl;
		return 0;
	}

	while (1){
		if (tickCounter.isTick() == true){
			if (gamepad.isExit() == true){ break; }
			gamepad.update();
			lSpd = 0.0f;
			rSpd = 0.0f;
			
			for (int i = 0; i < numButtons; i++){
				if (gamepad.getBtnMap(i) == true && gamepad.getBtnPressed(i) == true){ //when button is held...
					if (gamepad.getPulseFreq(i) == 0){ //adjoin continuous vibrations
						adjoinSpd(lSpd, gamepad.getLeftSpd(i));
						adjoinSpd(rSpd, gamepad.getRightSpd(i));
					}else if (gamepad.getBtnPosEdge(i) == true){ //initialize and adjoin new pulsing vibrations
						adjoinSpd(lSpd, gamepad.getLeftSpd(i));
						adjoinSpd(rSpd, gamepad.getRightSpd(i));
						pulseActive[i] = true;
						pulseTimer[i].setFreq(gamepad.getPulseFreq(i));
						pulseOffTimer[i].setFreq(gamepad.getPulseOffFreq(i));
						pulseCount[i] = gamepad.getNumPulses(i);
					}else if (pulseActive[i] == true && pulseCount[i] > 0){ //on active pulse...
						if (pulseTimer[i].isTick() == true){ //if tick, deactivate
							pulseActive[i] = false;
							pulseOffTimer[i].resetTick();
							if (pulseCount[i] < 1000){ pulseCount[i]--; }
						}else if (pulseTimer[i].isTick() == false){ //if not tick, maintain adjoin vibration
							adjoinSpd(lSpd, gamepad.getLeftSpd(i));
							adjoinSpd(rSpd, gamepad.getRightSpd(i));
						}
					}else if (pulseActive[i] == false && pulseCount[i] > 0) { //on deactive pulse...
						if (pulseOffTimer[i].isTick() == true){ //if tick, activate pulse and adjoin vibration ELSE if not tick, adjoin nothing
							adjoinSpd(lSpd, gamepad.getLeftSpd(i));
							adjoinSpd(rSpd, gamepad.getRightSpd(i));
							pulseActive[i] = true;
							pulseTimer[i].resetTick();
						} 
					}
				}else if (gamepad.getBtnMap(i) == true && gamepad.getBtnPressed(i) == false){ //when mapped button released...
					if (pulseActive[i] == true && pulseCount[i] > 0){ //on active pulse...
						if (pulseTimer[i].isTick() == true){ //if tick, deactivate
							pulseActive[i] = false;
							pulseCount[i] = 0;
						}else if (pulseTimer[i].isTick() == false){ //if not tick, maintain adjoin vibration
							adjoinSpd(lSpd, gamepad.getLeftSpd(i));
							adjoinSpd(rSpd, gamepad.getRightSpd(i));
						}
					}else if (pulseActive[i] == false && pulseCount[i] > 0) { //on deactive pulse...
						pulseCount[i] = 0;
					}
				}
			} //for loop
			gamepad.vibrate(lSpd, rSpd);
		} //tickCounter
	} //while loop

	std::cout << "--------------------------" << std::endl;
	std::cout << "|     END OF PROGRAM     |" << std::endl;
	std::cout << "--------------------------" << std::endl << std::endl;
	return 0;
}

void adjoinSpd(float &spd, float spdToAdjoin){
	if (spd < spdToAdjoin){ spd = spdToAdjoin; }
}