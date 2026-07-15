#pragma once

template<typename Value>
class TimingWheel
{
private:
	struct TimerNode
	{
		Value value;
		int rotation;
		TimerNode* next;
	};
	static constexpr int WHEEL_SIZE = 3600;
	TimerNode* slots[WHEEL_SIZE]; 
	int        currentTick = 0;
public:
};