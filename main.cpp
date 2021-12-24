#include "GamepadVib.h"
#include "TickCounter.h"
#include <iostream>
#include <fstream>
#include <iomanip> //for setw
#include <vector>

using namespace std;

struct Buffer{
	string pName;
	bool mapBuff;
	int  numPulsesBuff;
	float pulseFreqBuff;
	float pulseOffBuff;
	float lSpdBuff;
	float rSpdBuff;
};
void adjoinSpd(float &spd, float spdToAdjoin);
void printProfile(GamepadVib gpad, int numBtns, int cWidth);
void readProfile(fstream &file, GamepadVib &gpad, int numBtns);
void writeProfile(fstream &file, GamepadVib gpad, int numBtns, int cWidth);
void endProgram(fstream &file, vector <string> &vectList);

int main(){
	const int updateFreq = 60; //Hz
	const int gamepadIndex = 0; //which controller
	const int numButtons = 16;
	const int colWidth = 8;
	const string fileName = "profiles.txt";

	//********************
	//initialize variables
	//********************

	GamepadVib gamepad(gamepadIndex);
	TickCounter tickCounter(updateFreq);
	float lSpd, rSpd;
	bool pulseActive[numButtons];
	TickCounter pulseTimer[numButtons];
	TickCounter pulseOffTimer[numButtons];
	int pulseCount[numButtons];

	fstream profilesData(fileName, ios::in | ios::app);
	int temp;
	vector <string> profilesList;

	if (gamepad.isConnected() == true){
		cout << "gamepad connected" << '\n';
	}
	else{
		cout << "gamepad disconnected" << '\n';
		endProgram(profilesData, profilesList);
		return EXIT_FAILURE;
	}
	if (!profilesData){
		cout << "error opening file" << '\n';
		endProgram(profilesData, profilesList);
		return EXIT_FAILURE;
	}
	for (int i = 0; i < numButtons; i++){
		pulseActive[i] = false;
		pulseTimer[i].setFreq(0);
		pulseOffTimer[i].setFreq(0);
		pulseCount[i] = 0;
	}
	
	cout << "-------------------------------------" << '\n';
	cout << "|     GAMEPAD VIBRATION PROGRAM     |" << '\n';
	cout << "-------------------------------------" << '\n';

	//************************
	//reading/writing profiles
	//************************

	//cout << "writing to profiles.txt..." << endl;
	//writeProfiles(profilesData, gamepad, numButtons, colWidth);
	//cout << "done writing" << endl;
	while (!profilesData.eof() || !profilesData.fail()){
		temp = profilesData.peek();
		if (temp == EOF){ break; }
		readProfile(profilesData, gamepad, numButtons);
		profilesList.push_back(gamepad.profileName);
	}
	cout << "+++++++++++++++" << '\n';
	cout << "Profiles List: " << '\n';
	for (int i = 0; i < profilesList.size(); i++){
		cout << i << ". " << profilesList[i] << '\n';
	}

	int selection;
	cout << "select profile number: ";
	cin >> selection;

	profilesData.seekg(0L, ios::beg);
	while (!profilesData.eof() || !profilesData.fail()){
		temp = profilesData.peek();
		if (temp == EOF){ break; }
		readProfile(profilesData, gamepad, numButtons);
		if (gamepad.profileName == profilesList[selection]){ break; }
	}
	cout << "profile loaded" << '\n';
	printProfile(gamepad, numButtons, colWidth);

	//***************
	//vibration loop
	//set number of pulses to 0 for continuous vibrations
	//set pulseFreq > 0 for min pulse time of continuous vibs
	//set pulseNum >=1000 for infinite pulses
	//***************
	while (1){
		if (tickCounter.isTick() == true){
			gamepad.update();
			if (gamepad.isExit() == true){ 
				cout << "gamepad disconnected" << '\n';
				break; 
			}
			lSpd = 0.0f;
			rSpd = 0.0f;
			
			for (int i = 0; i < numButtons; i++){
				if (gamepad.btnMap[i] == true && gamepad.buttonsPressed[i] == true){ //when button is held...
					if (gamepad.numPulses[i] == 0){ //adjoin continuous vibrations
						adjoinSpd(lSpd, gamepad.leftSpeed[i]);
						adjoinSpd(rSpd, gamepad.rightSpeed[i]);
						pulseActive[i] = true; 
						pulseTimer[i].setFreq(gamepad.pulseFreq[i]);
						pulseCount[i] = gamepad.numPulses[i];
					}else if (gamepad.buttonsPosEdge[i] == true){ //initialize and adjoin new pulsing vibrations
						adjoinSpd(lSpd, gamepad.leftSpeed[i]);
						adjoinSpd(rSpd, gamepad.rightSpeed[i]);
						pulseActive[i] = true;
						pulseTimer[i].setFreq(gamepad.pulseFreq[i]);
						pulseOffTimer[i].setFreq(gamepad.pulseOffFreq[i]);
						pulseCount[i] = gamepad.numPulses[i];
					}else if (pulseActive[i] == true && pulseCount[i] > 0){ //on active pulse...
						if (pulseTimer[i].isTick() == true){ //if tick, deactivate
							pulseActive[i] = false;
							pulseOffTimer[i].resetTick();
							if (pulseCount[i] < 1000){ pulseCount[i]--; }
						}else if (pulseTimer[i].isTick() == false){ //if not tick, maintain adjoin vibration
							adjoinSpd(lSpd, gamepad.leftSpeed[i]);
							adjoinSpd(rSpd, gamepad.rightSpeed[i]);
						}
					}else if (pulseActive[i] == false && pulseCount[i] > 0) { //on deactive pulse...
						if (pulseOffTimer[i].isTick() == true){ //if tick, activate pulse and adjoin vibration ELSE if not tick, adjoin nothing
							adjoinSpd(lSpd, gamepad.leftSpeed[i]);
							adjoinSpd(rSpd, gamepad.rightSpeed[i]);
							pulseActive[i] = true;
							pulseTimer[i].resetTick();
						} 
					}
				}else if (gamepad.btnMap[i] == true && gamepad.buttonsPressed[i] == false){ //when mapped button released...
					if (pulseActive[i] == true && pulseCount[i] > 0){ //on active pulse... (pulse mode when never be true and > 0 at the same time)
						if (pulseTimer[i].isTick() == true){ //if tick, deactivate
							pulseActive[i] = false;
							pulseCount[i] = 0;
						}else if (pulseTimer[i].isTick() == false){ //if not tick, maintain adjoin vibration
							adjoinSpd(lSpd, gamepad.leftSpeed[i]);
							adjoinSpd(rSpd, gamepad.rightSpeed[i]);
						}
					}else if (pulseActive[i] == false && pulseCount[i] > 0) { //on deactive pulse...
						pulseCount[i] = 0;
					}else if (pulseActive[i] == true && pulseCount[i] == 0){ //set min pulse for continuous vib (only continuous can be true and == 0)
						if (pulseTimer[i].isTick() == false){ //keep vibing until time is up
							adjoinSpd(lSpd, gamepad.leftSpeed[i]);
							adjoinSpd(rSpd, gamepad.rightSpeed[i]);
						}else{ //stop vibing
							pulseActive[i] = false;
						}
					}
				}
			} //for loop
			gamepad.vibrate(lSpd, rSpd);
		} //tickCounter
	} //while loop

	endProgram(profilesData, profilesList);
	return EXIT_SUCCESS;
}

