#include "GamepadVib.h"
#include "TickCounter.h"
#include <iostream>
#include <fstream>
#include <iomanip> //for setw
#include <vector>
#include <comdef.h> //allocate sttring
using namespace std;

const float updateFreq = 30.0f; //Hz
const short int gamepadIndex = 0; //which controller
const short int colWidth = 8;
const string fileName = "profiles.txt";
vector <string> profilesList;

GamepadVib gamepad(gamepadIndex);
const short int numButtons = gamepad.numTotalButtons;
TickCounter tickCounter(updateFreq);
short int pulseCount[numButtons];
bool pulseActive[numButtons];
TickCounter pulseTimer[numButtons];
TickCounter pulseOffTimer[numButtons];
float lSpd, rSpd;
bool exitLoop = false;

struct Buffer{
	string pName;
	bool mapBuff;
	int  numPulsesBuff;
	float pulseFreqBuff;
	float pulseOffBuff;
	float lSpdBuff;
	float rSpdBuff;
};

extern "C" __declspec(dllexport) void readAllProfiles();
extern "C" __declspec(dllexport) int getNumProfiles();
extern "C" __declspec(dllexport) BSTR getProfileName(int index);
extern "C" __declspec(dllexport) void setProfile(int index);
extern "C" __declspec(dllexport) void vibrationLoop();
extern "C" __declspec(dllexport) void endVibLoop();
extern "C" __declspec(dllexport) bool isGamepadConnected();
extern "C" __declspec(dllexport) float getData(int field, int index);

void readAllProfiles();
int getNumProfiles();
BSTR getProfileName(int index);
void setProfile(int index);
void vibrationLoop();
void endVibLoop();
bool isGamepadConnected();
float getData(int field, int index);

void adjoinSpd(float &spd, float spdToAdjoin);
void printProfile(GamepadVib gpad, int numBtns, int cWidth);
void readProfile(fstream &file, GamepadVib &gpad, int numBtns);
void writeProfile(fstream &file, GamepadVib gpad, int numBtns, int cWidth);

void readAllProfiles() {
	int temp;
	fstream profilesData(fileName, ios::in | ios::app);

	while (!profilesData.eof() || !profilesData.fail()) {
		temp = profilesData.peek();
		if (temp == EOF) { break; }
		readProfile(profilesData, gamepad, numButtons);
		profilesList.push_back(gamepad.profileName);
	}
	profilesData.close();
}

int getNumProfiles() {
	return profilesList.size();
}

BSTR getProfileName(int index) {
	string str = profilesList[index];
	std::wstring widestr = std::wstring(str.begin(), str.end());
	const wchar_t* widecstr = widestr.c_str();
	return SysAllocString(widecstr);
}

void setProfile(int index) {
	int temp;
	fstream profilesData(fileName, ios::in | ios::app);

	profilesData.seekg(0L, ios::beg);
	while (!profilesData.eof() || !profilesData.fail()) {
		temp = profilesData.peek();
		if (temp == EOF) { break; }
		readProfile(profilesData, gamepad, numButtons);
		if (gamepad.profileName == profilesList[index]) { break; }
	}
	profilesData.close();
}

