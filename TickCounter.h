#ifndef TICKCOUNTER_H
#define TICKCOUNTER_H

#include <chrono>

class TickCounter
{
public:
	TickCounter();
	TickCounter(float tickFreq);

	bool isTick();
	void resetTick();
	void setFreq(float tickFreq);
private:
	long int _tickLength; //microseconds
	float _tickFreq;   //Hz
	std::chrono::time_point<std::chrono::high_resolution_clock> _start, _end;
};

#endif // TICKCOUNTER_H