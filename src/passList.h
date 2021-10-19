#pragma once
#include "pass.h"
#include <map>
#include <unordered_map>
#include <string>
#include <queue>

class PassList
{
public:
	struct PassListEntry
	{
		int priority;
		Pass* pass;
	};

	struct ComparePassOrder {
		bool operator()(PassListEntry const& p1, PassListEntry const& p2)
		{
			return p1.priority > p2.priority;
		}
	};

private:
	std::unordered_map<std::string, Pass*> internalMap;
	std::vector<PassListEntry> pqueue;

public:
	PassList();
	~PassList();

	void addPass(Pass* pass, int priority);
	void addPass(Pass* pass);

	const Pass* operator[](const char* passName) const {
		return internalMap.at(passName);
	}

	std::vector<PassListEntry>::const_iterator begin() const { return this->pqueue.begin(); };
	std::vector<PassListEntry>::const_iterator end() const { return this->pqueue.end(); };

	std::vector<PassListEntry>::iterator begin() { return this->pqueue.begin(); };
	std::vector<PassListEntry>::iterator end() { return this->pqueue.end(); };
};