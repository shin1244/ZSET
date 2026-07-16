#pragma once
#include <array>
#include <vector>
using namespace std;

template<typename T, size_t N>
class ObjectPool
{
private:
	std::array<T, N> pool_;
	std::vector<size_t> free_;
public:
	ObjectPool()
	{
		free_.reserve(N);
		for (size_t i = 0; i < N; ++i)
			free_.emplace_back(i);
	}
	T* allocate()
	{
		if (free_.empty()) return nullptr;
		size_t idx = free_.back();
		free_.pop_back();
		return &pool_[idx];
	}
	void deallocate(T* obj)
	{
		const size_t idx = obj - pool_.data();
		free_.push_back(idx);
	}
	size_t available() const { return free_.size(); }  
	static size_t capacity() { return N; }
	bool   empty()    const { return free_.empty(); } 
};
