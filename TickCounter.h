#ifndef TICKCOUNTER_H
#define TICKCOUNTER_H

//#include <Windows.h>
//#include <iomanip>
//#include <cstdlib>
#include <chrono>

class TickCounter
{
public:
	TickCounter();
	TickCounter(int tickRate);

	bool isTick();
	void resetTick();
private:
	int _tickRate;
	std::chrono::time_point<std::chrono::high_resolution_clock> _start, _end;
};

#endif // GAMEPAD_H