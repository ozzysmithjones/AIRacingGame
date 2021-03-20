#pragma once
#include <vector>
#include <stack>

class BehaviourTree;

enum class BehaviourState 
{
	FAIL,
	RUNNING,
	SUCCESS,
};

class Behaviour 
{
public:

	virtual void Reset() = 0;
	virtual BehaviourState Resume(BehaviourState childBehaviourState) = 0;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) = 0;
};

class SequencerBehaviour : Behaviour
{
private:

	int  m_index = 0;
	std::vector<Behaviour*> m_children;

	virtual void Reset() override;
	virtual BehaviourState Resume(BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

public:
	SequencerBehaviour(Behaviour* children[], int numChildren);
	~SequencerBehaviour();
};

class SelectorBehaviour : Behaviour
{

private:

	int  m_index = 0;
	std::vector<Behaviour*> m_children;

	// Inherited via Behaviour
	virtual void Reset() override;
	virtual BehaviourState Resume(BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

public:

	SelectorBehaviour(Behaviour* children[], int numChildren);
	~SelectorBehaviour();
};

class InverterBehaviour : Behaviour
{

private:
	Behaviour* m_child;

	// Inherited via Behaviour
	virtual void Reset() override;
	virtual BehaviourState Resume(BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;
public:

	InverterBehaviour(Behaviour* child);
	~InverterBehaviour();
};


class BehaviourTree
{
private:

	Behaviour* m_root;
	std::stack<Behaviour*> m_behaviourStack;

public:

	void Reset();
	void Update(const float deltaTime);

	BehaviourTree(Behaviour* root);
	~BehaviourTree();
};

