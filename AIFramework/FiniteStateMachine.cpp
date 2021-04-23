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

}

FiniteStateMachine::~FiniteStateMachine()
{
	for (auto pair : m_states)
	{
		delete pair.second;
	}

	m_states.clear();
}
