#pragma once
#include <random>
#include <vector>

template<typename Key, typename Value>
class SkipList {
private:
	static constexpr int MAX_LEVEL = 16;
	struct Node
	{
		Key key;
		Value value;
		struct Link {
			Node* next = nullptr;
			int span = 0;
		};
		Link* forward;
		int level;

		Node(const Key& k, const Value& v, const int lvl) {
			key = k;
			value = v;
			level = lvl;
			forward = new Link[lvl + 1];
		}
		~Node() {
			delete[] forward;
		}
	};

	Node* head_;
	int level_;
	int size_;

	static int randomLevel() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> dis(0, 1);

		int lvl = 0;

		while (dis(gen) == 1 && lvl < MAX_LEVEL) lvl++;
		return lvl;
	}
public:
	SkipList() {
		level_ = 0;
		size_ = 0;
		head_ = new Node(Key{}, Value{}, MAX_LEVEL);
	}
	~SkipList() {
		Node* cur = head_->forward[0];
		while (cur != nullptr)
		{
			Node* next = cur->forward[0];
			delete cur;
			cur = next;
		}
		delete head_;
	}

	Node* search(const Key& key) const {
		Node* cur = head_;
		for (int i = level_; i >= 0; --i) 
		{
			while (cur->forward[i].next != nullptr 
				&& cur->forward[i].next->key < key)
				cur = cur->forward[i].next;
		}
		cur = cur->forward[0].next;
		if (cur != nullptr && cur->key == key) return cur;
		return nullptr;
	}

	void insert(const Key& key, const Value& value) {
		Node* update[MAX_LEVEL + 1];
		int rank[MAX_LEVEL + 1];

		for (int i = 0; i <= MAX_LEVEL; ++i)
		{
			update[i] = nullptr;
			rank[i] = 0;
		}

		Node* cur = head_;

		for (int i = level_; i >= 0; --i)
		{
			rank[i] = (i == level_ ? 0 : rank[i + 1]);
			while (cur->forward[i].next != nullptr 
				&& cur->forward[i].next->key < key)
			{
				rank[i] += cur->forward[i].span;
				cur = cur->forward[i].next;
			}
			update[i] = cur;
		}

		cur = cur->forward[0].next;
		if (cur != nullptr && cur->key == key)
		{
			cur->value = value;
			return;
		}
		int lvl = randomLevel();
		if (lvl > level_)
		{
			for (int i = level_ + 1; i <= lvl; ++i)
			{
				rank[i] = 0;
				update[i] = head_;
				head_->forward[i].span = size_;
			}
			level_ = lvl;
		}
		Node* n = new Node(key, value, lvl);

		for (int i = 0; i <= lvl; ++i)
		{
			n->forward[i].next = update[i]->forward[i].next;
			update[i]->forward[i].next = n;
			
			n->forward[i].span = update[i]->forward[i].span - (rank[0] - rank[i]);
			update[i]->forward[i].span = rank[0] - rank[i] + 1;
		}

		for (int i = lvl; i <= level_; ++i)
		{
			update[i]->forward[i].span += 1;
		}
		size_++;
	}

	bool erase(const Key& key) {
	}

	void rangeSearch(Key& k1, Key& k2, std::vector<Node*>& v) {
	}

	int size() { return size_; }
	bool empty() { return size_ == 0; }
};