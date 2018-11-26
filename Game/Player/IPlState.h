#pragma once

class Player;
class PlStateMachine;
class IPlState:public IGameObject
{
public:
	IPlState(Player *pl,PlStateMachine *psm);
	~IPlState();
	virtual bool Start() = 0;
	virtual void Update() = 0;
protected:
	PlStateMachine * m_plstma = nullptr;
	Player * m_player = nullptr;
};

