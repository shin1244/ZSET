#pragma once
#include"SkipList.h"
#include<unordered_map>
#include<string>

class ZSet
{
private:
	SkipList<std::pair<int, std::string>, std::string> sl_;
	std::unordered_map<std::string, int> dict_;
public:
	void add(const std::string& member, int score) {
		auto it = dict_.find(member);
		if (it != dict_.end()) {
			int old = it->second;
			if (old == score) return;
			sl_.erase({ -old, member });
		}
		dict_[member] = score;

		sl_.insert({ -score , member }, member);
	}

	bool score(const std::string& member, int& out) const 
	{
		const auto it = dict_.find(member);
		if (it == dict_.end()) {
			return false;
		}

		out = it->second;
		return true;
	}

	bool erase(const std::string& member)
	{
		const auto it = dict_.find(member);
		if (it == dict_.end()) {
			return false;
		}
		sl_.erase({ it->second, it->first });
		dict_.erase(member);

		return true;
	}

	int rank(const std::string& member) const {
		const auto it = dict_.find(member);
		if (it == dict_.end()) return -1;
		return sl_.getRank({ -it->second, member });
	}

	bool getByRank(int rank, std::string& outMember, int& outScore) const {
		std::pair<int, std::string> key;
		std::string value;
		if (!sl_.getByRank(rank, key, value)) return false;
		outMember = value;
		outScore = -key.first;
		return true;
	}

	std::vector<std::pair<std::string, int>> range(int start, int stop) const {
		std::vector<std::pair<std::string, int>> result;
		for (int i = start; i <= stop; ++i) {
			std::pair<int, std::string> key;
			std::string value;
			if (sl_.getByRank(i, key, value)) {
				result.emplace_back(value, -key.first);
			}
		}
		return result;
	}

	int count() const {
		return sl_.size();
	}

	bool incrBy(const std::string& member, int delta, int& out)
	{
		auto it = dict_.find(member);
		int newScore;
		if (it == dict_.end())
		{	
			newScore = delta;
		} 
		else
		{
			newScore = it->second + delta;
		}
		add(member, newScore);
		out = newScore;
		return true;
	}
};