#pragma once
#include <stdexcept>
#include <unordered_map>

class State
{
public:
	virtual void Initialise() = 0;
	virtual void Enter() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Leave() = 0;

	virtual bool CanTransition() = 0;
};


class FiniteStateMachine
{
private:

	int	   m_currentStateID = -1;
	State* m_currentState = nullptr;
	std::unordered_map<int, State*> m_states;

protected:

	virtual void ManageTranstition(State* currentState, int typeID) = 0;

	//To organise states by type using templates
	class TypeID
	{
	private:

		static unsigned int GetNextID();

	public:

		template<typename T>
		static inline unsigned int GetID()
		{
			static unsigned int id = GetNextID();
			return id;
		}
	};

	template<typename T>
	void AddState(T* state, bool isDefaultState = false)
	{
		int id = TypeID::GetID<T>();
		m_states[id] = state;
		m_states[id]->Initialise();

		if (isDefaultState && m_currentState == nullptr)
		{
			m_currentState = state;
			m_currentStateID = id;
		}
	}

	template<typename T>
	void TransitionTo()
	{
		int id = TypeID::GetID<T>();

		if (m_states.find(id) == m_states.end())
		{
			throw std::exception("There is a missing state in the state machine");
		}

		if (m_currentState != nullptr) 
		{
			m_currentState->Leave();
		}

		m_currentState = m_states[id];
		m_currentStateID = id;
		m_currentState->Enter();
	}

public:

	void Update(const float deltaTime);
	
	FiniteStateMachine();
	~FiniteStateMachine();
};



