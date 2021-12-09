#ifndef TICKCOUNTER_H
#define TICKCOUNTER_H

#include <chrono>

class TickCounter
{
public:
	TickCounter();
	TickCounter(int tickFreq);

	bool isTick();
	void resetTick();
	void setFreq(int tickFreq);
private:
	int _tickLength; //microseconds
	int _tickFreq;   //Hz
	std::chrono::time_point<std::chrono::high_resolution_clock> _start, _end;
};

#endif // TICKCOUNTER_H