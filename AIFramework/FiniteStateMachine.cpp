#include "FiniteStateMachine.h"

unsigned int FiniteStateMachine::TypeID::GetNextID()
{
	static unsigned int _id = 0;
	return _id++;
}



void FiniteStateMachine::Update(const float deltaTime)
{
	if (m_currentState != nullptr)
	{
		m_currentState->Update(deltaTime);
		if (m_currentState->CanTransition())
		{
			ManageTranstition(m_currentState, m_currentStateID);
		}
	}
}

FiniteStateMachine::FiniteStateMachine()
{
	for (int i = 0; i < MAX_STATES; i++)
	{
		m_states[i] = nullptr;
	}
}

FiniteStateMachine::~FiniteStateMachine()
{
	for (int i = 0; i < MAX_STATES; i++)
	{
		if (m_states[i] != nullptr)
		{
			delete m_states[i];
		}
	}
}