void vibrationLoop(){
	exitLoop == false;
	
	//***************
	//vibration loop
	//set number of pulses to 0 for continuous vibrations
	//set pulseFreq > 0 for min pulse time of continuous vibs
	//set pulseNum >=1000 for infinite pulses
	//***************
	while (exitLoop == false){
		if (tickCounter.isTick() == true){
			gamepad.update();
			if (gamepad.isConnected() == true) {
				lSpd = 0.0f;
				rSpd = 0.0f;
				for (int i = 0; i < numButtons; i++) {
					if (gamepad.buttonMap[i] == true && gamepad.buttonsPressed[i] == true) { //when button is held...
						if (gamepad.numPulses[i] == 0) { //adjoin continuous vibrations
							adjoinSpd(lSpd, gamepad.leftSpeed[i]);
							adjoinSpd(rSpd, gamepad.rightSpeed[i]);
							pulseActive[i] = true;
							pulseTimer[i].setFreq(gamepad.pulseFreq[i]);
							pulseCount[i] = gamepad.numPulses[i];
						}
						else if (gamepad.buttonsPosEdge[i] == true) { //initialize and adjoin new pulsing vibrations
							adjoinSpd(lSpd, gamepad.leftSpeed[i]);
							adjoinSpd(rSpd, gamepad.rightSpeed[i]);
							pulseActive[i] = true;
							pulseTimer[i].setFreq(gamepad.pulseFreq[i]);
							pulseOffTimer[i].setFreq(gamepad.pulseOffFreq[i]);
							pulseCount[i] = gamepad.numPulses[i];
						}
						else if (pulseActive[i] == true && pulseCount[i] > 0) { //on active pulse...
							if (pulseTimer[i].isTick() == true) { //if tick, deactivate
								pulseActive[i] = false;
								pulseOffTimer[i].resetTick();
								if (pulseCount[i] < 1000) { pulseCount[i]--; }
							}
							else if (pulseTimer[i].isTick() == false) { //if not tick, maintain adjoin vibration
								adjoinSpd(lSpd, gamepad.leftSpeed[i]);
								adjoinSpd(rSpd, gamepad.rightSpeed[i]);
							}
						}
						else if (pulseActive[i] == false && pulseCount[i] > 0) { //on deactive pulse...
							if (pulseOffTimer[i].isTick() == true) { //if tick, activate pulse and adjoin vibration ELSE if not tick, adjoin nothing
								adjoinSpd(lSpd, gamepad.leftSpeed[i]);
								adjoinSpd(rSpd, gamepad.rightSpeed[i]);
								pulseActive[i] = true;
								pulseTimer[i].resetTick();
							}
						}
					}
					else if (gamepad.buttonMap[i] == true && gamepad.buttonsPressed[i] == false) { //when mapped button released...
						if (pulseActive[i] == true && pulseCount[i] > 0) { //on active pulse... (pulse mode will never be true and == 0 at the same time)
							if (pulseTimer[i].isTick() == true) { //if tick, deactivate
								pulseActive[i] = false;
								pulseCount[i] = 0;
							}
							else if (pulseTimer[i].isTick() == false) { //if not tick, maintain adjoin vibration
								adjoinSpd(lSpd, gamepad.leftSpeed[i]);
								adjoinSpd(rSpd, gamepad.rightSpeed[i]);
							}
						}
						else if (pulseActive[i] == false && pulseCount[i] > 0) { //on deactive pulse...
							pulseCount[i] = 0;
						}
						else if (pulseActive[i] == true && pulseCount[i] == 0) { //set min pulse for continuous vib (only continuous can be true and == 0)
							if (pulseTimer[i].isTick() == false) { //keep vibing until time is up
								adjoinSpd(lSpd, gamepad.leftSpeed[i]);
								adjoinSpd(rSpd, gamepad.rightSpeed[i]);
							}
							else { //stop vibing
								pulseActive[i] = false;
							}
						}
					}
				} //for loop
				gamepad.vibrate(lSpd, rSpd);
			}//if connected
		} //tickCounter
	} //while loop
}

void endVibLoop() {
	exitLoop = true;
}

bool isGamepadConnected() {
	bool b = gamepad.isConnected(); //need to return b, cant return gamepad.isConnected, dunno why, P/Invoke is weird
	return b;
}

float getData(int field, int index) {
	switch (field) {
		case 0:
			return (float)gamepad.buttonMap[index];
			break;
		case 1:
			return (float)gamepad.numPulses[index];
			break;
		case 2:
			return gamepad.pulseFreq[index];
			break;
		case 3:
			return gamepad.pulseOffFreq[index];
			break;
		case 4:
			return gamepad.leftSpeed[index];
			break;
		case 5:
			return gamepad.rightSpeed[index];
			break;
		default:
			return -1;
	}
}

//***************
//
//internal functions
// 
//***************

void adjoinSpd(float &spd, float spdToAdjoin){
	if (spd < spdToAdjoin){ spd = spdToAdjoin; }
}

void printProfile(GamepadVib gpad, int numBtns, int cWidth){
	cout << gpad.profileName << endl;
	for (int i = 0; i < numBtns; i++){
		cout << setw(cWidth) << gpad.buttonMap[i];
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

		gpad.buttonMap[i] = buff.mapBuff;
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
		file << setw(cWidth) << gpad.buttonMap[i];
			file << setw(cWidth) << gpad.numPulses[i];
		file << setw(cWidth) << gpad.pulseFreq[i];
		file << setw(cWidth) << gpad.pulseOffFreq[i];
		file << setw(cWidth) << gpad.leftSpeed[i];
		file << setw(cWidth) << gpad.rightSpeed[i];
		file << '\n';
	}
}