#pragma once
#include "EnStateMove.h"
#include "EnStateIdle.h"
#include "EnemyState.h"
class Enemy;
class EnStateMachine
{
public:
	EnStateMachine(Enemy* en);
	~EnStateMachine();
	bool Start();
	void Update();
	void Change(EnemyState::MoveState state);
private:
	EnStateMove m_enMove;
	EnStateIdle m_enIdle;
	IEnState *m_iens = nullptr;
	EnemyState::MoveState m_state = EnemyState::noState;
};
