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
	virtual void Start() = 0;
	virtual BehaviourState Resume(Behaviour* child,BehaviourState childBehaviourState) = 0;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) = 0;
};

class SequencerBehaviour : public Behaviour
{
private:

	int  m_index = 0;
	std::vector<Behaviour*> m_children;

	virtual void Reset() override;
	virtual void Start() override;
	virtual BehaviourState Resume(Behaviour* child,BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

public:
	SequencerBehaviour(Behaviour* children[], int numChildren);
	~SequencerBehaviour();
};

class SelectorBehaviour : public Behaviour
{

private:

	int  m_index = 0;
	std::vector<Behaviour*> m_children;

	// Inherited via Behaviour
	virtual void Reset() override;
	virtual void Start() override;
	virtual BehaviourState Resume(Behaviour* child,BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

public:

	SelectorBehaviour(Behaviour* children[], int numChildren);
	~SelectorBehaviour();
};

class InverterBehaviour : public Behaviour
{

private:
	Behaviour* m_child;

	// Inherited via Behaviour
	virtual void Reset() override;
	virtual void Start() override;
	virtual BehaviourState Resume(Behaviour* child,BehaviourState childBehaviourState) override;
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

