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
		cout << "gamepad connected" << endl;
	}
	else{
		cout << "gamepad disconnected" << endl;
		return EXIT_FAILURE;
	}
	if (!profilesData){
		cout << "error opening file";
		return EXIT_FAILURE;
	}
	for (int i = 0; i < numButtons; i++){
		pulseActive[i] = false;
		pulseTimer[i].setFreq(0);
		pulseOffTimer[i].setFreq(0);
		pulseCount[i] = 0;
	}
	
	cout << "-------------------------------------" << endl;
	cout << "|     GAMEPAD VIBRATION PROGRAM     |" << endl;
	cout << "-------------------------------------" << endl;

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
		profilesList.push_back(gamepad.getProfileName());
	}
	cout << "+++++++++++++++" << endl;
	cout << "Profiles List: " << endl;
	for (int i = 0; i < profilesList.size(); i++){
		cout << i << ". " << profilesList[i] << endl;
	}

	int selection;
	cout << "select profile number: ";
	cin >> selection;

	profilesData.seekg(0L, ios::beg);
	while (!profilesData.eof() || !profilesData.fail()){
		temp = profilesData.peek();
		if (temp == EOF){ break; }
		readProfile(profilesData, gamepad, numButtons);
		if (gamepad.getProfileName() == profilesList[selection]){ break; }
	}
	cout << "profile loaded" << endl;
	printProfile(gamepad, numButtons, colWidth);

	//***************
	//vibration loop
	//***************
	while (1){
		if (tickCounter.isTick() == true){
			gamepad.update();
			if (gamepad.isExit() == true){ 
				cout << "gamepad disconnected" << endl;
				break; 
			}
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

	cout << "--------------------------" << endl;
	cout << "|     END OF PROGRAM     |" << endl;
	cout << "--------------------------" << endl << endl;
	profilesData.close();
	profilesList.empty();
	return EXIT_SUCCESS;
}

void adjoinSpd(float &spd, float spdToAdjoin){
	if (spd < spdToAdjoin){ spd = spdToAdjoin; }
}

void printProfile(GamepadVib gpad, int numBtns, int cWidth){
	cout << gpad.getProfileName() << endl;
	for (int i = 0; i < numBtns; i++){
		cout << setw(cWidth) << gpad.getBtnMap(i);
		cout << setw(cWidth) << gpad.getNumPulses(i);
		cout << setw(cWidth) << gpad.getPulseFreq(i);
		cout << setw(cWidth) << gpad.getPulseOffFreq(i);
		cout << setw(cWidth) << gpad.getLeftSpd(i);
		cout << setw(cWidth) << gpad.getRightSpd(i);
		cout << endl;
	}
}

void readProfile(fstream &file, GamepadVib &gpad, int numBtns){
	Buffer buff;
	getline(file, buff.pName);
	if (buff.pName == ""){ return; }
	gpad.setProfileName(buff.pName);
	for (int i = 0; i < numBtns; i++){
		file >> buff.mapBuff;
		file >> buff.numPulsesBuff;
		file >> buff.pulseFreqBuff;
		file >> buff.pulseOffBuff;
		file >> buff.lSpdBuff;
		file >> buff.rSpdBuff;

		gpad.setBtnMap(buff.mapBuff, i);
		gpad.setNumPulses(buff.numPulsesBuff, i);
		gpad.setPulseFreq(buff.pulseFreqBuff, i);
		gpad.setPulseOffFreq(buff.pulseOffBuff, i);
		gpad.setLeftSpd(buff.lSpdBuff, i);
		gpad.setRightSpd(buff.rSpdBuff, i);
	}
	file.ignore(1, '\n');
}

void writeProfile(fstream &file, GamepadVib gpad, int numBtns, int cWidth){
	file << gpad.getProfileName() << endl;
	for (int i = 0; i < numBtns; i++){
		file << setw(cWidth) << gpad.getBtnMap(i);
		file << setw(cWidth) << gpad.getNumPulses(i);
		file << setw(cWidth) << gpad.getPulseFreq(i);
		file << setw(cWidth) << gpad.getPulseOffFreq(i);
		file << setw(cWidth) << gpad.getLeftSpd(i);
		file << setw(cWidth) << gpad.getRightSpd(i);
		file << endl;
	}
}