void adjoinSpd(float &spd, float spdToAdjoin){
	if (spd < spdToAdjoin){ spd = spdToAdjoin; }
}

void printProfile(GamepadVib gpad, int numBtns, int cWidth){
	cout << gpad.profileName << endl;
	for (int i = 0; i < numBtns; i++){
		cout << setw(cWidth) << gpad.btnMap[i];
		cout << setw(cWidth) << gpad.numPulses[i];
		cout << setw(cWidth) << gpad.pulseFreq[i];
		cout << setw(cWidth) << gpad.pulseOffFreq[i];
		cout << setw(cWidth) << gpad.leftSpeed[i];
		cout << setw(cWidth) << gpad.rightSpeed[i];
		cout << '\n';
	}
}

void readProfile(fstream &file, GamepadVib &gpad, int numBtns){
	Buffer buff;
	getline(file, buff.pName);
	if (buff.pName == ""){ return; }
	gpad.profileName = buff.pName;
	for (int i = 0; i < numBtns; i++){
		file >> buff.mapBuff;
		file >> buff.numPulsesBuff;
		file >> buff.pulseFreqBuff;
		file >> buff.pulseOffBuff;
		file >> buff.lSpdBuff;
		file >> buff.rSpdBuff;

		gpad.btnMap[i] = buff.mapBuff;
		gpad.numPulses[i] = buff.numPulsesBuff;
		gpad.pulseFreq[i] = buff.pulseFreqBuff;
		gpad.pulseOffFreq[i] = buff.pulseOffBuff;
		gpad.leftSpeed[i] = buff.lSpdBuff;
		gpad.rightSpeed[i] = buff.rSpdBuff;
	}
	file.ignore(1, '\n');
}

void writeProfile(fstream &file, GamepadVib gpad, int numBtns, int cWidth){
	file << gpad.profileName << endl;
	for (int i = 0; i < numBtns; i++){
		file << setw(cWidth) << gpad.btnMap[i];
			file << setw(cWidth) << gpad.numPulses[i];
		file << setw(cWidth) << gpad.pulseFreq[i];
		file << setw(cWidth) << gpad.pulseOffFreq[i];
		file << setw(cWidth) << gpad.leftSpeed[i];
		file << setw(cWidth) << gpad.rightSpeed[i];
		file << '\n';
	}
}

void endProgram(fstream &file, vector <string> &vectList){
	cout << "--------------------------" << '\n';
	cout << "|     END OF PROGRAM     |" << '\n';
	cout << "--------------------------" << '\n';
	file.close();
	vectList.empty();
	system("pause");
}