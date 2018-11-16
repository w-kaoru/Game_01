#pragma once

class Player;
class PlStateMachine;
class IPlState:public IGameObject
{
public:
	IPlState(Player *pl,PlStateMachine *psm);
	~IPlState();
	bool Start() = 0;
	void Update() = 0;
protected:
	PlStateMachine * m_plstma = nullptr;
	Player * m_player = nullptr;
};

