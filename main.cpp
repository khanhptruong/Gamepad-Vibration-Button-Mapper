#include "GamepadVib.h"
#include "TickCounter.h"
#include <iostream>

int main(){
	const int updateFreq = 60; //Hz
	const int gamepadIndex = 0; //which controller

	GamepadVib gamepad(gamepadIndex);
	TickCounter tickCounter(updateFreq);
	TickCounter vibFreqCounter;
	bool pulseContState = false;
	
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
			gamepad.update();

			if (gamepad.isConnected() == false){
				std::cout << "gamepad disconnected" << std::endl;
				break;
			}else if (gamepad.isExit() == true){
				break;
			}else if (gamepad.isVibContPressed() == true){
				if (gamepad.isVibContPosEdge() == true){
					gamepad.vibCont();
					vibFreqCounter.setFreq(gamepad.getVibFreq());
					pulseContState = true;
				}else if (vibFreqCounter.isTick() == true && gamepad.getVibFreq() > 0){
					if (pulseContState == true){
						gamepad.vibrate(0.0f, 0.0f);
						pulseContState = false;
					}else{
						gamepad.vibCont();
						pulseContState = true;
					}
				}
			}else if (gamepad.isVibOncePosEdge() == true){
				gamepad.vibOnce();
				vibFreqCounter.setFreq(gamepad.getVibFreq());
			}else if (vibFreqCounter.isTick() == true){
				gamepad.vibrate(0.0f, 0.0f);
			}
		}
	}

	std::cout << "--------------------------" << std::endl;
	std::cout << "|     END OF PROGRAM     |" << std::endl;
	std::cout << "--------------------------" << std::endl << std::endl;
	return 0;
}