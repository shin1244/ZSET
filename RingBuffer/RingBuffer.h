#pragma once
#include <array>
#include <atomic>

template<typename T, size_t N>
class RingBuffer
{
private:
	std::array<T, N> buffer_ = {};
	std::atomic<size_t> head_{ 0 };
	std::atomic<size_t> tail_{ 0 };
public:
	static constexpr size_t capacity() { return N - 1; }

	bool push(const T& value)
	{
		size_t t = tail_.load(std::memory_order_relaxed);
		size_t next = (t + 1) % N;

		if (next == head_.load(std::memory_order_acquire))
			return false;

		buffer_[t] = value;
		tail_.store(next, std::memory_order_release);
		return true;
	}

	bool pop(T& out)
	{
		size_t h = head_.load(std::memory_order_relaxed);

		if (h == tail_.load(std::memory_order_acquire))
			return false;
		out = buffer_[h];
		head_.store((h + 1) % N, std::memory_order_release);
		return true;
	}
};
