#include "BehaviourTree.h"

void BehaviourTree::Reset()
{
	while (m_behaviourStack.size() > 1)
	{
		m_behaviourStack.top()->Reset();
		m_behaviourStack.pop();
	}

	m_root->Reset();
}

void BehaviourTree::Update(const float deltaTime)
{
	Behaviour* child;
	do 
	{
		child = nullptr;
		BehaviourState behaviourState = m_behaviourStack.top()->Update(deltaTime, child);

		if (child != nullptr)
		{
			m_behaviourStack.push(child);
		}
		else 
		{
			while (behaviourState != BehaviourState::RUNNING && m_behaviourStack.size() > 1)
			{
				m_behaviourStack.pop();
				behaviourState = m_behaviourStack.top()->Resume(behaviourState);
			}
		}

	} while (child != nullptr);
}

BehaviourTree::BehaviourTree(Behaviour* root)
{
	m_root = root;
	m_behaviourStack.push(root);
}

BehaviourTree::~BehaviourTree()
{
	delete m_root;
	m_root = nullptr;
}

BehaviourState SequencerBehaviour::Resume(BehaviourState childBehaviourState)
{
	if(childBehaviourState == BehaviourState::FAIL)
	{
		m_index = 0;
		return BehaviourState::FAIL;
	}

	return BehaviourState::RUNNING;
}

BehaviourState SequencerBehaviour::Update(const float deltaTime, Behaviour*& childToPush)
{
	if (m_index >= m_children.size())
	{
		m_index = 0;
		return BehaviourState::SUCCESS;
	}

	childToPush = m_children[m_index];
	m_index++;

	return BehaviourState::RUNNING;
}

SequencerBehaviour::SequencerBehaviour(Behaviour* children[], int numChildren)
{
	for (int i = 0; i < numChildren; i++)
	{
		m_children.push_back(children[i]);
	}
}

SequencerBehaviour::~SequencerBehaviour()
{
	for (auto p : m_children)
	{
		delete p;
		p = nullptr;
	}
}

void SequencerBehaviour::Reset()
{
	m_index = 0;
}

void SelectorBehaviour::Reset()
{
	m_index = 0;
}


BehaviourState SelectorBehaviour::Resume(BehaviourState childBehaviourState)
{
	if (childBehaviourState == BehaviourState::FAIL)
	{
		if (m_index >= m_children.size())
		{
			m_index = 0;
			return BehaviourState::FAIL;
		}

		return BehaviourState::RUNNING;
	}

	if (childBehaviourState == BehaviourState::SUCCESS)
	{
		m_index = 0;
		return BehaviourState::SUCCESS;
	}

	return BehaviourState::RUNNING;
}

BehaviourState SelectorBehaviour::Update(const float deltaTime, Behaviour*& childToPush)
{
	if (m_index >= m_children.size())
	{
		m_index = 0;
		return BehaviourState::FAIL;
	}

	childToPush = m_children[m_index];
	m_index++;

	return BehaviourState::RUNNING;
}

SelectorBehaviour::SelectorBehaviour(Behaviour* children[], int numChildren)
{
	for (int i = 0; i < numChildren; i++)
	{
		m_children.push_back(children[i]);
	}
}

SelectorBehaviour::~SelectorBehaviour()
{
	for (auto p : m_children)
	{
		delete p;
		p = nullptr;
	}
}

void InverterBehaviour::Reset()
{
}

BehaviourState InverterBehaviour::Resume(BehaviourState childBehaviourState)
{
	switch (childBehaviourState)
	{
	case BehaviourState::SUCCESS:
		return BehaviourState::FAIL;
	case BehaviourState::FAIL:
		return BehaviourState::SUCCESS;
	}

	return childBehaviourState;
}

BehaviourState InverterBehaviour::Update(const float deltaTime, Behaviour*& childToPush)
{
	childToPush = m_child;
	return BehaviourState::RUNNING;
}

InverterBehaviour::InverterBehaviour(Behaviour* child)
{
	m_child = child;
}

InverterBehaviour::~InverterBehaviour()
{
	delete m_child;
	m_child = nullptr;
}
