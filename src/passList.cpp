#include "passList.h"

PassList::PassList()
{
	std::make_heap(this->pqueue.begin(), this->pqueue.end(), ComparePassOrder());
}

PassList::~PassList()
{
	for (auto& pass : internalMap) {
		delete pass.second;
	}
}

void PassList::addPass(Pass* pass, int priority)
{
	// We're taking ownership of the pass ptr so we'll be deleting that shit ourselves if this shit deletes itself
	internalMap.insert(std::make_pair(pass->name, pass));

	PassListEntry entry;
	entry.priority = priority;
	entry.pass = pass;

	pqueue.push_back(entry);
	std::push_heap(this->pqueue.begin(), this->pqueue.end(), ComparePassOrder());
}

void PassList::addPass(Pass* pass)
{
	this->addPass(pass, this->pqueue.size());
}
