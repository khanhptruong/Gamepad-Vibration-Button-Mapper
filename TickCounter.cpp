#include "TickCounter.h"

TickCounter::TickCounter(){
	_tickLength = 16666; //16666 microseconds = 60Hz
	_tickFreq = 60.0f;
	_start = std::chrono::high_resolution_clock::now();
}

TickCounter::TickCounter(float tickFreq){
	if (tickFreq > 0){
		_tickFreq = tickFreq;
	}else{
		_tickFreq = 60.0f;
	}
	_tickLength = 1E6 / _tickFreq;
	_start = std::chrono::high_resolution_clock::now();
}

bool TickCounter::isTick(){
	_end = std::chrono::high_resolution_clock::now();
	int microseconds = std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count();

	if (microseconds >= _tickLength){
		_start = std::chrono::high_resolution_clock::now();
		return true;
	}else{
		return false;
	}
}

void TickCounter::resetTick(){
	_start = std::chrono::high_resolution_clock::now();
}

void TickCounter::setFreq(float tickFreq){
	if (tickFreq > 0){
		_tickFreq = tickFreq;
	}
	else{
		_tickFreq = 60.0f;
	}
	_tickLength = 1E6 / _tickFreq;
	_start = std::chrono::high_resolution_clock::now();
}