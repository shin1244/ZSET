#pragma once
#include"SkipList.h"
#include<unordered_map>
#include<string>

class ZSet
{
private:
	SkipList<std::pair<int, std::string>, std::string> _sl;
	std::unordered_map<std::string, int> _dict;
public:
	void add(const std::string& member, int score) {
		auto it = _dict.find(member);
		if (it != _dict.end()) {
			int old = it->second;
			if (old == score) return;

			_sl.erase({ old, member });
		}
		_dict[member] = score;

		_sl.insert({ score , member }, member);
	}

	bool score(const std::string& member, int& out) const {
		auto it = _dict.find(member);
		if (it == _dict.end()) {
			return false;
		}

		out = it->second;
		return true;
	}

	bool erase(const std::string& member) {
		auto it = _dict.find(member);
		if (it == _dict.end()) {
			return false;
		}
		_sl.erase({ it->second, it->first });
		_dict.erase(member);

		return true;
	}
};