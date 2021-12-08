#include "TickCounter.h"

TickCounter::TickCounter(){
	_tickRate = 16666; //16666 microseconds = 60fps
	_start = std::chrono::high_resolution_clock::now();
}

TickCounter::TickCounter(int tickRate){
	if (tickRate > 0){
		_tickRate = tickRate;
	}else{
		_tickRate = 16666; //16666 microseconds = 60fps
	}
	_start = std::chrono::high_resolution_clock::now();
}

bool TickCounter::isTick(){
	_end = std::chrono::high_resolution_clock::now();
	int microseconds = std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count();

	if (microseconds >= _tickRate){
		_start = std::chrono::high_resolution_clock::now();
		return true;
	}else{
		return false;
	}
}

void TickCounter::resetTick(){
	_start = std::chrono::high_resolution_clock::now();
}