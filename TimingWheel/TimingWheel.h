#pragma once
#include <unordered_map>

template<typename Value>
class TimingWheel
{
private:
	
	struct Node
	{
		Value value;
		int rotation;
		Node* prev;
		Node* next;

		Node(const Value& v, const int r) : value(v), rotation(r), prev(nullptr), next(nullptr){}
		~Node() {}
	};
	static constexpr int WHEEL_SIZE = 3600;
	Node* slots_[WHEEL_SIZE]; 
	int currentTick_ = 0;
	std::unordered_map<int, Node*> map_;

	void append(int tick, Node* n)
	{
		slots_[tick]->prev = n;
		n->prev = nullptr;
		n->next = slots_[tick];
		slots_[tick] = n;
	}
public:
	int add(int delayTicks, const Value& value)
	{	
		int insertRot = delayTicks / WHEEL_SIZE;
		int insertTick = (currentTick_ + delayTicks) % WHEEL_SIZE;
		Node* n = new Node(value, insertRot);

		append(insertTick, n);
		return insertTick;
	}
	void remove(int id);  
	void tick()
	{
		currentTick_ = (currentTick_ + 1) % WHEEL_SIZE;

		Node* cur = slots_[currentTick_];
		Node* prev = nullptr;
		while (cur != nullptr)
		{
			if (cur->rotation == 0)
			{
				Node* nextNode = cur->next;
				if (prev == nullptr) slots_[currentTick_] = nextNode;
				else prev->next = nextNode;
				delete cur;
				cur = nextNode;
			}
			else
			{
				prev = cur;
				--cur->rotation;
				cur = cur->next;
			}
		}
	}
};